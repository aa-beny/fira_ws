// #include <memory>
// #include <rclcpp/rclcpp.hpp>
// #include <geometry_msgs/msg/pose.hpp>
// #include <geometry_msgs/msg/pose_stamped.hpp>
// #include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
// #include <tf2_ros/transform_listener.h>
// #include <tf2_ros/buffer.h>
// #include <tf2/LinearMath/Transform.h>
// #include <tf2/LinearMath/Quaternion.h>

// class CameraToBasePrinter : public rclcpp::Node {
// public:
//   CameraToBasePrinter()
//   : Node("camera_to_base_printer"),
//     tf_buffer_(this->get_clock()),
//     tf_listener_(tf_buffer_)
//   {
//     // this->declare_parameter<std::vector<double>>("camera_to_ee_translate", {-0.04, 0.10, -0.13});
//     this->declare_parameter<std::vector<double>>("camera_to_ee_translate", {-0.04, 0.035,-0.065});
//     // this->declare_parameter<std::vector<double>>("camera_to_ee_translate", {-0.032, 0.049, -0.018});
//     // this->declare_parameter<std::vector<double>>("camera_to_ee_quaternion", {0.0, 0.0, 0.0, 1.0});
//     this->declare_parameter<std::vector<double>>("camera_to_ee_quaternion", {0.0, 0.0, 0.0, 1.0});
//     // this->declare_parameter<std::vector<double>>("camera_to_ee_quaternion", {0.5, -0.5, 0.5, -0.5});

//     subscription_ = this->create_subscription<geometry_msgs::msg::Pose>(
//       "/text_coordinate", 10,
//       std::bind(&CameraToBasePrinter::callback, this, std::placeholders::_1)
//     );

//     publisher_ = this->create_publisher<geometry_msgs::msg::Pose>("/target_pose_in_base", 10);
//     // publisher_ = this->create_publisher<geometry_msgs::msg::Pose>("/target_pose_in_base_stamped", 10);
//   }

// private:
//   void callback(const geometry_msgs::msg::Pose::SharedPtr msg)
//   {
//     tf2::Vector3 point_in_camera(msg->position.x, msg->position.y, msg->position.z);

//     auto cam_to_ee_t = this->get_parameter("camera_to_ee_translate").as_double_array();
//     auto cam_to_ee_q = this->get_parameter("camera_to_ee_quaternion").as_double_array();


//     if (cam_to_ee_t.size() != 3 || cam_to_ee_q.size() != 4) {
//       RCLCPP_WARN(this->get_logger(), "❌ camera_to_ee parameter wrong");
//       return;
//     }

//     tf2::Vector3 translation_cam_to_ee(cam_to_ee_t[0], cam_to_ee_t[1], cam_to_ee_t[2]);
//     tf2::Quaternion rotation_cam_to_ee(cam_to_ee_q[0], cam_to_ee_q[1], cam_to_ee_q[2], cam_to_ee_q[3]);
//     tf2::Transform T_cam_to_ee(rotation_cam_to_ee, translation_cam_to_ee);

//     try {
//       geometry_msgs::msg::TransformStamped ee_to_base_tf =
//         tf_buffer_.lookupTransform("base_link", "link6", tf2::TimePointZero);
//         // tf_buffer_.lookupTransform("base_link", "camera_link", tf2::TimePointZero);

//       tf2::Transform T_ee_to_base;
//       tf2::fromMsg(ee_to_base_tf.transform, T_ee_to_base);

//       tf2::Vector3 point_in_ee = T_cam_to_ee * point_in_camera;
//       tf2::Vector3 point_in_base = T_ee_to_base * point_in_ee;

//       RCLCPP_INFO(this->get_logger(), "Camera Point: (%.3f, %.3f, %.3f)",
//             msg->position.x, msg->position.y, msg->position.z);
//       RCLCPP_INFO(this->get_logger(), "EE Point:     (%.3f, %.3f, %.3f)",
//             point_in_ee.x(), point_in_ee.y(), point_in_ee.z());
//       RCLCPP_INFO(this->get_logger(), "Base Point:   (%.3f, %.3f, %.3f)",
//             point_in_base.x(), point_in_base.y(), point_in_base.z());

//       geometry_msgs::msg::Pose target_pose;
//       target_pose.position.x = point_in_base.x();
//       target_pose.position.y = point_in_base.y();
//       target_pose.position.z = point_in_base.z();
//       target_pose.orientation.w = 1.0;  // 單位四元數，其餘為 0

