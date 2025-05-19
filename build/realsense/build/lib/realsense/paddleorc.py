# import rclpy
# from rclpy.node import Node
# from sensor_msgs.msg import Image
# from std_msgs.msg import String
# from cv_bridge import CvBridge
# import cv2
# import numpy as np
# import paddle
# from paddleocr import PaddleOCR

# # 强制使用 GPU
# paddle.set_device("gpu")
# paddle.set_flags({"FLAGS_cudnn_deterministic": True})

# # GPU 预热（触发 CUDA 初始化，避免 cuBLAS 出错）
# _ = paddle.matmul(paddle.to_tensor([[1.0, 2.0], [3.0, 4.0]]), paddle.to_tensor([[1.0], [1.0]]))

# # 允许识别的字母
# TARGET_LETTERS = {"F", "I", "R", "A"}

# class OCRNode(Node):
#     def __init__(self):
#         super().__init__('ocr_node')
#         self.bridge = CvBridge()
#         self.subscription = self.create_subscription(
#             Image, '/camera/color', self.image_callback, 10)
#         self.publisher = self.create_publisher(String, '/ocr_results', 10)
#         self.get_logger().info('OCR Node Initialized')
#         self.ocr = None  # OCR 初始化延后
    
#     def image_callback(self, msg):
#         try:
#             # 延迟 OCR 初始化，确保 CUDA 初始化完成
#             if self.ocr is None:
#                 self.ocr = PaddleOCR(use_angle_cls=True, lang="en", det_db_box_thresh=0.3, rec_algorithm='CRNN')

#             # 转换 ROS Image 到 OpenCV 格式
#             cv_image = self.bridge.imgmsg_to_cv2(msg, "bgr8")
#             rgb_image = cv2.cvtColor(cv_image, cv2.COLOR_BGR2RGB)

#             # OCR 识别
#             results = self.ocr.ocr(rgb_image, cls=True)
#             detected_objects = []

#             if results and isinstance(results, list) and results[0] is not None:
#                 for result in results:
#                     for line in result:
#                         try:
#                             points, (text, confidence) = line[0], line[1]
#                             text = text.strip().upper()
#                             if text in TARGET_LETTERS and confidence > 0.5:
#                                 x_center = int(sum([p[0] for p in points]) / 4)
#                                 y_center = int(sum([p[1] for p in points]) / 4)
#                                 detected_objects.append(f"{text},{x_center},{y_center}")
#                                 points_np = np.array(points, np.int32).reshape((-1, 1, 2))
#                                 cv2.polylines(cv_image, [points_np], isClosed=True, color=(0, 255, 0), thickness=2)
#                                 cv2.putText(cv_image, f"{text} ({confidence:.2f})", (x_center, y_center),
#                                             cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2, cv2.LINE_AA)
#                         except Exception as e:
#                             self.get_logger().error(f"解析 OCR 结果出错: {e}")

#             if detected_objects:
#                 output_msg = String()
#                 output_msg.data = ";".join(detected_objects)
#                 self.publisher.publish(output_msg)
#                 self.get_logger().info(f"检测结果: {output_msg.data}")

#             cv2.imshow("OCR Detection", cv_image)
#             cv2.waitKey(1)
        
#         except Exception as e:
#             self.get_logger().error(f"处理图像出错: {e}")


# def main(args=None):
#     rclpy.init(args=args)
#     node = OCRNode()
#     rclpy.spin(node)
#     node.destroy_node()
#     rclpy.shutdown()
#     cv2.destroyAllWindows()

# if __name__ == '__main__':
#     main()


import rclpy  # 匯入 ROS 2 的主程式庫，負責節點初始化與運行
from rclpy.node import Node  # 匯入 ROS 節點類別，用來創建 ROS 節點
from sensor_msgs.msg import Image  # 匯入 ROS 中的影像訊息格式
from std_msgs.msg import String  # 匯入 ROS 中的字串訊息格式，用來傳送 OCR 結果
from cv_bridge import CvBridge  # 匯入橋接工具，負責 ROS Image <-> OpenCV 圖片轉換
import cv2  # 匯入 OpenCV 圖像處理函式庫
import numpy as np  # 匯入 NumPy，做數學與陣列處理
import paddle  # 匯入 PaddlePaddle 深度學習框架
from paddleocr import PaddleOCR  # 匯入 PaddleOCR 模組
import logging  # 匯入 logging 模組，管理訊息輸出等級

