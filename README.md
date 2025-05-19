# 🯢 RealSense + MoveIt2 機械臂整合專案

本專案整合了 Intel RealSense 相機文字識別（OCR）與 MoveIt 2 機械臂控制功能，支援多模組視覺處理與控制操作。

---

## 📦 環境建立

1. 匯入套件並安裝依賴：

```bash
vcs import src < src/tb4_arm_ros2/dynamixel_control.repos
rosdep update
sudo apt update
rosdep install --from-paths src --ignore-src -r -y
```

---

## 👁️ 視覺模組

### 📦 個別節點啟動

| 節點 | 功能 |
|------|------|
| `color_range` | 色彩區域檢測 |
| `realsense_read` | RealSense 相機讀取與影像發布 |
| `paddleorc` | PaddleOCR 英文文字識別 |
| `depth_calculate` | 深度座標轉換至相機座標系 |

```bash
ros2 run realsense color_range
ros2 run realsense realsense_read
ros2 run realsense paddleorc
ros2 run realsense depth_calculate
```

### 🧹 可視化整合啟動

```bash
ros2 launch realsense visual_realsense.py
```

---

## 🤖 機械臂控制模組

### ✅ 必要啟動

#### 1. MoveIt2 控制伺服器

```bash
ros2 launch small_arm_moveit_config moveit2_server.launch.py
```

#### 2. 控制主程式（必須）

```bash
ros2 launch small_arm_moveit_cpp moveit_main.launch.py
```

---

### 💪 節點單獨啟動

| Node | 功能 |
|------|------|
| `gripper_joint_value.cpp` | 控制夾爪張合（未測試） |
| `auto_pose_goal.cpp` | 訂閱末端點位 `/pose_goal_position` 並移動 |
| `name_goal.cpp` | 移動至命名點位（需在 SRDF 或 MoveIt Assistant 中定義） |
| `ee_pose_in_base.cpp` | 將相機座標轉換至 base_link 座標 |

```bash
ros2 run small_arm_moveit_cpp gripper_joint_value
ros2 run small_arm_moveit_cpp auto_pose_goal
ros2 run small_arm_moveit_cpp name_goal
ros2 run small_arm_moveit_cpp ee_pose_in_base
```

---

## 🎯 測試點位發送

以下範例將單次發送一個末端點位（Pose）：

```bash
ros2 topic pub --once /pose_goal_position geometry_msgs/msg/Pose \
"{position: {x: -0.061, y: 0.287, z: 0.030}, orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}"
```

---

## 💡 備註

- 請確認 RealSense 啟動時使用 **對齒深度** `/camera/aligned_depth_to_color/image_raw` 並配合 **color 相機內參**。
- 若出現影像設備錯誤，請確認 USB 連線與驅動是否正確載入。
- PaddleOCR 預設使用 **GPU**，需確保 docker 中 CUDA 驅動配置正確。

# fira_ws