//       publisher_->publish(target_pose);

//       RCLCPP_INFO(this->get_logger(),
//         "📌 Publish base_link coordinate: x=%.3f y=%.3f z=%.3f",
//         point_in_base.x(), point_in_base.y(), point_in_base.z());

//     } catch (const tf2::TransformException & ex) {
//       RCLCPP_WARN(this->get_logger(), "TF translate fail: %s", ex.what());
//     }
//   }

//   tf2_ros::Buffer tf_buffer_;
//   tf2_ros::TransformListener tf_listener_;

//   rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr subscription_;
//   rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr publisher_;
// };

// int main(int argc, char * argv[]) {
//   rclcpp::init(argc, argv);
//   rclcpp::spin(std::make_shared<CameraToBasePrinter>());
//   rclcpp::shutdown();
//   return 0;
// }










#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2/LinearMath/Quaternion.h>
#include "std_msgs/msg/float32.hpp"
float received_target_z_; // 初始值（避免沒收到）
bool target_z_received_ = false;
bool pose_received_ = false;
// CameraToBasePrinter 節點負責將從相機收到的 3D 座標（來自文字辨識）
// 轉換成機器人底座 base_link 座標系，提供手臂控制器用。
class CameraToBasePrinter : public rclcpp::Node {
public:
  CameraToBasePrinter()
  // 🔸 這段是「建構函式」，當這個 ROS 節點啟動時會執行。
  // 🔸 tf_buffer_ 和 tf_listener_ 是 tf2 的核心工具，用來查詢兩個座標系（frame）間的轉換關係。
  : Node("camera_to_base_printer"),
    tf_buffer_(this->get_clock()),
    tf_listener_(tf_buffer_)
  {
    // 🔸 這裡宣告兩個參數：相機與末端 effector（link6）間的手動轉換資訊。
    // 🔸 平移部分代表：相機位於末端前方 4cm、右側 3.5cm、上方 6.5cm
    // 🔸 四元數 {0, 0, 0, 1} 代表相機方向與末端座標對齊，沒有旋轉。
    this->declare_parameter<std::vector<double>>("camera_to_ee_translate", {-0.04, 0.035,-0.065});
    this->declare_parameter<std::vector<double>>("camera_to_ee_quaternion", {0.0, 0.0, 0.0, 1.0});

    // 🔸 訂閱來自 OCR 節點（例如 paddleocr 或 vision node）的訊息，
    // 主題名稱為 /text_coordinate，格式是 geometry_msgs::msg::Pose。
    // 🔸 每當收到座標訊息，就會自動呼叫 callback() 處理該點位。
    subscription_ = this->create_subscription<geometry_msgs::msg::Pose>(
      "/text_coordinate", 10,
      std::bind(&CameraToBasePrinter::callback, this, std::placeholders::_1)
    );

    z_level_sub_ = this->create_subscription<std_msgs::msg::Float32>(
    "/target_z_height", 10,
    [this](std_msgs::msg::Float32::SharedPtr msg) {
        received_target_z_ = msg->data;
        target_z_received_ = true;
        RCLCPP_INFO(this->get_logger(), "📥 Received target_z from topic: %.3f", received_target_z_);
    });

    // 🔸 建立一個 publisher，將轉換後的點（在 base_link 座標系）送給機械手臂。
    publisher_ = this->create_publisher<geometry_msgs::msg::Pose>("/target_pose_in_base", 10);

    // ✅ 加入：初始化最近接收 pose 的時間
    last_pose_time_ = this->now();

    // ✅ 加入：定時檢查是否已經超過一段時間沒收到座標（每秒檢查一次）
    timeout_timer_ = this->create_wall_timer(
      std::chrono::seconds(1),
      std::bind(&CameraToBasePrinter::checkTimeout, this)
    );
  }

private:
  void callback(const geometry_msgs::msg::Pose::SharedPtr msg)
  {
    double camera_x = msg->position.x;
    double camera_z = msg->position.z;

    // double received_target_z_ = 0.105;  // 預設高度
    // bool executed_ = false;  
    
    // ✅ 加入：每次收到新 pose 就更新時間戳
    last_pose_time_ = this->now();  // 更新接收時間
    
    
    geometry_msgs::msg::Pose input_pose = *msg;// ✅ 把 ROS 傳進來的座標資料複製成變數 input_pose，好讓我們後面能使用這個值
    
    RCLCPP_INFO(this->get_logger(),"🎯 Camera XYZ: (%.3f, %.3f, %.3f)", msg->position.x, msg->position.y, msg->position.z);// ✅ 印出收到的相機座標（也就是偵測到文字的位置），幫助你在 terminal 上除錯

    // tf2::Vector3 point_in_camera(input_pose.position.x, input_pose.position.y, 0.0);// ✅ 把相機的座標轉成 tf2 的向量格式，Z 我們先設成 0，因為我們主要關心 X 跟 Y 平面上的位置
    
    
    tf2::Vector3 point_in_camera(input_pose.position.x, input_pose.position.y, input_pose.position.z);  // ✅ 用進來的 Z 做 XY 轉換

    // ✅ 從參數中取得「相機到末端」的平移向量與旋轉四元數（這些是你手動設的 camera 位移跟方向）
    auto cam_to_ee_t = this->get_parameter("camera_to_ee_translate").as_double_array();
    auto cam_to_ee_q = this->get_parameter("camera_to_ee_quaternion").as_double_array();
    // ✅ 如果參數長度不正確，代表設定錯誤 → 回傳錯誤訊息，這樣程式不會繼續做下去
    if (cam_to_ee_t.size() != 3 || cam_to_ee_q.size() != 4) {
      RCLCPP_WARN(this->get_logger(), "Parameter format incorrect: camera_to_ee");
      return;
    }
    // ✅ 建立一個代表「從相機轉到末端」的座標轉換（平移 + 旋轉）
    tf2::Vector3 translation_cam_to_ee(cam_to_ee_t[0], cam_to_ee_t[1], cam_to_ee_t[2]);
    tf2::Quaternion rotation_cam_to_ee(cam_to_ee_q[0], cam_to_ee_q[1], cam_to_ee_q[2], cam_to_ee_q[3]);
    tf2::Transform T_cam_to_ee(rotation_cam_to_ee, translation_cam_to_ee);

    try {
      // ✅ 這是使用 tf2 查詢「base_link 到 link6」之間的座標轉換（也就是末端 → base）
      geometry_msgs::msg::TransformStamped ee_to_base_tf =
        tf_buffer_.lookupTransform("base_link", "link6", tf2::TimePointZero);

      // ✅ 把 ROS 格式的 Transform 轉成 tf2 的格式，這樣才能進行矩陣運算
      tf2::Transform T_ee_to_base;
      tf2::fromMsg(ee_to_base_tf.transform, T_ee_to_base);

       // ✅ 把點從「相機座標」先轉換到「末端座標」
      tf2::Vector3 point_in_ee = T_cam_to_ee * point_in_camera;
      RCLCPP_INFO(this->get_logger(), "🦾 In EE:     (%.3f, %.3f, %.3f)",point_in_ee.x(), point_in_ee.y(), point_in_ee.z());

      // ✅ 接著再把這個點從「末端座標」轉換到「base_link 座標」
      tf2::Vector3 point_in_base = T_ee_to_base * point_in_ee;
      RCLCPP_INFO(this->get_logger(), "🏁 In Base:   (%.3f, %.3f, %.3f)",point_in_base.x(), point_in_base.y(), point_in_base.z());

      // double high_camera_x = point_in_base.x();
      tf2::Vector3 camera_to_base = T_ee_to_base * T_cam_to_ee * tf2::Vector3(0, 0, 0);
      // T_cam_to_ee 是你手動設定的 camera → ee 的轉換矩陣。

      // 乘上 T_ee_to_base 就是 camera → base 的總體轉換。

      // 所以這一行代表你相機的位置在 base_link 下面的座標為 (-0.043, 0.260, 0.059)。

      RCLCPP_INFO(this->get_logger(), "Camera to base: x=%.3f y=%.3f z=%.3f", camera_to_base.x(), camera_to_base.y(), camera_to_base.z());

      tf2::Vector3 ee_in_base = T_ee_to_base * tf2::Vector3(0, 0, 0);//從「末端（link6）的原點」出發，經過 ee → base 的轉換，得到「末端原點在 base 座標系下的位置」。
      // T_ee_to_base 是 link6 → base_link 的 tf2 轉換矩陣。
      // tf2::Vector3(0,0,0) 表示末端（link6）自身的原點。
      // ee_in_base 就是「末端原點在 base 座標系的座標」。
      // 這裡 x≈0、y=0.221、z=0.121，表示你手臂末端目前在 base_link 的位置是在 Y 軸 22cm 左邊、Z 軸 12cm 高處。
      RCLCPP_INFO(this->get_logger(), "ee_in_base: x=%.3f y=%.3f z=%.3f", ee_in_base.x(), ee_in_base.y(), ee_in_base.z());

      // ✅ 建立最終要發送給機械手臂的 Pose
      geometry_msgs::msg::Pose target_pose;
      // double dx = -0.04;
      
      // double dy = 0.065;
      // double dy = 0.06;


      // double dx = -0.06;
      // double dy = 0.06;
      // double dx = 0.0;
      // // double dy = 0.0;
      // if (target_pose.position.x < -0.10) {
      //       dx = 0.065;  // ← 左側偏左 → 加一點 X
      //   } else if (target_pose.position.x > 0.10) {
      //       dx = -0.025;  // → 右側偏右 → 減一點 X
      //   } else {
      //       dx = 0.0;  // 中間就不用補
      // }
      double dx = 0.0;
      double dy = 0.0;

      // 用 target_pose.position.x/z 直接判斷，不需要額外變數
      // if (camera_z < 0.02) {  // LOW
      //     if (camera_x < -0.10) {
      //         dx = 0.015; dy = 0.06;
      //         RCLCPP_INFO(this->get_logger(), "🟥 LOW-LEFT → dx=%.3f dy=%.3f", dx, dy);
      //     } else if (camera_x > 0.10) {
      //         dx = -0.02; dy = 0.06;
      //         RCLCPP_INFO(this->get_logger(), "🟥 LOW-RIGHT → dx=%.3f dy=%.3f", dx, dy);
      //     } else {
      //         dx = 0.0; dy = 0.06;
      //         RCLCPP_INFO(this->get_logger(), "🟥 LOW-MID → dx=%.3f dy=%.3f", dx, dy);
      //     }
      // } else if (camera_z < 0.08) {  // MID
      //     if (camera_x < -0.11) {
      //         dx = 0.068; dy = 0.058;
      //         RCLCPP_INFO(this->get_logger(), "🟧 MID-LEFT → dx=%.3f dy=%.3f", dx, dy);
      //     } else if (camera_x > 0.10) {
      //         dx = -0.075; dy = 0.058;
      //         RCLCPP_INFO(this->get_logger(), "🟧 MID-RIGHT → dx=%.3f dy=%.3f", dx, dy);
      //     } else {
      //         dx = 0.0; dy = 0.058;
      //         RCLCPP_INFO(this->get_logger(), "🟧 MID-MID → dx=%.3f dy=%.3f", dx, dy);
      //     }
      // } else {  // HIGH
      //     if (camera_x < -0.10) {
      //         dx = 0.01; dy = 0.10;
      //         RCLCPP_INFO(this->get_logger(), "🟩 HIGH-LEFT → dx=%.3f dy=%.3f", dx, dy);
      //     } else if (camera_x > 0.10) {
      //         dx = -0.02; dy = 0.10;
      //         RCLCPP_INFO(this->get_logger(), "🟩 HIGH-RIGHT → dx=%.3f dy=%.3f", dx, dy);
      //     } else {
      //         dx = 0.0; dy = 0.10;
      //         RCLCPP_INFO(this->get_logger(), "🟩 HIGH-MID → dx=%.3f dy=%.3f", dx, dy);
      //     }
      // }


     // 到時候要改轉換完的座標判斷 用 target_pose.position.x/z 直接判斷，不需要額外變數
      if (camera_z < 0.02) {  // LOW
          if (camera_x > 0.01) {
              dx = 0.07; dy = 0.00;
              RCLCPP_INFO(this->get_logger(), "🟥 LOW-RIGHT → dx=%.3f dy=%.3f", dx, dy);
          } else {
              dx = -0.02; dy = 0.00;
              RCLCPP_INFO(this->get_logger(), "🟥 LOW-LEFT → dx=%.3f dy=%.3f", dx, dy);
          }
      } else if (camera_z < 0.08) {  // MID
          if (camera_x > 0.01) {
              dx = 0.07; dy = 0.00;
              RCLCPP_INFO(this->get_logger(), "🟧 MID-RIGHT → dx=%.3f dy=%.3f", dx, dy);
          } else {
              dx = -0.02; dy = 0.00;
              RCLCPP_INFO(this->get_logger(), "🟧 MID-LEFT → dx=%.3f dy=%.3f", dx, dy);
          }
      } else {  // HIGH
          if (camera_x > 0.01) {
              dx = 0.02; dy = 0.000;
              // dx = 0.0; dy = 0.0;
              RCLCPP_INFO(this->get_logger(), "🟩 HIGH-RIGHT → dx=%.3f dy=%.3f", dx, dy);
          } else {
              dx = -0.00; dy = 0.000;
              //  dx = 0.0; dy = 0.0;
              RCLCPP_INFO(this->get_logger(), "🟩 HIGH-LEFT → dx=%.3f dy=%.3f", dx, dy);
          }
      }







      
      target_pose.position.x = point_in_base.x() + dx;
      target_pose.position.y = point_in_base.y() + dy;
      // target_pose.position.z = input_pose.position.z;
      target_pose.position.z = received_target_z_;  // ✅ 使用傳過來的固定高度

      // point_in_base 是：相機內偵測到的點 → 相機 → ee → base 的最終轉換結果。

      // 再加上偏移量 dx=-0.04、dy=+0.035：這是你自己調整機械臂接觸點位置的修正。

     // z=0.1 是你根據深度圖的 input_pose.position.z 設定。
      RCLCPP_INFO(this->get_logger(),
        "Converted Pose:\n  XY Transformed | Z (raw+offset)\n  → x=%.3f y=%.3f z=%.3f",
        target_pose.position.x, target_pose.position.y, target_pose.position.z);
      RCLCPP_INFO(this->get_logger(),
          "🎯 補償後目標位置：x=%.3f, y=%.3f, z=%.3f",
          target_pose.position.x,
          target_pose.position.y,
          target_pose.position.z);

      // ✅ 使用 T_ee_to_base 裡的四元數來取得目前末端方向
      tf2::Quaternion ee_current_rotation = T_ee_to_base.getRotation();
      // 表示 link6 → base 的旋轉姿態（四元數）。
      // 幾乎是 (0, 0, 0, 1)，代表沒有旋轉（identity quaternion）→ 和 base 對齊。
      // 如果這邊曾出現 (0, 0, 1, 0) 就是轉了 180 度，那很危險。
      RCLCPP_INFO(this->get_logger(), "🔁 Orientation from TF: x=%.3f y=%.3f z=%.3f w=%.3f",
                  ee_current_rotation.x(), ee_current_rotation.y(), ee_current_rotation.z(), ee_current_rotation.w());
      target_pose.orientation = tf2::toMsg(ee_current_rotation);



// // ✅ 過濾：只接受落在安全操作區的點
//       if (target_pose.position.x < -0.10 || target_pose.position.x > 0.25 ||
//           target_pose.position.y < -0.15 || target_pose.position.y > 0.25 ||
//           target_pose.position.z < 0.005 || target_pose.position.z > 0.3)
//       {
//           RCLCPP_WARN(this->get_logger(),
//               "❌ Skipped invalid target pose: x=%.3f y=%.3f z=%.3f",
//               target_pose.position.x, target_pose.position.y, target_pose.position.z);
//           return;
//       }


      if (target_z_received_) {
        publisher_->publish(target_pose);
        // pose_received_ = false;  // ✅ 發佈完一次就關閉，等下一次新座標
        target_z_received_ = false;  // ✅ 下一次要重新等 z 和 pose 都來
      }
      // publisher_->publish(target_pose);
    

    } catch (const tf2::TransformException & ex) {
      RCLCPP_WARN(this->get_logger(), "TF lookup failed: %s", ex.what());
    }
  }

