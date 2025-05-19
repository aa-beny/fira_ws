import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from moveit_configs_utils import MoveItConfigsBuilder

def generate_launch_description():

    # build moveit
    moveit_config = (
        MoveItConfigsBuilder(
            robot_name="small_arm", package_name="small_arm_moveit_config"
        )
        .robot_description(file_path="config/small_arm.urdf.xacro")
        .robot_description_semantic(file_path="config/small_arm.srdf")
        .trajectory_execution(file_path="config/moveit_controllers.yaml")
        .moveit_cpp(
            file_path=os.path.join(
                get_package_share_directory("small_arm_moveit_cpp"),
                "config/moveit_cpp.yaml"
            )
        )
        .to_moveit_configs()
    )

    # 宣告 Launch 參數 cpp_node，只宣告一次！可從命令列傳入
    declare_cpp_node = DeclareLaunchArgument(
        "cpp_node",
        default_value="auto_pose_goal",  # 預設值
        # default_value="ee_pose_in_base",
        description="C++ node executable name to launch",
    )

    # 根據 launch 參數執行對應的 Node
    moveit_cpp_node = Node(
        name="moveit_cpp",
        package="small_arm_moveit_cpp",
        executable=LaunchConfiguration("cpp_node"),
        output="both",
        parameters=[moveit_config.to_dict()],
    )

    return LaunchDescription([
        declare_cpp_node,        # 把參數註冊進 launch
        moveit_cpp_node,         # 執行節點
    ])
