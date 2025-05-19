// 引入 C++ 標準函式庫的相關標頭檔
#include <memory>       // 提供 smart pointers（如 shared_ptr）
#include <vector>       // 提供 std::vector
#include <iostream>     // 提供輸出功能
#include <string>       // 提供 std::string
#include <thread>       // 提供多執行緒 std::thread
#include <termios.h>    // 提供終端控制（目前未使用）
#include <unistd.h>     // 提供 sleep、close 等 POSIX 函式（sleep 會用到）
#include <chrono>       // 提供時間控制（例如 sleep_for）

// 引入 ROS2 與 MoveIt2 的相關標頭檔
#include <rclcpp/rclcpp.hpp>                                        // ROS2 基礎節點功能
#include <geometry_msgs/msg/pose.hpp>                               // ROS Pose 訊息格式
#include <moveit/move_group_interface/move_group_interface.h>       // MoveIt2 中的機械臂控制介面

// 宣告一個繼承自 rclcpp::Node 的類別 PoseSequencer，負責執行序列化的姿勢控制
class PoseSequencer : public rclcpp::Node, public std::enable_shared_from_this<PoseSequencer> {
public:
  PoseSequencer()
  : Node("pose_sequence_keyboard_controlled"), current_index_(0) {
    // 初始化 MoveGroupInterface，使用 "small_arm" 作為 move_group 名稱
    // 並確保傳入的是 shared_ptr 的 this（來自 enable_shared_from_this）
    move_group_interface_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
      std::enable_shared_from_this<PoseSequencer>::shared_from_this(), "small_arm");

    // 設定要依序移動的目標點（共 20 個）
    target_poses_ = {
      make_pose(-0.001, 0.184, 0.224), make_pose(-0.051, 0.188, 0.235),
      make_pose(-0.002, 0.188, 0.191), make_pose(0.020, 0.189, 0.191),
      make_pose(0.020, 0.185, 0.208), make_pose(0.040, 0.184, 0.212),
      make_pose(-0.072, 0.185, 0.221), make_pose(-0.019, 0.237, 0.177),
      make_pose(0.003, 0.120, 0.154), make_pose(-0.023, 0.158, 0.242),
      make_pose(-0.033, 0.160, 0.256), make_pose(0.019, 0.162, 0.254),
      make_pose(0.041, 0.103, 0.248), make_pose(0.066, 0.161, 0.254),
      make_pose(-0.002, 0.176, 0.177), make_pose(-0.002, 0.207, 0.169),
      make_pose(-0.057, 0.204, 0.200), make_pose(-0.090, 0.206, 0.202),
      make_pose(-0.042, 0.209, 0.153), make_pose(-0.005, 0.210, 0.150)
    };

    RCLCPP_INFO(this->get_logger(), "🔁 Moving to multiple poses with a 30s countdown between each...");

    // 建立一個背景 thread，不斷執行 trigger_next() 處理點位
    std::thread([this]() {
      while (rclcpp::ok()) {
        this->trigger_next();  // 嘗試移動到下一個目標點
      }
    }).detach();  // thread 分離，讓主執行緒可以進入 rclcpp::spin()
  }

private:
  // 建立簡單工具函式：輸入 x, y, z，產生一個預設為無旋轉的 Pose
  geometry_msgs::msg::Pose make_pose(double x, double y, double z) {
    geometry_msgs::msg::Pose pose;
    pose.orientation.w = 1.0;  // 無旋轉（單位四元數）
    pose.position.x = x;
    pose.position.y = y;
    pose.position.z = z;
    return pose;
  }

  // 主控制函式：依序執行每一個目標點位的移動
  void trigger_next() {
    // 若已完成全部點位，顯示完成訊息並返回
    if (current_index_ >= target_poses_.size()) {
      RCLCPP_INFO(this->get_logger(), "✅ All points completed.");
      return;
    }

    // 還有未完成的點位，就繼續執行
    while (current_index_ < target_poses_.size()) {
      RCLCPP_INFO(this->get_logger(), "▶️  Moving to point %zu", current_index_ + 1);

      // 倒數 30 秒提示，使用 sleep_for 模擬計時器
      for (int i = 30; i >= 0; --i) {
        RCLCPP_INFO(this->get_logger(), "⏳  Next move in: %d seconds", i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }

      // 設定目標位置為下一個 Pose
      move_group_interface_->setPoseTarget(target_poses_[current_index_]);

      // 嘗試規劃路徑
      moveit::planning_interface::MoveGroupInterface::Plan plan;
      bool success = (move_group_interface_->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

      // 如果規劃成功就執行
      if (success) {
        move_group_interface_->execute(plan);
        RCLCPP_INFO(this->get_logger(), "✅ Moved to point %zu", current_index_ + 1);
      } else {
        RCLCPP_WARN(this->get_logger(), "⚠️  Planning to point %zu failed", current_index_ + 1);
      }

      // 前進到下一個點位
      current_index_++;
    }
  }

  // === 成員變數區 ===
  size_t current_index_;    // 當前要移動的目標點 index
  std::vector<geometry_msgs::msg::Pose> target_poses_;   // 所有的目標點位
  std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface_; // MoveIt 操作介面
};

// === 主函式 ===
int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);  // 初始化 ROS2 節點

  auto node = std::make_shared<PoseSequencer>();  // 建立 PoseSequencer 節點物件

  rclcpp::spin(node);         // 開始執行節點（這會讓 ROS2 處理所有回呼與排程）

  rclcpp::shutdown();         // 當節點結束，關閉 ROS2 系統
  return 0;
}