logging.getLogger('ppocr').setLevel(logging.ERROR)  # 設定只顯示錯誤等級的訊息，讓畫面更乾淨

paddle.set_device("gpu")  # 設定使用 GPU 執行推論
paddle.set_flags({"FLAGS_cudnn_deterministic": True})  # 固定 CUDA 行為，避免每次結果不同

# 執行一次矩陣乘法，讓 GPU 預熱，避免第一次使用時 cuBLAS 出錯
_ = paddle.matmul(paddle.to_tensor([[1.0, 2.0], [3.0, 4.0]]), paddle.to_tensor([[1.0], [1.0]]))

TARGET_LETTERS = {"F", "I", "R", "A"}  # 只允許辨識這些字母

# 將圖片縮放到目標尺寸，並用黑邊補齊，保持比例不變
def letterbox_resize(image, target_width, target_height, color=(0, 0, 0)):
    h, w = image.shape[:2]  # 取得原始圖片高度與寬度
    scale = min(target_width / w, target_height / h)  # 計算縮放比例
    new_w = int(w * scale)  # 計算縮放後寬度
    new_h = int(h * scale)  # 計算縮放後高度

    resized = cv2.resize(image, (new_w, new_h))  # 縮放圖片

    pad_w = target_width - new_w  # 計算需要補多少寬度
    pad_h = target_height - new_h  # 計算需要補多少高度

    top = pad_h // 2
    bottom = pad_h - top
    left = pad_w // 2
    right = pad_w - left

    # 加黑邊補齊
    padded = cv2.copyMakeBorder(resized, top, bottom, left, right, cv2.BORDER_CONSTANT, value=color)
    return padded

