#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <moveit/move_group_interface/move_group_interface.h>

class GripperControlNode : public rclcpp::Node {
public: //create node
  GripperControlNode()
  : Node("gripper_control_node")
  {
    // create a group control "gripper"
    move_group_interface_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
      shared_from_this(), "gripper");

    // sub create
    gripper_cmd_sub_ = this->create_subscription<std_msgs::msg::Bool>("/gripper_command", 10,std::bind(&GripperControlNode::gripperCallback, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "Gripper node on, Wait");
  }

private:
  void gripperCallback(const std_msgs::msg::Bool::SharedPtr msg)
  {
    double target_value = msg->data ? 0.0 : 0.8;  // true=關閉, false=打開
    std::string action = msg->data ? "close" : "open";

    RCLCPP_INFO(this->get_logger(), "receive the command", action.c_str(), target_value);

    //setting target value
    move_group_interface_->setJointValueTarget("robotiq_85_left_knuckle_joint", target_value);

    //try to planning
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = (move_group_interface_->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

    if (success) {
      move_group_interface_->execute(plan);  // 執行動作
      RCLCPP_INFO(this->get_logger(), "Gripper Finish");
    } else {
      RCLCPP_ERROR(this->get_logger(), "Gripper Fail");
    }
  }

  std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr gripper_cmd_sub_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GripperControlNode>());
  rclcpp::shutdown();
  return 0;
}
