#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <moveit/move_group_interface/move_group_interface.h>

class GripperControlNode : public rclcpp::Node
{
bool is_gripper_busy_ = false;
bool is_drop_mode_ = false;
public:
  GripperControlNode()
  : Node("gripper_control_node")
  {
    gripper_cmd_sub_ = this->create_subscription<std_msgs::msg::Bool>(
      "/gripper_command", 10,
      std::bind(&GripperControlNode::gripperCallback, this, std::placeholders::_1));
    drop_mode_sub_ = this->create_subscription<std_msgs::msg::Bool>(
      "/is_drop_mode", 10,
      [this](const std_msgs::msg::Bool::SharedPtr msg) {
        is_drop_mode_ = msg->data;
        RCLCPP_INFO(this->get_logger(), "📦 Drop Mode set to: %s", is_drop_mode_ ? "true" : "false");
      });
  }

  void init()
  {
    
    // 修正版本：使用 this->shared_from_this()
    move_group_interface_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
      this->shared_from_this(), "gripper");
  }


private:
  void gripperCallback(const std_msgs::msg::Bool::SharedPtr msg)
  {

    
    if (!move_group_interface_) {
      RCLCPP_ERROR(this->get_logger(), "MoveGroupInterface not initialized yet!");
      return;
    }
    double target_value;
    if (msg->data) {
      target_value = is_drop_mode_ ? 0.65: 0.0;  // 開啟夾爪：依 drop_mode 判斷  
    } else {
      
      target_value = 1.03;// 關閉夾爪
    }
// 如果 msg->data == true：進入上方 if，打開

// 如果 msg->data == false：進入 else，夾資子關

// 如果 is_drop_mode_ == true：目標值為 1.03

// 如果 is_drop_mode_ == false：目標值為 0.5
    // double target_value = msg->data ? 0.65 : 1.05;
    // double target_value = msg->data ? 0.0 :1.05;
    std::string action = msg->data ? "close" : "open";

    RCLCPP_INFO(this->get_logger(), "Received command: %s (target=%.2f)", action.c_str(), target_value);

    // 設定目標關節角度
    // move_group_interface_->setJointValueTarget("joint7", target_value);
    
    move_group_interface_->setJointValueTarget("gripper_joint1", target_value);

    // 規劃並執行
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    bool success = (move_group_interface_->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

    if (success) {
      move_group_interface_->execute(plan);
      RCLCPP_INFO(this->get_logger(), "Gripper executed %s successfully.", action.c_str());
    } else {
      RCLCPP_ERROR(this->get_logger(), "Gripper planning failed.");
    }
  }


  // void gripperCallback(const std_msgs::msg::Bool::SharedPtr msg)
  // {
  //   if (!move_group_interface_) {
  //     RCLCPP_ERROR(this->get_logger(), "MoveGroupInterface not initialized yet!");
  //     return;
  //   }

  //   // 若正在執行，不接受新指令（避免打架）
  //   if (is_gripper_busy_) {
  //     RCLCPP_WARN(this->get_logger(), "Gripper is busy. Ignoring command.");
  //     return;
  //   }

  //   is_gripper_busy_ = true;

  //   double target_value = msg->data ? 0.0 : 0.10;
  //   std::string action = msg->data ? "close" : "open";

  //   RCLCPP_INFO(this->get_logger(), "🟢 Received command: %s (target=%.2f)", action.c_str(), target_value);

  //   move_group_interface_->setJointValueTarget("joint7", target_value);

  //   moveit::planning_interface::MoveGroupInterface::Plan plan;
  //   bool success = (move_group_interface_->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

  //   if (success) {
  //     move_group_interface_->execute(plan);
  //     RCLCPP_INFO(this->get_logger(), "✅ Gripper executed %s successfully.", action.c_str());
  //   } else {
  //     RCLCPP_ERROR(this->get_logger(), "❌ Gripper planning failed.");
  //   }

  //   // 正確：透過 dynamic_pointer_cast 取得原本的類別型別
  //   auto self = std::dynamic_pointer_cast<GripperControlNode>(this->shared_from_this());
  //   this->create_wall_timer(std::chrono::milliseconds(500), [self]() {
  //     self->is_gripper_busy_ = false;
  //     RCLCPP_INFO(self->get_logger(), "🔄 Gripper ready for next command.");
  //   });
  // }



  std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr gripper_cmd_sub_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr drop_mode_sub_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<GripperControlNode>();
  node->init();  // 初始化 MoveGroupInterface

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