  // ✅ 新增：檢查是否超過 2 秒沒收到座標資料
  void checkTimeout()
  {
    rclcpp::Time now = this->now();
    if ((now - last_pose_time_).seconds() > 50.0)
    {
      RCLCPP_WARN(this->get_logger(), "No pose received recently. Stopping pose publish.");
    }
  }




  
  tf2_ros::Buffer tf_buffer_;
  tf2_ros::TransformListener tf_listener_;
  rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr subscription_;
  rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr publisher_;

  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr z_level_sub_;

  rclcpp::TimerBase::SharedPtr timeout_timer_; // ✅ 新增變數：定時器
  rclcpp::Time last_pose_time_; // ✅ 新增變數：紀錄最後一次 pose 接收時間
};

int main(int argc, char * argv[]) {
  // 🔸 初始化 ROS 2 節點並執行這個節點直到被手動中止
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CameraToBasePrinter>());
  rclcpp::shutdown();
  return 0;
}




// #include <memory>
// #include <rclcpp/rclcpp.hpp>
// #include <geometry_msgs/msg/pose.hpp>
// #include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
// #include <tf2_ros/transform_listener.h>
// #include <tf2_ros/buffer.h>
// #include <tf2/LinearMath/Transform.h>
// #include <tf2/LinearMath/Quaternion.h>

