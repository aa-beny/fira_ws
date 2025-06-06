// #include <memory>

// #include <rclcpp/rclcpp.hpp>
// #include <moveit/move_group_interface/move_group_interface.h>

// int main(int argc, char * argv[])
// {
//   // Initialize ROS and create the Node
//   rclcpp::init(argc, argv);
//   auto const node = std::make_shared<rclcpp::Node>("joint_goal");

//   // Create a ROS logger
//   auto const logger = rclcpp::get_logger("joint_goal");

//   // We spin up a SingleThreadedExecutor so we can get current joint values later
//   rclcpp::executors::SingleThreadedExecutor executor;
//   executor.add_node(node);
//   auto spinner = std::thread([&executor]() { executor.spin(); });

//   // Create the MoveIt Move Group Interface for panda arm
//   using moveit::planning_interface::MoveGroupInterface;
//   auto move_group_interface = MoveGroupInterface(node, "small_arm");

//   // Get all joint positions
//   std::vector<double> joint_group_positions = move_group_interface.getCurrentJointValues();

//   // Sets the first joint value
//   joint_group_positions[1] = -1.57;  // radians
//   move_group_interface.setJointValueTarget(joint_group_positions);

//   // Create a plan to these joint values and check if that plan is successful
//   moveit::planning_interface::MoveGroupInterface::Plan my_plan;
//   bool success = (move_group_interface.plan(my_plan) == moveit::core::MoveItErrorCode::SUCCESS);

//   // If the plan is successful, execute the plan
//   if(success) {
//     move_group_interface.execute(my_plan);
//   } else {
//     RCLCPP_ERROR(logger, "Planing failed!");
//   }

//   // Shutdown
//   rclcpp::shutdown();
//   spinner.join();
//   return 0;
// }
