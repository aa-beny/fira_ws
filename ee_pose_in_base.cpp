#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2/LinearMath/Quaternion.h>

class CameraToBasePrinter : public rclcpp::Node {
public:
  CameraToBasePrinter()
  : Node("camera_to_base_printer"),
    tf_buffer_(this->get_clock()),
    tf_listener_(tf_buffer_)
  {
    // this->declare_parameter<std::vector<double>>("camera_to_ee_translate", {-0.053, 0.08, -0.6});
    this->declare_parameter<std::vector<double>>("camera_to_ee_translate", {-0.04, 0.10, -0.13});
    this->declare_parameter<std::vector<double>>("camera_to_ee_quaternion", {0.0, 0.0, 0.0, 1.0});

    subscription_ = this->create_subscription<geometry_msgs::msg::Pose>(
      "/text_coordinate", 10,
      std::bind(&CameraToBasePrinter::callback, this, std::placeholders::_1)
    );

    publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("/target_pose_in_base_stamped", 10);
  }

private:
  void callback(const geometry_msgs::msg::Pose::SharedPtr msg)
  {
    tf2::Vector3 point_in_camera(msg->position.x, msg->position.y, msg->position.z);

    auto cam_to_ee_t = this->get_parameter("camera_to_ee_translate").as_double_array();
    auto cam_to_ee_q = this->get_parameter("camera_to_ee_quaternion").as_double_array();

    if (cam_to_ee_t.size() != 3 || cam_to_ee_q.size() != 4) {
      RCLCPP_WARN(this->get_logger(), "❌ camera_to_ee parameter wrong");
      return;
    }

    tf2::Vector3 translation_cam_to_ee(cam_to_ee_t[0], cam_to_ee_t[1], cam_to_ee_t[2]);
    tf2::Quaternion rotation_cam_to_ee(cam_to_ee_q[0], cam_to_ee_q[1], cam_to_ee_q[2], cam_to_ee_q[3]);
    tf2::Transform T_cam_to_ee(rotation_cam_to_ee, translation_cam_to_ee);

    try {
      geometry_msgs::msg::TransformStamped ee_to_base_tf =
        tf_buffer_.lookupTransform("base_link", "link6", tf2::TimePointZero);

      tf2::Transform T_ee_to_base;
      tf2::fromMsg(ee_to_base_tf.transform, T_ee_to_base);

      tf2::Vector3 point_in_ee = T_cam_to_ee * point_in_camera;
      tf2::Vector3 point_in_base = T_ee_to_base * point_in_ee;

      geometry_msgs::msg::PoseStamped stamped_pose;
      stamped_pose.header.stamp = this->get_clock()->now();
      stamped_pose.header.frame_id = "world";  // ← 要和 RViz Fixed Frame 一致
      stamped_pose.pose.position.x = point_in_base.x();
      stamped_pose.pose.position.y = point_in_base.y();
      stamped_pose.pose.position.z = point_in_base.z();
      stamped_pose.pose.orientation.w = 1.0;  // 單位四元數，其餘為 0

      publisher_->publish(stamped_pose);

      RCLCPP_INFO(this->get_logger(),
        "📌 Published PoseStamped in base_link: x=%.3f y=%.3f z=%.3f",
        point_in_base.x(), point_in_base.y(), point_in_base.z());

    } catch (const tf2::TransformException & ex) {
      RCLCPP_WARN(this->get_logger(), "TF translate fail: %s", ex.what());
    }
  }

  tf2_ros::Buffer tf_buffer_;
  tf2_ros::TransformListener tf_listener_;
  rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr subscription_;
  rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CameraToBasePrinter>());
  rclcpp::shutdown();
  return 0;
}
