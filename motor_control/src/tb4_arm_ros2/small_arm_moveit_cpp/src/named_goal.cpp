#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <moveit/move_group_interface/move_group_interface.h>

class NamedGoalControl : public rclcpp::Node {
public:
  NamedGoalControl() : Node("named_goal_controller") {
    move_group_interface_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
      shared_from_this(), "small_arm");

    goal_subscriber_ = this->create_subscription<std_msgs::msg::String>(
      "named_goal_target", 10,
      std::bind(&NamedGoalControl::goal_callback, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "NamedGoalControl ready: Waiting for command");
  }

private:
  void goal_callback(const std_msgs::msg::String::SharedPtr msg) {
    std::string target = msg->data;
    RCLCPP_INFO(this->get_logger(), "Received target: %s", target.c_str());

    move_group_interface_->setNamedTarget(target);

    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = (move_group_interface_->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

    if (success) {
      RCLCPP_INFO(this->get_logger(), "Planning succeeded for target: %s", target.c_str());
      move_group_interface_->execute(plan);
    } else {
      RCLCPP_ERROR(this->get_logger(), "Planning failed for target: %s", target.c_str());
    }
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr goal_subscriber_;
  std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<NamedGoalControl>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
