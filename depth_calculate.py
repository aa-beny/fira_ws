import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from geometry_msgs.msg import Pose
from std_msgs.msg import String
from cv_bridge import CvBridge
import numpy as np
import cv2

class DepthToWorldNode(Node):
    def __init__(self):
        super().__init__('depth_to_world_node')
        self.last_pose = None
        self.bridge = CvBridge()

        # # RealSense camera intrinsics (假設解析度1280x720)
        # self.fx = 638.1703
        # self.fy = 638.1703
        # self.cx = 638.6412
        # self.cy = 355.8140
        # RealSense camera intrinsics (1280x720) from Depth camera
        self.fx = 640.9104
        self.fy = 640.9104
        self.cx = 644.4667
        self.cy = 353.9984


        self.latest_depth_image = None  # 保存最近一張 depth 圖

        self.depth_sub = self.create_subscription(Image, '/camera/depth', self.depth_callback, 10)
        self.text_info_sub = self.create_subscription(String, '/ocr_results', self.text_info_callback, 10)
        self.publisher = self.create_publisher(Pose, '/text_coordinate', 10)

    def depth_callback(self, msg):
        self.latest_depth_image = msg  # 只保存，不處理

    def text_info_callback(self, msg):
        if not msg.data:
            self.get_logger().warn('Received empty OCR result, skipping.')
            return

        if self.latest_depth_image is None:
            self.get_logger().warn('No depth image available yet, skipping.')
            return

        detected_texts = [item.split(',') for item in msg.data.split(';')]
        depth_image = self.bridge.imgmsg_to_cv2(self.latest_depth_image, desired_encoding='passthrough')

        world_coordinates = []
        for text, u, v in detected_texts:
            u, v = int(u), int(v)
            depth = self.get_valid_depth(depth_image, u, v)
            if depth is None:
                self.get_logger().warn(f"⚠️ No valid depth around ({u},{v}), skipping this text.")
                continue
            x, y, z = self.pixel_to_world(u, v, depth)
            self.get_logger().info(f"[{text}] depth={depth:.3f} @ ({u},{v}) → ({x:.3f}, {y:.3f}, {z:.3f})")
            world_coordinates.append((text, x, y, z))
        
        if world_coordinates:
            first_text, x, y, z = world_coordinates[0]

            # pose 跳動過濾
            if self.last_pose is not None:
                dx = abs(x - self.last_pose.position.x)
                dy = abs(y - self.last_pose.position.y)
                dz = abs(z - self.last_pose.position.z)
                if dx > 0.5 or dy > 0.5 or dz > 0.5:
                    self.get_logger().warn("⚠️ Pose jump detected, skipping.")
                    return

            pose_msg = Pose()
            pose_msg.position.x = float(x)
            pose_msg.position.y = float(y)
            pose_msg.position.z = float(z)
            self.publisher.publish(pose_msg)
            self.get_logger().info(f'Published World Coordinate for "{first_text}": ({x:.3f}, {y:.3f}, {z:.3f})')

            # 記住上一次
            self.last_pose = pose_msg

        else:
            self.get_logger().warn('World coordinate calculation failed, skipping.')


        # if world_coordinates:
        #     first_text, x, y, z = world_coordinates[0]
        #     pose_msg = Pose()
        #     pose_msg.position.x = float(x)
        #     pose_msg.position.y = float(y)
        #     pose_msg.position.z = float(z)
        #     self.publisher.publish(pose_msg)ㄠ
        #     self.get_logger().info(f'Published World Coordinate for "{first_text}": ({x:.3f}, {y:.3f}, {z:.3f})')
        # else:
        #     self.get_logger().warn('World coordinate calculation failed, skipping.')

    def get_valid_depth(self, depth_image, u, v, search_radius=3):
        """在(u,v)附近搜尋合法的depth"""
        h, w = depth_image.shape
        if not (0 <= u < w and 0 <= v < h):
            return None

        depth = depth_image[v, u] / 1000.0
        self.get_logger().info(f"Depth at pixel ({u}, {v}) = {depth:.3f}")

        if depth > 0 and not np.isnan(depth) and not np.isinf(depth):
            return depth

        # 搜附近
        for r in range(1, search_radius + 1):
            for du in [-r, 0, r]:
                for dv in [-r, 0, r]:
                    nu, nv = u + du, v + dv
                    if 0 <= nu < w and 0 <= nv < h:
                        d = depth_image[nv, nu] / 1000.0
                        if d > 0 and not np.isnan(d) and not np.isinf(d):
                            return d
        return None

    def pixel_to_world(self, u, v, depth):
        X = (u - self.cx) * depth / self.fx
        Y = -((v - self.cy) * depth / self.fy)
        Z = depth
        
        return (X, Y, Z)

def main(args=None):
    rclpy.init(args=args)
    node = DepthToWorldNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