// class CameraToBasePrinter : public rclcpp::Node {
// public:
//   CameraToBasePrinter()
//   : Node("camera_to_base_printer"),
//     tf_buffer_(this->get_clock()),
//     tf_listener_(tf_buffer_)
//   {
//     // 相機相對於末端執行器（link6）的手動轉換
//     this->declare_parameter<std::vector<double>>("camera_to_ee_translate", {-0.04, 0.035, -0.065});
//     this->declare_parameter<std::vector<double>>("camera_to_ee_quaternion", {0.0, 0.0, 0.0, 1.0});

//     subscription_ = this->create_subscription<geometry_msgs::msg::Pose>(
//       "/text_coordinate", 10,
//       std::bind(&CameraToBasePrinter::callback, this, std::placeholders::_1)
//     );

//     publisher_ = this->create_publisher<geometry_msgs::msg::Pose>("/target_pose_in_base", 10);
//   }

// private:
//   void callback(const geometry_msgs::msg::Pose::SharedPtr msg)
//   {
//     // 接收到相機座標的 2D 投影點，z 來自深度圖
//     tf2::Vector3 point_in_camera(msg->position.x, msg->position.y, msg->position.z);

//     // 讀取手動設定的相機 → 末端的轉換參數
//     auto cam_to_ee_t = this->get_parameter("camera_to_ee_translate").as_double_array();
//     auto cam_to_ee_q = this->get_parameter("camera_to_ee_quaternion").as_double_array();

