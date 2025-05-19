import pyrealsense2 as rs
import numpy as np
import cv2

# 初始化 RealSense
pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
pipeline.start(config)

# 設置深度相機的內部參數
profile = pipeline.get_active_profile()
depth_sensor = profile.get_device().first_depth_sensor()
depth_scale = depth_sensor.get_depth_scale()  # 轉換為真實距離 (m)
intrinsics = profile.get_stream(rs.stream.depth).as_video_stream_profile().get_intrinsics()

# 透過 RealSense 轉換像素座標到 3D 空間
def pixel_to_3d(x, y, depth_frame):
    depth = depth_frame.get_distance(x, y)  # 取得該點的深度值（米）
    if depth == 0:
        return None  # 無法獲取深度

    # 轉換為 3D 空間座標
    point = rs.rs2_deproject_pixel_to_point(intrinsics, [x, y], depth)
    return point  # [X, Y, Z]（米）

try:
    while True:
        frames = pipeline.wait_for_frames()
        depth_frame = frames.get_depth_frame()
        color_frame = frames.get_color_frame()
        if not depth_frame or not color_frame:
            continue

        # 轉換為 NumPy 陣列
        depth_image = np.asanyarray(depth_frame.get_data())
        color_image = np.asanyarray(color_frame.get_data())

        # 轉換為灰階影像
        gray = cv2.cvtColor(color_image, cv2.COLOR_BGR2GRAY)

        # 使用 Canny 邊緣檢測
        edges = cv2.Canny(gray, 100, 200)

        # 找到輪廓
        contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        # 繪製輪廓和計算座標
        for cnt in contours:
            # 計算木塊的外接矩形
            x, y, w, h = cv2.boundingRect(cnt)

            # 計算木塊的中心點
            center_x, center_y = x + w // 2, y + h // 2

            # 轉換為 3D 坐標
            block_3d = pixel_to_3d(center_x, center_y, depth_frame)
            if block_3d:
                x3d, y3d, z3d = block_3d

                # 在影像上繪製邊框和座標
                cv2.rectangle(color_image, (x, y), (x + w, y + h), (0, 255, 0), 2)
                cv2.circle(color_image, (center_x, center_y), 5, (0, 0, 255), -1)
                cv2.putText(color_image, f"({x3d:.3f}, {y3d:.3f}, {z3d:.3f})m",
                            (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 255), 2)

        # 顯示影像
        cv2.imshow("Detected Blocks", color_image)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

finally:
    pipeline.stop()
    cv2.destroyAllWindows()