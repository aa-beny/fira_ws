# import rclpy
# from rclpy.node import Node
# from sensor_msgs.msg import Image
# from geometry_msgs.msg import Pose
# from std_msgs.msg import String
# from cv_bridge import CvBridge
# import numpy as np
# import cv2

# class DepthToWorldNode(Node):
#     def __init__(self):
#         super().__init__('depth_to_world_node')
#         self.last_pose = None
#         self.bridge = CvBridge()

#         # # RealSense camera intrinsics (假設解析度1280x720)
#         # self.fx = 638.1703
#         # self.fy = 638.1703
#         # self.cx = 638.6412
#         # self.cy = 355.8140
#         # RealSense camera intrinsics (1280x720) from Depth camera
#         self.fx = 640.9104
#         self.fy = 640.9104
#         self.cx = 644.4667
#         self.cy = 353.9984


#         self.latest_depth_image = None  # 保存最近一張 depth 圖

#         self.depth_sub = self.create_subscription(Image, '/camera/depth', self.depth_callback, 10)
#         self.text_info_sub = self.create_subscription(String, '/ocr_results', self.text_info_callback, 10)
#         self.publisher = self.create_publisher(Pose, '/text_coordinate', 10)

#     def depth_callback(self, msg):
#         self.latest_depth_image = msg  # 只保存，不處理

#     def text_info_callback(self, msg):
#         if not msg.data:
#             self.get_logger().warn('Received empty OCR result, skipping.')
#             return

#         if self.latest_depth_image is None:
#             self.get_logger().warn('No depth image available yet, skipping.')
#             return

#         detected_texts = [item.split(',') for item in msg.data.split(';')]
#         depth_image = self.bridge.imgmsg_to_cv2(self.latest_depth_image, desired_encoding='passthrough')

#         world_coordinates = []
#         for text, u, v in detected_texts:
#             u, v = int(u), int(v)
#             depth = self.get_valid_depth(depth_image, u, v)
#             if depth is None:
#                 self.get_logger().warn(f"⚠️ No valid depth around ({u},{v}), skipping this text.")
#                 continue
#             x, y, z = self.pixel_to_world(u, v, depth)
#             self.get_logger().info(f"[{text}] depth={depth:.3f} @ ({u},{v}) → ({x:.3f}, {y:.3f}, {z:.3f})")
#             world_coordinates.append((text, x, y, z))
        
#         if world_coordinates:
#             first_text, x, y, z = world_coordinates[0]

#             # pose 跳動過濾
#             if self.last_pose is not None:
#                 dx = abs(x - self.last_pose.position.x)
#                 dy = abs(y - self.last_pose.position.y)
#                 dz = abs(z - self.last_pose.position.z)
#                 if dx > 0.5 or dy > 0.5 or dz > 0.5:
#                     self.get_logger().warn("⚠️ Pose jump detected, skipping.")
#                     return

#             pose_msg = Pose()
#             pose_msg.position.x = float(x)
#             pose_msg.position.y = float(y)
#             pose_msg.position.z = float(z)
#             self.publisher.publish(pose_msg)
#             self.get_logger().info(f'Published World Coordinate for "{first_text}": ({x:.3f}, {y:.3f}, {z:.3f})')

#             # 記住上一次
#             self.last_pose = pose_msg

#         else:
#             self.get_logger().warn('World coordinate calculation failed, skipping.')


#         # if world_coordinates:
#         #     first_text, x, y, z = world_coordinates[0]
#         #     pose_msg = Pose()
#         #     pose_msg.position.x = float(x)
#         #     pose_msg.position.y = float(y)
#         #     pose_msg.position.z = float(z)
#         #     self.publisher.publish(pose_msg)ㄠ
#         #     self.get_logger().info(f'Published World Coordinate for "{first_text}": ({x:.3f}, {y:.3f}, {z:.3f})')
#         # else:
#         #     self.get_logger().warn('World coordinate calculation failed, skipping.')

#     def get_valid_depth(self, depth_image, u, v, search_radius=3):
#         """在(u,v)附近搜尋合法的depth"""
#         h, w = depth_image.shape
#         if not (0 <= u < w and 0 <= v < h):
#             return None