//     if (cam_to_ee_t.size() != 3 || cam_to_ee_q.size() != 4) {
//       RCLCPP_WARN(this->get_logger(), "❌ camera_to_ee 參數格式錯誤");
//       return;
//     }

//     tf2::Transform T_cam_to_ee(tf2::Quaternion(cam_to_ee_q[0], cam_to_ee_q[1], cam_to_ee_q[2], cam_to_ee_q[3]),
//                                 tf2::Vector3(cam_to_ee_t[0], cam_to_ee_t[1], cam_to_ee_t[2]));

//     try {
//       // 查詢末端執行器到 base_link 的 tf
//       geometry_msgs::msg::TransformStamped ee_to_base_tf =
//         tf_buffer_.lookupTransform("base_link", "link6", tf2::TimePointZero);

//       tf2::Transform T_ee_to_base;
//       tf2::fromMsg(ee_to_base_tf.transform, T_ee_to_base);

//       // 組合轉換：camera → ee → base
//       tf2::Vector3 point_in_base = T_ee_to_base * (T_cam_to_ee * point_in_camera);

//       // 建立最終 pose 並發布
//       geometry_msgs::msg::Pose target_pose;
//       target_pose.position.x = point_in_base.x();
//       target_pose.position.y = point_in_base.y();
//       target_pose.position.z = point_in_base.z();
//       target_pose.orientation = tf2::toMsg(T_ee_to_base.getRotation());

//       publisher_->publish(target_pose);

//       RCLCPP_INFO(this->get_logger(),
//         "📌 Transformed Point → x=%.3f y=%.3f z=%.3f",
//         point_in_base.x(), point_in_base.y(), point_in_base.z());

//     } catch (const tf2::TransformException & ex) {
//       RCLCPP_WARN(this->get_logger(), "TF lookup failed: %s", ex.what());
//     }
//   }

//   tf2_ros::Buffer tf_buffer_;
//   tf2_ros::TransformListener tf_listener_;
//   rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr subscription_;
//   rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr publisher_;
// };

// int main(int argc, char * argv[]) {
//   rclcpp::init(argc, argv);
//   rclcpp::spin(std::make_shared<CameraToBasePrinter>());
//   rclcpp::shutdown();
//   return 0;
// }
