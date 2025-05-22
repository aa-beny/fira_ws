# 載入 ROS2 的必要模組
import rclpy  # ROS2 的 Python 套件，負責啟動節點
from rclpy.node import Node  # 所有 ROS2 節點都要繼承這個類別

# 這是影像的資料格式，用來接收來自相機的畫面
from sensor_msgs.msg import Image

# 這是 ROS 中用來發送純文字的訊息格式
from std_msgs.msg import String

# 這個模組可以把 ROS 的影像資料轉成 OpenCV 可以處理的格式
from cv_bridge import CvBridge

# OpenCV 是一個圖像處理工具，用來分析畫面
import cv2

# NumPy 是數學函式庫，用來處理矩陣（像是影像資料）
import numpy as np


# 定義一個類別 ColorObjectTracker，繼承自 ROS2 的 Node（也就是節點）
class ColorObjectTracker(Node):
    def __init__(self):
        # 呼叫父類別的建構函式，設定這個節點的名字叫 color_object_tracker
        super().__init__('color_object_tracker')

        # 建立一個 bridge，讓 ROS 的影像可以轉成 OpenCV 的格式
        self.bridge = CvBridge()
        
        # 訂閱相機畫面，當有新的畫面時會叫 color_callback 這個函式來處理
        self.color_sub = self.create_subscription(Image, '/camera/color', self.color_callback, 10)

        # 準備一個發佈器，用來發送我們分析出來的顏色順序（例如 "Blue,Green,Yellow"）
        self.color_pub = self.create_publisher(String, '/color_sort', 10)

        # 定義三個顏色的 HSV 範圍（HSV 是一種顏色表達方式，比 RGB 更容易分辨顏色）
        self.color_range = {
            "Blue": {'min': [78, 89, 24], 'max': [115, 255, 255]},
            "Yellow": {'min': [13, 104, 55], 'max': [46, 221, 255]},
            "Green": {'min': [42, 52, 30], 'max': [77, 255, 255]}
        }

        # 定義這些顏色在畫面上要用什麼顏色畫框（BGR 格式）
        self.bgr_color = {
            "Blue": (255, 0, 0),      # 藍色
            "Yellow": (0, 255, 255),  # 黃色
            "Green": (0, 255, 0)      # 綠色
        }

    # 處理相機影像的函式
    def color_callback(self, msg):
        # 把 ROS 的影像資料轉成 OpenCV 的 BGR 格式影像
        frame = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')

        # 把 BGR 顏色空間轉成 HSV，方便我們用 HSV 範圍來找顏色
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # 建立一個 list，用來存放我們找到的物體中心點 X 座標 和 顏色名稱
        detected_objects = []

        # 針對每一種顏色開始偵測
        for color_name, hsv_range in self.color_range.items():
            # 把 HSV 範圍轉成 NumPy 陣列
            lower = np.array(hsv_range['min'])
            upper = np.array(hsv_range['max'])

            # 產生遮罩，只保留在這個顏色範圍內的區域（其他全部變成黑色）
            mask = cv2.inRange(hsv, lower, upper)

            # 找遮罩中的所有輪廓（也就是可能的顏色區塊）
            contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

            # 一個一個檢查輪廓
            for cnt in contours:
                # 如果這個輪廓太小（面積小於 500），就跳過不要理他
                if cv2.contourArea(cnt) > 500:
                    # 用矩形包住這個區塊
                    x, y, w, h = cv2.boundingRect(cnt)
                    # 算出這個矩形的中心點 X 座標
                    cx = x + w // 2

                    # 把這個物體記錄下來（X 座標 + 顏色）
                    detected_objects.append((cx, color_name))

                    # 在畫面上畫出矩形框和文字
                    cv2.rectangle(frame, (x, y), (x + w, y + h), self.bgr_color[color_name], 2)
                    cv2.putText(frame, color_name, (x, y - 10),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.7, self.bgr_color[color_name], 2)

        # 把所有找到的顏色物體依照 X 座標排序（從左到右）
        detected_objects.sort(key=lambda obj: obj[0])

        # 取出排序好的顏色名稱（只要 color_name 部分）
        sorted_colors = [obj[1] for obj in detected_objects]

        # 如果有找到顏色物體就發送出去
        if sorted_colors:
            # 把顏色陣列轉成一個逗號分隔的字串
            sorted_str = ','.join(sorted_colors)

            # 建立 ROS String 訊息並放入資料
            msg = String()
            msg.data = sorted_str

            # 發布這個訊息到 /color_sort
            self.color_pub.publish(msg)

            # 印出訊息，方便我們除錯或觀察
            self.get_logger().info(f'Published colors: {msg.data}')

        # 顯示畫面，讓我們看到偵測結果
        cv2.imshow("Color Detection", frame)
        cv2.waitKey(1)  # 每張畫面顯示一下再換下一張

# 程式的進入點（主函式）
def main(args=None):
    rclpy.init(args=args)  # 初始化 ROS2
    node = ColorObjectTracker()  # 建立我們的節點物件
    try:
        rclpy.spin(node)  # 進入 ROS 的無限迴圈，等待影像訊息進來
    except KeyboardInterrupt:
        pass  # 如果按 Ctrl+C 結束，就直接跳出
    node.destroy_node()  # 關閉節點
    rclpy.shutdown()  # 關閉 ROS
    cv2.destroyAllWindows()  # 關掉 OpenCV 所有視窗

# 如果這個檔案是主程式，就執行 main()
if __name__ == '__main__':
    main()