# OCR ROS 節點類別
def main(args=None):
    rclpy.init()  # 初始化 ROS 2 系統

    class OCRNode(Node):
        def __init__(self):
            super().__init__('ocr_node')  # 設定節點名稱
            self.bridge = CvBridge()  # 建立影像轉換器
            self.subscription = self.create_subscription(Image, '/camera/color', self.image_callback, 10)  # 訂閱相機彩色影像
            self.publisher = self.create_publisher(String, '/ocr_results', 10)  # 發布辨識結果
            self.get_logger().info('OCR Node Initialized')  # 印出初始化訊息
            self.ocr = None  # 延遲初始化 OCR 模型
            self.last_result = ""  # 儲存上一次的結果，避免重複傳送相同結果

        def image_callback(self, msg):
            try:
                if self.ocr is None:
                    # 延遲加載 OCR 模型（第一次收到影像時才載入）
                    self.ocr = PaddleOCR(use_angle_cls=True, lang="en", det_db_box_thresh=0.3, rec_algorithm='CRNN')

                # 將 ROS Image 轉為 OpenCV 圖片格式（BGR）
                cv_image = self.bridge.imgmsg_to_cv2(msg, "bgr8")
                rgb_image = cv2.cvtColor(cv_image, cv2.COLOR_BGR2RGB)  # 模型需要 RGB 格式

                results = self.ocr.ocr(rgb_image, cls=True)  # 執行 OCR 模型推論
                detected_objects = []  # 存放有效結果

                if results and isinstance(results, list) and results[0] is not None:
                    # for result in results:
                    #     for line in result:
                    #         points, (text, confidence) = line[0], line[1]  # 解構偵測結果
                    #         text = text.strip().upper()  # 去空白並轉為大寫

                    #         # 過濾條件：只要指定字母且信心值大於 0.5
                    #         if text in TARGET_LETTERS and confidence > 0.5:
                    #             x_center = int(sum([p[0] for p in points]) / 4)
                    #             y_center = int(sum([p[1] for p in points]) / 4)
                    #             detected_objects.append(f"{text},{x_center},{y_center}")

                    #             # 畫出框線與標籤
                    #             points_np = np.array(points, np.int32).reshape((-1, 1, 2))
                    #             cv2.polylines(cv_image, [points_np], isClosed=True, color=(0, 255, 0), thickness=2)
                    #             cv2.putText(cv_image, f"{text} ({confidence:.2f})", (x_center, y_center),
                    #                         cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2, cv2.LINE_AA)
                    for result in results:
                        for line in result:
                            points, (text, confidence) = line[0], line[1]
                            text = text.strip().upper()

                            if text in TARGET_LETTERS and confidence > 0.5:

                                self.get_logger().info(f"🔍 OCR [{text}] box points: {points}")


                                # ✅ 四個角點已經包含整個字的範圍，平均它們會更接近字體實體的中心
                                # x_avg = int(sum([p[0] for p in points]) / 4)
                                # y_avg = int(sum([p[1] for p in points]) / 4)

                                all_x = np.array([p[0] for p in points])
                                all_y = np.array([p[1] for p in points])

                                x_center = int(np.median(all_x))
                                y_center = int(np.median(all_y))
                                self.get_logger().info(f"🎯 Center for [{text}]: u={x_center}, v={y_center}, conf={confidence:.2f}")
                                detected_objects.append(f"{text},{x_center},{y_center}")
                                cv2.putText(cv_image, f"{text} ({confidence:.2f})", (x_center, y_center),
                                            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2, cv2.LINE_AA)
                                self.get_logger().info(f"🧪 [{text}] points={points}")
                                self.get_logger().info(f"📊 {text} → x_avg={int(np.mean(all_x))}, x_median={x_center}, y_avg={int(np.mean(all_y))}, y_median={y_center}")


                                # self.get_logger().info(f"🎯 Center for [{text}]: u={x_avg}, v={y_avg}, conf={confidence:.2f}")

                                # # ✅ 將這個中心點當作抓取對象（而不是某條框線或內部推估）
                                # detected_objects.append(f"{text},{x_avg},{y_avg}")

                                # # 🎨 視覺化：畫出邊框與文字（這部分不變）
                                # points_np = np.array(points, np.int32).reshape((-1, 1, 2))
                                # cv2.polylines(cv_image, [points_np], isClosed=True, color=(0, 255, 0), thickness=2)
                                # cv2.putText(cv_image, f"{text} ({confidence:.2f})", (x_avg, y_avg),
                                #             cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2, cv2.LINE_AA)
                                





                                # all_x = np.array([p[0] for p in points])
                                # all_y = np.array([p[1] for p in points])

                                # x_median = int(np.median(all_x))
                                # y_median = int(np.median(all_y))

                                # # 顯示差異
                                # self.get_logger().info(f"📊 {text} → x_avg={x_avg}, x_median={x_median}, y_avg={y_avg}, y_median={y_median}")


                # 若有新結果且與上次不同，則發布
                if detected_objects:
                    current_result = ";".join(detected_objects)
                    # if current_result != self.last_result:
                    output_msg = String()
                    output_msg.data = current_result
                    self.publisher.publish(output_msg)
                    self.get_logger().info(f"OCR result: {output_msg.data}")
                    self.last_result = current_result

                # 顯示畫面結果
                cv_image_resized = letterbox_resize(cv_image, 1280, 720)
                cv2.namedWindow("OCR Detection", cv2.WINDOW_NORMAL)
                cv2.imshow("OCR Detection", cv_image_resized)
                cv2.waitKey(1)


                if not detected_objects:
                    if self.last_result != "":
                        empty_msg = String()
                        empty_msg.data = ""  # 發送空字串
                        self.publisher.publish(empty_msg)
                        self.get_logger().info("OCR result: (empty)")
                        self.last_result = ""

                        

            except Exception as e:
                self.get_logger().error(f"Image processing error: {e}")

    node = OCRNode()  # 建立節點實例
    rclpy.spin(node)  # 讓節點持續運行（接收資料、回呼函式）
    node.destroy_node()  # 結束後銷毀節點
    rclpy.shutdown()  # 關閉 ROS
    cv2.destroyAllWindows()  # 關閉顯示視窗

if __name__ == '__main__':
    main()