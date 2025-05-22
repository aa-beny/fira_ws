import rclpy  # 匯入 ROS 2 的 Python 函式庫，讓你可以使用 ROS 的功能寫節點
from rclpy.node import Node  # 匯入 Node 類別，是 ROS 節點的基本結構
from std_msgs.msg import Bool, String  # 匯入兩種基本訊息型別：布林值和字串
import time  # 匯入 time 模組，用來加入延遲（例如 sleep）
import threading  # 匯入 threading 模組，用來建立背景執行的主迴圈

# 定義一個名為 TaskMaster 的類別，這是我們的主控制器，負責控制任務流程
class TaskMaster(Node):
    def __init__(self):
        super().__init__('task_master')  # 建立 ROS 節點，名稱為 'task_master'

        self.state = 'WAIT_FOR_CAR'  # 初始狀態設定為等待車子抵達取貨區
        self.processed_letters = set()  # 使用一個集合來記錄已經抓過的字母（F, I, R, A）

        # 訂閱車子到達的訊號，當 /car_arrived 主題發出 True 時，表示車子抵達
        self.create_subscription(Bool, '/car_arrived', self.car_arrived_callback, 10)
        # 訂閱 OCR 模組辨識到的文字訊息（例如 "F,512,384;I,200,300"）
        self.create_subscription(String, '/ocr_results', self.ocr_callback, 10)
        self.create_subscription(Bool, '/pose_goal_status', self.pose_status_callback, 10)
        self.pose_done = False
        # 建立可以發送的 ROS 主題
        self.allow_pub = self.create_publisher(Bool, '/allow_publish_pose', 10)  # 發布是否允許 depth 模組發布座標
        self.detect_fira = self.create_publisher(Bool, '/do_detect_fira', 10)  # 通知手臂回到拍照位置
        self.gripper_cmd = self.create_publisher(Bool, '/gripper_command', 10)  # 控制夾爪夾取或放開
        self.car_to_drop = self.create_publisher(Bool, '/car_to_drop_zone', 10)  # 通知車子前往放置區
        self.ocr_target_pub = self.create_publisher(String, '/target_ocr', 10)  # 傳送要追蹤的字母與畫面座標給 depth 模組
        self.car_command_pub = self.create_publisher(String, '/car_command', 10)

        self.task_mode_pub = self.create_publisher(Bool, '/task_mode', 10)
        

        self.current_letter = None  # 紀錄目前要處理的字母（例：'F'）
        self.drop_letter_pose_dict = {}  # 儲存放置區辨識出來的所有字母與畫面座標
        self.car_arrived_for_drop = False  # 記錄車子是否已經抵達放置區
        self.allow_pose_disabled_for_drop = False  # 確保放置階段的 allow_pose 只發一次

        self.pose_resend_timer = self.create_timer(0.2, self.pose_timer_callback)
        # 新增：分檢流程設定
        self.sorting_zones = ['B', 'C', 'D']  # 三個分檢區
        self.all_letters = ['F', 'I', 'R', 'A']  # 四個字母
        self.letter_to_zone = {}  # （可擴充：每個字母屬於哪個區）
        self.sorting_tasks = []  # 儲存要去哪些區（目前未使用）
        self.current_zone_index = 0  # 現在第幾個分檢區
        
        self.get_logger().info('✅ Task Master node initialized')  # 印出初始化完成訊息
        self.get_logger().info('🚀 Starting: sending car to first sorting zone B')
        self.car_command_pub.publish(String(data='B'))
        self.waiting_for_pose = False  # ➕ 加這個

        # 開始執行主迴圈，這是一個背景執行緒，每 0.2 秒執行一次
        threading.Thread(target=self.main_loop, daemon=True).start()

    # 車子抵達時會收到這個 callback 訊息
    def car_arrived_callback(self, msg):
        if msg.data and self.state == 'WAIT_FOR_CAR':  # 如果現在狀態是等待車子，而且訊息為 True
            self.get_logger().info('🚗 [CALLBACK] Car arrived at PICK zone → switching to PICK_MODE')

            self.allow_pose_disabled_for_drop = False  # 重設旗標以允許新的 pose 發布
            self.send_gripper(True)
            self.state = 'PICK_MODE'  # 進入抓取階段
            
            # 每次切換模式時呼叫
            self.task_mode_pub.publish(Bool(data=(self.state == 'DROP_MODE')))

            self.pose_done = False

        elif self.state == 'DROP_MODE':  # 如果現在是放置階段且車子到了
            self.get_logger().info('🚗 [CALLBACK] Car arrived at DROP zone → ready to place object')

            self.allow_pose_disabled_for_drop = False
            self.car_arrived_for_drop = True  # 標記已到達放置區
            self.task_mode_pub.publish(Bool(data=(self.state == 'DROP_MODE')))
            self.pose_done = False

    # OCR 模組辨識出文字後會進入這個函式
    def ocr_callback(self, msg):
        self.get_logger().info(f"🔁 [OCR] State={self.state}, Letter={self.current_letter}, Completed={self.processed_letters}")


        # 如果現在是在夾取階段
        if self.state == 'PICK_MODE':
            letters = []  # 儲存尚未處理過的字母資料
            for entry in msg.data.strip().split(';'):  # 把整串結果切開，逐一處理每筆資料（格式如 F,512,384）
                parts = entry.split(',')  # 把字母與座標拆開成 list，例如 ['F', '512', '384']
                if len(parts) == 3:
                    letter = parts[0].upper()  # 將字母轉成大寫
                    if letter in {'F', 'I', 'R', 'A'} and letter not in self.processed_letters:
                        letters.append(entry)  # 如果是合法字母且沒抓過，就加入清單中

            if letters:
                selected = letters[0]  # 先挑第一個尚未處理的字母資料
                self.current_letter = selected.split(',')[0].upper()  # 設定目前要抓的字母
                self.get_logger().info(f"🔍 [PICK] Selected letter: {self.current_letter} → sending pose request + waiting to grab")

                self.ocr_target_pub.publish(String(data=selected))  # 把該目標發給 depth node 處理座標
                # self.allow_pub.publish(Bool(data=True))  # 通知 depth node 可以發布座標了
                if not self.allow_pose_disabled_for_drop:
                    self.allow_pub.publish(Bool(data=True))  # 通知 depth node 可以發布座標了
                    self.allow_pose_disabled_for_drop = True  # 標記已發送過
                # time.sleep(3.5)  # 等待手臂完成動作

                # while not self.pose_done and rclpy.ok():
                if not self.pose_done:
                    self.get_logger().info("⏳ pick Waiting for arm to complete motion...")
                    self.ocr_target_pub.publish(String(data=selected))  # 把該目標發給 depth node 處理座標
                    # time.sleep(0.2)
                # else:
                #     self.pose_done = False  # reset for next action

                #     self.send_gripper(False)  # 發送夾取指令
                #     self.move_to_photo_pose()  # 讓手臂回到拍照點
                #     self.get_logger().info('🔁 Returned to photo position, sending car to drop zone')
                #     self.car_to_drop.publish(Bool(data=True))  # 通知車子去放置區
                #     self.state = 'DROP_MODE'  # 切換狀態為放置模式

        # 如果是放置階段，並且有字母要處理
                # 如果目前是在放置階段，並且已經有記錄手上夾的是哪一個字母（self.current_letter）
        elif self.state == 'DROP_MODE' and self.current_letter:
                # 如果車子還沒到達放置區，就不繼續執行放置
            if not self.car_arrived_for_drop:
                # 顯示目前系統正等待車子抵達放置區，這行會一直出現在 log 裡直到車子到
                self.get_logger().info("[WAIT] Waiting for car to arrive at drop zone before placing object...")
                return
            new_dict = {}  # 準備一個字典來存放：畫面上每個字母所對應的位置（畫面 pixel 座標）
            for entry in msg.data.strip().split(';'):  # 把整串 OCR 資料用分號切開，一筆一筆處理
                parts = entry.split(',')  # 把像 "F,512,384" 這樣的格式分成 ['F', '512', '384']
                if len(parts) == 3:
                    letter = parts[0].upper()  # 取得字母並轉成大寫（保險做法）
                    if letter in {'F', 'I', 'R', 'A'}:  # 如果這筆是我們要處理的目標字母
                        try:
                            u = int(float(parts[1]))  # 把第二欄（像素 x）轉成整數
                            v = int(float(parts[2]))  # 把第三欄（像素 y）轉成整數
                            new_dict[letter] = (u, v)  # 把這個字母和座標加到字典中
                        except:
                            continue  # 如果資料格式錯誤就跳過

            # 最後，把這個包含所有字母座標的字典記錄下來，等等要根據 self.current_letter 來查位置
            self.drop_letter_pose_dict = new_dict  # 儲存放置區辨識資料
            self.get_logger().info(f"📦 [DROP] OCR result: {self.drop_letter_pose_dict}")

                        # 如果目前要放的字母有出現在辨識結果中
            if self.current_letter in self.drop_letter_pose_dict:
                # 從字典中取出該字母對應的畫面座標 (u, v)
                u, v = self.drop_letter_pose_dict[self.current_letter]
                # 組合成字串格式，例如 'F,512,384'，要傳給 depth 模組
                target_msg = f"{self.current_letter},{u},{v}"
                # 顯示我們目前要放哪個字母、準備對準其位置
                self.get_logger().info(f"[DROP] Selected letter '{self.current_letter}' for placement at pixel ({u}, {v})")
                # 發布給 depth 模組，請它針對這個像素位置發出 3D 座標
               
                if not self.allow_pose_disabled_for_drop:
                    self.allow_pub.publish(Bool(data=True))  # 通知 depth node 可以發布座標了
                    self.allow_pose_disabled_for_drop = True  # 標記已發送過

                self.get_logger().info("⏳ Waiting for arm to place object...")
                # time.sleep(3.5)  # 等手臂放置動作完成
                self.ocr_target_pub.publish(String(data=target_msg))
                # while not self.pose_done and rclpy.ok():
                if not self.pose_done:
                    self.ocr_target_pub.publish(String(data=target_msg))  # 發送該字母位置給 depth node
                    self.get_logger().info("⏳ DROP Waiting for arm to complete motion...")
                    # time.sleep(0.2)
                # else:
                #     self.pose_done = False  # reset for next action


                #     self.send_gripper(True)  # 發送放開指令
                #     self.get_logger().info(f'✅ Successfully placed [{self.current_letter}]')
                #     self.move_to_photo_pose()  # 回拍照點
                #     self.processed_letters.add(self.current_letter)  # 標記該字母已處理
                #     self.current_letter = None

                #     self.car_arrived_for_drop = False

                #     self.state = 'WAIT_FOR_CAR'  # 回到等待車子階段
            else:
                self.get_logger().warn(f"⚠️ [DROP] Could not find letter {self.current_letter}")

    # 發送控制手臂回到拍照點的訊號
    def move_to_photo_pose(self):
        self.get_logger().info("📸 Returning to photo pose")
        self.detect_fira.publish(Bool(data=True))
        time.sleep(10)

        # while not self.pose_done and rclpy.ok():
        #     self.get_logger().info("⏳ photo_pose Waiting for arm to complete motion...")
        #     time.sleep(0.2)
        # self.pose_done = False  # reset for next action

        self.detect_fira.publish(Bool(data=False))

    # 發送夾爪控制指令（False 代表夾住，True 代表放開）
        # This function sends a gripper command to either grab or release an object
        # 發送控制夾爪的指令（False表示夾取，True表示放開）
    def send_gripper(self, close):
        action = "GRAB" if close else "RELEASE"  # 根據輸入的布林值決定動作名稱（英文顯示）
        self.get_logger().info(f"[GRIPPER] Executing action: {action}")  # 印出目前正在執行的夾爪動作
        self.gripper_cmd.publish(Bool(data=close))  # 將指令發送給 /gripper_command 主題
        self.get_logger().debug(f"[GRIPPER] Bool message published: {close}")  # 除錯用，確認傳出的布林值
        time.sleep(1.5)  # 暫停 1.5 秒，給夾爪執行時間  # Wait to allow gripper hardware time to finish moving

    def pose_status_callback(self, msg):
        self.get_logger().info(f"📬 Received pose_goal_status: {msg.data}")  # 加這行
        # if msg.data == "[POSE_GOAL] Success":
        if msg.data:
            self.get_logger().info("🟢 Arm reported success")
            self.pose_done = True
            self.waiting_for_pose = False  # ➕ 當任務成功，清除等待狀態
            if self.state == 'PICK_MODE':
                self.send_gripper(False)  # 夾住
                self.move_to_photo_pose()  # 回拍照點
                self.get_logger().info('🔁 Returned to photo position, sending car to drop zone')
                self.car_to_drop.publish(Bool(data=True))  # 通知車子去放置區
                self.car_command_pub.publish(String(data='E'))
                self.state = 'DROP_MODE'  # 切換到放置階段


            elif self.state == 'DROP_MODE':
                self.send_gripper(True)  # 放開
                self.get_logger().info(f'✅ Successfully placed [{self.current_letter}]')
                self.move_to_photo_pose()  # 回拍照點
                self.processed_letters.add(self.current_letter)  # 標記已處理
                self.current_letter = None
                self.car_arrived_for_drop = False
                                # 檢查是否要切換分檢區或回家
                # 🔍 檢查我們是不是已經把 4 個字母都處理完了？
                # 這邊會去看 self.processed_letters 這個「已處理字母清單」裡面有幾個字
                # 如果已經有 4 個字母（F, I, R, A）就代表任務完成
                if len(self.processed_letters) == 4:

                    # ✅ 印出「任務完成，準備回家」
                    self.get_logger().info("🏁 All letters done. Going home.")

                    # 🏡 發一個指令給車子：「去 A 地點」→ 代表回家
                    self.car_command_pub.publish(String(data='A'))

                # 🚗 如果還沒完成所有字母，而且還有下一個分檢區可以去，就進入這個判斷
                elif self.current_zone_index < len(self.sorting_zones):

                    # 🧭 從 sorting_zones（像 ['B', 'C', 'D']）這個清單中，找現在要去的區域
                    # 比如 current_zone_index = 1，就會是 C
                    next_zone = self.sorting_zones[self.current_zone_index]

                    # ✅ 印出訊息：「準備去下一個分檢區」
                    self.get_logger().info(f"🚗 Moving to next sorting zone {next_zone}")

                    # 🗺️ 發一個指令給車子：「去分檢區 B / C / D」
                    self.car_command_pub.publish(String(data=next_zone))

                    # ➕ 下一次就跳到下一個區（B → C → D）
                    self.current_zone_index += 1

                # 🔄 無論是回家還是去下一區，我們都會把狀態切換成「等待車子到」
                # 接下來就等 /car_arrived 的訊號來觸發下一步
                # self.state = 'WAIT_FOR_CAR'
                self.state = 'WAIT_FOR_CAR'  # 回到等車階段
            # elif msg.data == "[POSE_GOAL] Failed":
        else:
            self.get_logger().warn("🔴 Arm reported failure")
            self.pose_done = False
    def pose_timer_callback(self):
        if self.waiting_for_pose and self.last_target_msg:
            self.get_logger().info("🔁 Resending target while waiting for pose done...")
            self.ocr_target_pub.publish(self.last_target_msg)

    # 背景執行主迴圈，每 0.2 秒印一次目前狀態，方便除錯
        # This function runs continuously in the background to print system state for debugging
        # 背景執行的主迴圈，用來每 0.2 秒印出目前狀態，方便除錯
    def main_loop(self):
        while rclpy.ok():  # 如果 ROS 還在正常運作中
            self.get_logger().debug(f"[LOOP] Current state: {self.state}, Current letter: {self.current_letter}, Letters done: {self.processed_letters}")  # 顯示目前狀態與字母
            time.sleep(0.2)  # 暫停 0.2 秒，降低 CPU 使用與 log 轟炸  # Wait 0.2 seconds before looping again to reduce console spam

# ROS 節點主程式入口

def main():
    rclpy.init()  # 初始化 ROS
    node = TaskMaster()  # 建立節點
    rclpy.spin(node)  # 讓節點一直跑著（接收與處理訊息）
    node.destroy_node()  # 關閉節點
    rclpy.shutdown()  # 關閉 ROS

if __name__ == '__main__':
    main()

