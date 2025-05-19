cmake_minimum_required(VERSION 3.8)
project(small_arm_moveit_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# === Find dependencies ===
find_package(ament_cmake REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)

# === Executables ===
# add_executable(joint_goal src/joint_goal.cpp)
# ament_target_dependencies(joint_goal moveit_ros_planning_interface rclcpp)

add_executable(named_goal src/named_goal.cpp)
ament_target_dependencies(named_goal moveit_ros_planning_interface rclcpp)

add_executable(ee_pose_in_base src/ee_pose_in_base.cpp)
ament_target_dependencies(ee_pose_in_base moveit_ros_planning_interface rclcpp)

add_executable(eye_hand_pose src/eye_hand_pose.cpp)
ament_target_dependencies(eye_hand_pose moveit_ros_planning_interface rclcpp)

add_executable(gripper_joint_value src/gripper_joint_value.cpp)
ament_target_dependencies(gripper_joint_value moveit_ros_planning_interface rclcpp)


add_executable(auto_pose_goal src/auto_pose_goal.cpp)
ament_target_dependencies(auto_pose_goal moveit_ros_planning_interface rclcpp geometry_msgs std_msgs)

# === Install targets ===
install(TARGETS
  # joint_goal
  named_goal
  ee_pose_in_base
  eye_hand_pose
  auto_pose_goal
  gripper_joint_value

  DESTINATION lib/${PROJECT_NAME}
)

# === Install folders (launch/config) ===
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})
install(DIRECTORY config DESTINATION share/${PROJECT_NAME})

# === Lint & Testing ===
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
