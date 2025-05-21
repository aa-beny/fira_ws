// #include <memory>
// #include <rclcpp/rclcpp.hpp>
// #include <std_msgs/msg/string.hpp>
// #include <geometry_msgs/msg/pose.hpp>
// #include <moveit/move_group_interface/move_group_interface.h>

// class PoseGoalNode : public rclcpp::Node
// {
// public:
//     PoseGoalNode()
//     : Node("pose_goal_node"),
//       logger_(this->get_logger())
//     {
//         publisher_ = this->create_publisher<std_msgs::msg::String>("pose_goal_status", 10);

//         subscriber_ = this->create_subscription<geometry_msgs::msg::Pose>(
//             "/target_pose_in_base", 10,
//             std::bind(&PoseGoalNode::cmdCallback, this, std::placeholders::_1)
//         );

//         RCLCPP_INFO(logger_, "PoseGoalNode started.");
//     }

// private:
//     void cmdCallback(const geometry_msgs::msg::Pose::SharedPtr msg)
//     {
//         if (!move_group_interface_)
//         {   
//             move_group_interface_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
//                 shared_from_this(), "small_arm");
//             move_group_interface_->setPlanningTime(5.0);  // Optional: Set a max planning time
//             move_group_interface_->setMaxVelocityScalingFactor(0.05);  // 預設是 1.0
//     	    move_group_interface_->setMaxAccelerationScalingFactor(0.05);  // 預設是 1.0
//             // move_group_interface_->setVelocityScalingFactor(0.05);
//             // move_group_interface_->setAccelerationScalingFactor(0.05);
//         }

//         RCLCPP_INFO(logger_, "[SUB] Received target pose: [x=%.4f, y=%.4f, z=%.4f]",
//                     msg->position.x, msg->position.y, msg->position.z);

//         // RCLCPP_INFO(logger_, "Velocity scaling: %.2f", move_group_interface_->getMaxVelocityScalingFactor());
//         // RCLCPP_INFO(logger_, "Acceleration scaling: %.2f", move_group_interface_->getMaxAccelerationScalingFactor());

//         move_group_interface_->setPoseTarget(*msg);

//         moveit::planning_interface::MoveGroupInterface::Plan my_plan;
//         bool success = (move_group_interface_->plan(my_plan) == moveit::core::MoveItErrorCode::SUCCESS);
            
//         std_msgs::msg::String status_msg;
//         if (success)
//         {
//             RCLCPP_INFO(logger_, "[MoveIt] Plan success. Executing...");
//             move_group_interface_->execute(my_plan);
//             status_msg.data = "[PUB] Work success.";
//         }
//         else
//         {
//             RCLCPP_ERROR(logger_, "[MoveIt] Plan failed.");
//             status_msg.data = "[PUB] Plan failed.";
//         }

//         publisher_->publish(status_msg);
//     }

//     rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
//     rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr subscriber_;
//     std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface_;
//     rclcpp::Logger logger_;
// };


// int main(int argc, char *argv[])
// {
//     rclcpp::init(argc, argv);
//     rclcpp::spin(std::make_shared<PoseGoalNode>());
//     rclcpp::shutdown();
//     return 0;
// }



