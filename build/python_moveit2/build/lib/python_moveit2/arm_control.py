import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32MultiArray,Int8,String,Float32
import numpy as np
from geometry_msgs.msg import Pose


class ArmControl(Node):
    def __init__(self):
        super().__init__('arm_control_node')  # Initialize the node with a name
        
        #word detect
        self.target_word_pub = self.create_subscription(Float32MultiArray,'/word_target_position',self.word_target_postion_callback,10)
        # self.car_position_sub =self.create_subscription(Int8,'/car_position',self.camera_to_base_callback,10)

        #arm ee position
        self.name_goal_publisher = self.create_publisher(Pose,'/arm/name_goal',10)
        self.pose_goal_publisher = self.create_publisher(String,'/arm/pose_goal',10)
        #gripper control
        self.gripper_position = self.create_publisher(Float32,'arm/gripper_position')
        self.gripper_state_publisher = self.create_publisher(Int8,'/arm/gripper_state',10)
        self.gripper_position_control_pub = self.create_publisher(Float32,'/arm/gripper_position_control')
        #coordinate
        self.camera_to_base_sub = self.create_subscription(Pose,"/arm/camera_base",self.camera_to_base_callback,10 )
        self.ee_coordinate_sub = self.create_subscription(Pose,"ee_coordinate",self.ee_coordinate_callback,10) 

        #arm control
        self.mode_publisher = self.create_publisher(String,'/arm/mode',10)
        self.get_logger().info('ArmControl node has been started.')
        
        #camera to base link transformation
        self.camera_to_base = None
        #mode setting
        self.gripper = None
        self.name_goal = None
        self.pose_goal = None
        self.word_detection = None

        self.status = ["gripper","name_goal","pose_goal","word_detection"]
    
    def word_target_position_callback(self,msg):
        self.word_detection = msg.data
        self.get_logger().warn(self.word_detection)


    def camera_to_base_callback(self, msg: Float32MultiArray):
        self.camera_to_base = np.array(msg.data)
        self.get_logger().info(f'camera_to_base: {self.camera_to_base}')

    def ee_coordinate_callback(self,msg):
        self.ee_cordinate = np.array(msg.data)
        self.get_logger().info(self.ee_cordinate)
        
    # def mode_select(self):



def main(args=None):
    rclpy.init(args=args)
    node = ArmControl()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