#         depth = depth_image[v, u] / 1000.0
#         self.get_logger().info(f"Depth at pixel ({u}, {v}) = {depth:.3f}")

#         if depth > 0 and not np.isnan(depth) and not np.isinf(depth):
#             return depth

#         # 搜附近
#         for r in range(1, search_radius + 1):
#             for du in [-r, 0, r]:
#                 for dv in [-r, 0, r]:
#                     nu, nv = u + du, v + dv
#                     if 0 <= nu < w and 0 <= nv < h:
#                         d = depth_image[nv, nu] / 1000.0
#                         if d > 0 and not np.isnan(d) and not np.isinf(d):
#                             return d
#         return None

#     def pixel_to_world(self, u, v, depth):
#         X = (u - self.cx) * depth / self.fx
#         Y = -((v - self.cy) * depth / self.fy)
#         Z = depth
        
#         return (X, Y, Z)

# def main(args=None):
#     rclpy.init(args=args)
#     node = DepthToWorldNode()
#     rclpy.spin(node)
#     node.destroy_node()
#     rclpy.shutdown()

# if __name__ == '__main__':
#     main()



import rclpy  # ROS 2 的 Python 函式庫
from rclpy.node import Node  # 建立 ROS 節點的基礎類別
from sensor_msgs.msg import Image  # 用來接收 ROS 中的影像資料
from geometry_msgs.msg import Pose  # 發布 3D 座標（位置）
from std_msgs.msg import String  # 接收 OCR 偵測到的文字與位置
from std_msgs.msg import Bool 
from cv_bridge import CvBridge  # 將 ROS Image 轉換為 OpenCV 圖片
import numpy as np  # 做數學運算與陣列處理
import cv2  # OpenCV 圖像處理函式庫
from builtin_interfaces.msg import Time
from std_msgs.msg import Float32
print("🔥 正在執行新版 depth_calculate.py")
class RGBPixelTo3D(Node):
    def __init__(self):
        super().__init__('rgb_pixel_to_3d_node')  # 初始化 ROS 節點
        self.text_sub = self.create_subscription(String, '/ocr_results', self.text_callback, 10)  # 訂閱 OCR 偵測結果
        # self.text_sub = self.create_subscription(String, '/target_ocr', self.text_callback, 10)  # 訂閱 OCR 偵測結果
        self.depth_sub = self.create_subscription(Image, '/camera/depth', self.depth_callback, 10)  # 訂閱深度圖像
        self.pose_pub = self.create_publisher(Pose, '/text_coordinate', 10)  # 發布 3D 位置給手臂用

        self.z_level_pub = self.create_publisher(Float32, '/target_z_height', 10)

        self.bridge = CvBridge()  # 建立 OpenCV 與 ROS Image 的轉換器
        self.allow_pub = False  # 初始為不允許
        self.allow_pub_sub = self.create_subscription(Bool, '/allow_publish_pose', self.allow_pub_callback, 10)
        # 相機的內參數（RealSense 深度相機在 1280x720 解析度下的數值）
        self.fx = 640.9104
        self.fy = 640.9104
        self.cx = 644.4667
        self.cy = 353.9984
        self.pose_buffer = []  # 儲存多個連續 Pose 資料，用來判斷穩定性再發布

        # 初始化變數
        self.detected_texts = []  # 存放 OCR 偵測到的文字與像素位置 (u,v)
        self.plane_params = None  # 擬合出來的平面參數
        self.avg_z = None         # ROI 區域的平均深度
        self.target_z = None      # 根據平均深度決定的抓取高度

        self.last_3d_pose = None
        # self.sent_pose = False
        self.similar_count = 0  # 紀錄連續相同目標的次數
        # 建立視窗顯示擬合結果
        cv2.namedWindow("Fitted Plane View", cv2.WINDOW_NORMAL)
        # init 時加：
        self.last_text_time = self.get_clock().now()
        self.keep_text_seconds = 0.3  # OCR 有效保留 0.3 秒

        self.waiting_count = 0  # 追蹤 pose 連續跳動的次數
        self.max_waiting_limit = 8  # 超過幾次就強制清空
        
    def allow_pub_callback(self, msg):
        self.allow_pub = msg.data
        self.get_logger().info(f"🔄 Pose publishing allowed: {self.allow_pub}")

    def text_callback(self, msg):

        self.last_text_time = self.get_clock().now()

        if not msg.data.strip():
            self.detected_texts = []
            self.get_logger().info("No OCR data received — cleared detected_texts.")
            return
        



        self.detected_texts.clear()  # 清空前一幀資料
        entries = [item.split(',') for item in msg.data.split(';') if len(item.split(',')) == 3]  # 分割每一筆資料
        for text, u_str, v_str in entries:
            try:
                u, v = int(float(u_str)), int(float(v_str))  # 將像素座標轉為整數
                self.detected_texts.append((text, u, v))  # 加入清單
            except ValueError:
                continue  # 若轉換失敗就略過

    def ransac_plane_fit(self, points, threshold=0.01, iterations=100):
        best_eq = None
        best_inliers = []
        for _ in range(iterations):
            sample = points[np.random.choice(points.shape[0], 3, replace=False)]
            p1, p2, p3 = sample
            normal = np.cross(p2 - p1, p3 - p1)  # 外積求平面法向量
            if np.linalg.norm(normal) == 0:
                continue
            normal = normal / np.linalg.norm(normal)
            d = -np.dot(normal, p1)
            distances = np.abs(np.dot(points, normal) + d)  # 所有點到平面的距離
            inliers = np.where(distances < threshold)[0]
            if len(inliers) > len(best_inliers):
                best_inliers = inliers
                best_eq = (normal, d, best_inliers)
        return best_eq
    def get_median_depth(self, depth_image, u, v, radius=2):
        h, w = depth_image.shape
        depths = []
        for du in range(-radius, radius+1):
            for dv in range(-radius, radius+1):
                uu = u + du
                vv = v + dv
                if 0 <= uu < w and 0 <= vv < h:
                    d = depth_image[vv, uu] / 1000.0
                    if 0.01 < d < 5.0:  # 忽略異常值
                        depths.append(d)
        if len(depths) == 0:
            return None
        return float(np.median(depths))

    def depth_callback(self, msg):
        now = self.get_clock().now()
        # if len(self.detected_texts) == 0:
        if len(self.detected_texts) == 0 or (now - self.last_text_time).nanoseconds * 1e-9 > self.keep_text_seconds:
            self.get_logger().info("No OCR data received.")
            # self.sent_pose = False  # 可以接受新目標了
            self.similar_count = 0
            return  # 沒字 → 不處理整個 depth callback
        



        depth_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='passthrough')  # ROS Image → OpenCV 圖片
        h, w = depth_image.shape
        # 看 A 字附近的地板有多遠 我從影像正中間開始，畫一個 400 像素寬的正方形區域」，
        box_size = 400  # ROI 區域大小（正方形區塊）
        u_start = w // 2 - box_size // 2
        v_start = h // 2 - box_size // 2


        
        points = []
        uvs = []
        region_depths = []

        # 從 ROI 區塊中每隔 2 像素取一點進行擬合 我從這個區域裡，每隔 2 個 pixel 拿一點然後看這一點的深度值 z = depth_image[v, u] 是多少（也就是離相機的距離）
        for v in range(v_start, v_start + box_size, 2):
            for u in range(u_start, u_start + box_size, 2):
                z = depth_image[v, u] * 0.001  # mm → m
                if z == 0 or z > 2.0:
                    continue  # 無效或太遠的點跳過
                x = (u - self.cx) * z / self.fx
                y = (v - self.cy) * z / self.fy
                points.append([x, y, z])
                uvs.append((u, v))
                region_depths.append(z)  #把這堆點的距離記下來

        if len(points) < 10:
            self.get_logger().warn("Not enough points for plane fitting.")
            return



        # ✅ 用所有點取「空間中位數」作為穩定目標點
        median_point = np.median(points, axis=0)
        median_x, median_y, median_z = median_point

        self.get_logger().info(f"✅ Median point → x={median_x:.3f}, y={median_y:.3f}, z={median_z:.3f}")



        # self.avg_z = float(np.mean(region_depths)) + 0.025  # 算這一大堆 z 值的平均 計算平均深度並微調「估計」出這整片區域的平均深度 = avg_z


        current_z = np.mean(region_depths)
        # current_z = np.median(region_depths)
        

        # === 中位數過濾機制 ===＝＝＝＝＝＝＝＝＝＝＝
        if not hasattr(self, "z_buffer"):
            self.z_buffer = []

        self.z_buffer.append(current_z)
        if len(self.z_buffer) > 10:
            self.z_buffer.pop(0)

        # 使用中位數代替平均值，抗雜訊效果好
        self.avg_z = float(np.median(self.z_buffer)) + 0.025
        # === 中位數過濾機制 ===＝＝＝＝＝＝＝＝＝＝＝＝＝＝

        self.get_logger().info(f"Average depth (avg_z): {self.avg_z:.3f} m")
        if self.avg_z < 0.28 or self.avg_z > 0.75:
            self.get_logger().warn(f"⚠️ avg_z={self.avg_z:.3f} out of range — skipping")
            return
        # 根據深度分類高度
        # if 0.33 < self.avg_z < 0.38:
        #     self.target_z = 0.01  
        #     self.get_logger().info("Height: LOW")
        # elif 0.29 < self.avg_z < 0.33:
        #     # self.target_z = 0.06  # 中等高度
        #     self.target_z = 0.058  
        #     self.get_logger().info("Height: MID")
        # else:
        #     self.target_z = 0.105  
        #     self.get_logger().info("Height: HIGH")

        # if 0.31 < self.avg_z < 0.38: # 0.338 0.339 0.319
        #     self.target_z = 0.01  
        #     self.get_logger().info("Height: LOW")
        # elif 0.28 < self.avg_z < 0.31:
        #     # self.target_z = 0.06  # 中等高度0.297  0.368 0.367  0.38 0.336
        #     self.target_z = 0.058  
        #     self.get_logger().info("Height: MID")
        # else:
        #     self.target_z = 0.105   # 中0.553
        #     self.get_logger().info("Height: HIGH")

        if 0.345 < self.avg_z < 0.38: # 9 0.354 0.36
            self.target_z = 0.01  
            self.get_logger().info("Height: LOW")
        elif 0.315 < self.avg_z < 0.345:
            # self.target_z = 0.06  # 中等高度0.313  0.32
            self.target_z = 0.058  
            self.get_logger().info("Height: MID")
        else:
            self.target_z = 0.105   # 0.3
            self.get_logger().info("Height: HIGH")

        # if self.avg_z < 0.29:
        #     self.target_z = 0.105  # HIGH
        #     self.get_logger().info("Height: HIGH")
        # elif self.avg_z < 0.37:
        #     self.target_z = 0.058  # MID
        #     self.get_logger().info("Height: MID")
        # else:
        #     self.target_z = 0.01   # LOW
        #     self.get_logger().info("Height: LOW")
        

        result = self.ransac_plane_fit(np.array(points))
        if result is None:
            self.get_logger().warn("RANSAC plane fitting failed.")
            return

        normal, d, inlier_idx = result
        a, b, c = normal
        self.plane_params = (a, b, c, d)  # 儲存平面參數

        # 畫出 ROI 區域與平面內點（紅色點）
        vis = cv2.normalize(depth_image, None, 0, 255, cv2.NORM_MINMAX)
        vis = cv2.cvtColor(vis.astype(np.uint8), cv2.COLOR_GRAY2BGR)
        for idx in inlier_idx:
            u, v = uvs[idx]
            cv2.circle(vis, (u, v), 1, (0, 0, 255), -1)
        cv2.rectangle(vis, (u_start, v_start), (u_start + box_size, v_start + box_size), (255, 255, 0), 1)
        cv2.putText(vis, f"Avg Z: {self.avg_z:.3f}m", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
        cv2.imshow("Fitted Plane View", vis)
        cv2.waitKey(10)
        # pose.position.x = median_x
        # pose.position.y = median_y
        # 將每個 OCR 偵測到的文字投影成 3D 座標並發送
        for text, u, v in self.detected_texts:
            # x = (u - self.cx) / self.fx
            # y = -(v - self.cy) / self.fy
            # z = self.avg_z
            # pose = Pose()


            # pose.position.x = x * z
            # pose.position.y = y * z

            # # depth = self.get_valid_depth(depth_image, u, v)
            # depth = self.get_median_depth(depth_image, u, v)
            # if depth is None:
            #     self.get_logger().warn(f"⚠️ No valid depth found around ({u},{v}), skipping")
            #     continue
            # x = (u - self.cx) * depth / self.fx
            # y = -(v - self.cy) * depth / self.fy
            # z = depth  # ✅ 使用 pixel 的真實 z 值


            # depth = self.avg_z
            depth = self.target_z
            

            x = (u - self.cx) * depth / self.fx
            y = -(v - self.cy) * depth / self.fy
            z = depth  # or z = self.target_z，也行
            pose = Pose()
            pose.position.x = x
            pose.position.y = y
            pose.position.z = z

            # ✅ 發送對應高度（target_z）
            z_msg = Float32()
            z_msg.data = self.target_z
            self.z_level_pub.publish(z_msg)
            self.get_logger().info(f"📤 Published target_z: {self.target_z:.3f}")

            # pose.position.z = self.target_z
            # pose.position.z = depth
            if self.allow_pub:
                # 有 last_3d_pose 時才做比較
                if self.last_3d_pose:
                    dx = abs(pose.position.x - self.last_3d_pose.position.x)
                    dy = abs(pose.position.y - self.last_3d_pose.position.y)
                    dz = abs(pose.position.z - self.last_3d_pose.position.z)
                    if dx < 0.04 and dy < 0.04 and dz < 0.04:
                        self.pose_buffer.append((pose.position.x, pose.position.y, pose.position.z))
                        self.last_3d_pose = pose  # ✅ 只有成功加入 buffer 才更新

                        self.waiting_count = 0  # ✅ 成功就清空計數
                    else:
                        self.get_logger().warn(f"❌ Pose jump too large — dx={dx:.3f}, dy={dy:.3f}, dz={dz:.3f}")
                        self.waiting_count += 1
                        if self.waiting_count > self.max_waiting_limit:
                            self.get_logger().warn("⚠️ Waiting too long with unstable poses — reset last_3d_pose.")
                            self.last_3d_pose = None
                            self.waiting_count = 0  # ✅ 清空讓下次重新開始
                else:
                    self.pose_buffer.append((pose.position.x, pose.position.y, pose.position.z))
                    self.last_3d_pose = pose  # ✅ 第一次直接設定
                    
                    self.waiting_count = 0  # 第一次就直接重設

                # 發送條件
                if len(self.pose_buffer) >= 10:
                    xs, ys, zs = zip(*self.pose_buffer)
                    # pose.position.x = np.mean(xs)
                    # pose.position.y = np.mean(ys)
                    # pose.position.z = np.mean(zs)
                    pose.position.x = float(np.median(xs))
                    pose.position.y = float(np.median(ys))
                    pose.position.z = float(np.median(zs))

                    self.pose_pub.publish(pose)
                    self.get_logger().info(
                        f"🎯 Published pose: X={pose.position.x:.3f}, Y={pose.position.y:.3f}, Z={pose.position.z:.3f}"
                    )
                                        
                                        


                    
                    self.pose_buffer.clear()
                    self.allow_pub = False
                    self.last_3d_pose = None  # ✅ 重置讓下一輪重新開始
                elif len(self.pose_buffer) > 15:
                    self.get_logger().warn("⚠️ Still unstable — buffer cleared.")
                    self.pose_buffer.clear()
                    self.allow_pub = True  # 看你要不要自動 retry
                    self.last_3d_pose = None  # ✅ 清掉參考點，避免後續比錯


def main():
    rclpy.init()
    node = RGBPixelTo3D()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()