#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <std_msgs/msg/bool.hpp>
class PoseGoalNode : public rclcpp::Node
{
// class 裡面成員變數
bool trigger_enabled_ = true;
public:
    PoseGoalNode()
    : Node("pose_goal_node"),
      logger_(this->get_logger())
    {
        publisher_ = this->create_publisher<std_msgs::msg::Bool>("pose_goal_status", 10);
        

        subscriber_ = this->create_subscription<geometry_msgs::msg::Pose>(
            "/target_pose_in_base", 10,
            std::bind(&PoseGoalNode::cmdCallback, this, std::placeholders::_1)
        );
        // 訂閱者初始化
        trigger_sub_ = this->create_subscription<std_msgs::msg::Bool>(
            "/do_detect_fira", 10,
            std::bind(&PoseGoalNode::trigger_callback, this, std::placeholders::_1));

        last_pose_time_ = this->now();  // ✅ 初始化時間
        timeout_timer_ = this->create_wall_timer(  // ✅ 啟動定時器，每秒檢查一次
            std::chrono::seconds(1),
            std::bind(&PoseGoalNode::checkTimeout, this)
        );

        RCLCPP_INFO(logger_, "PoseGoalNode started.");
    }
// callback 實作
void trigger_callback(const std_msgs::msg::Bool::SharedPtr msg)
{
    if (msg->data && trigger_enabled_)
    {
        if (!move_group_interface_)
        {
            move_group_interface_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
                shared_from_this(), "small_arm");
            move_group_interface_->setPlanningTime(5.0);
            move_group_interface_->setMaxVelocityScalingFactor(0.05);
            move_group_interface_->setMaxAccelerationScalingFactor(0.05);
        }

        RCLCPP_INFO(this->get_logger(), "🟢 Received TRUE → Moving to detect_fira");
        // move_group_interface_->setNamedTarget("detect_fira");
        move_group_interface_->setNamedTarget("home");

        // 建議先嘗試規劃再執行
        moveit::planning_interface::MoveGroupInterface::Plan my_plan;
        bool success = (move_group_interface_->plan(my_plan) == moveit::core::MoveItErrorCode::SUCCESS);
        
        // std_msgs::msg::String status_msg;
        std_msgs::msg::Bool status_msg;
        if (success)
        {
            move_group_interface_->execute(my_plan);
            RCLCPP_INFO(this->get_logger(), "✅ Executed detect_fira.");
            status_msg.data = true;
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "❌ Planning to detect_fira failed.");
            status_msg.data = false;
        }

        // publisher_->publish(status_msg);
        // RCLCPP_INFO(this->get_logger(), "📤 Published pose status: %s", success ? "True" : "False");
        trigger_enabled_ = false;
        RCLCPP_INFO(this->get_logger(), "📴 Auto-reset disabled until next TRUE");
    }
    else if (!msg->data)
    {
        trigger_enabled_ = true;
        RCLCPP_INFO(this->get_logger(), "🔁 Received FALSE → Re-arming movement trigger");
    }
}

private:
    void cmdCallback(const geometry_msgs::msg::Pose::SharedPtr msg)
    {
        last_pose_time_ = this->now();  // ✅ 更新最近收到 pose 的時間

        if (!move_group_interface_)
        {
            move_group_interface_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
                shared_from_this(), "small_arm");
            move_group_interface_->setPlanningTime(5.0);
            move_group_interface_->setMaxVelocityScalingFactor(0.05);
            move_group_interface_->setMaxAccelerationScalingFactor(0.05);
        }

        RCLCPP_INFO(logger_, "[SUB] Received target pose: [x=%.4f, y=%.4f, z=%.4f]",
                    msg->position.x, msg->position.y, msg->position.z);

        move_group_interface_->setPoseTarget(*msg);

        moveit::planning_interface::MoveGroupInterface::Plan my_plan;
        bool success = (move_group_interface_->plan(my_plan) == moveit::core::MoveItErrorCode::SUCCESS);

        // std_msgs::msg::String status_msg;
        std_msgs::msg::Bool status_msg;
        if (success)
        {
            RCLCPP_INFO(logger_, "[MoveIt] Plan success. Executing...");
            move_group_interface_->execute(my_plan);
            status_msg.data = true;
        }
        else
        {
            RCLCPP_ERROR(logger_, "[MoveIt] Plan failed.");
            status_msg.data = false;
        }

        publisher_->publish(status_msg);
        RCLCPP_INFO(this->get_logger(), "📤 Published pose status: %s", success ? "True" : "False");

    }

    void checkTimeout()
    {
        rclcpp::Time now = this->now();
        if ((now - last_pose_time_).seconds() > 1.0)  // ✅ 超過 2 秒沒收到新 pose
        {
            if (move_group_interface_)
            {
                move_group_interface_->stop();  // ✅ 停止手臂
                RCLCPP_WARN(this->get_logger(), "[Timeout] No new target pose. Stopping the robot.");

                // std_msgs::msg::String status_msg;
                // std_msgs::msg::Bool status_msg;
                // status_msg.data = false;
                // publisher_->publish(status_msg);
                // RCLCPP_INFO(this->get_logger(), "📤 Published pose status: %s", success ? "True" : "False");
            }
        }
    }

    // rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr publisher_;
    rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr subscriber_;

    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr trigger_sub_;

    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface_;
    rclcpp::Logger logger_;

    rclcpp::TimerBase::SharedPtr timeout_timer_;  // ✅ 定時器
    rclcpp::Time last_pose_time_;  // ✅ 最近收到目標的時間
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PoseGoalNode>());
    rclcpp::shutdown();
    return 0;
}
