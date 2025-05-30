// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from dynamixel_workbench_msgs:msg/AX.idl
// generated code does not contain a copyright notice

#ifndef DYNAMIXEL_WORKBENCH_MSGS__MSG__DETAIL__AX__BUILDER_HPP_
#define DYNAMIXEL_WORKBENCH_MSGS__MSG__DETAIL__AX__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "dynamixel_workbench_msgs/msg/detail/ax__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace dynamixel_workbench_msgs
{

namespace msg
{

namespace builder
{

class Init_AX_punch
{
public:
  explicit Init_AX_punch(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  ::dynamixel_workbench_msgs::msg::AX punch(::dynamixel_workbench_msgs::msg::AX::_punch_type arg)
  {
    msg_.punch = std::move(arg);
    return std::move(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_lock
{
public:
  explicit Init_AX_lock(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_punch lock(::dynamixel_workbench_msgs::msg::AX::_lock_type arg)
  {
    msg_.lock = std::move(arg);
    return Init_AX_punch(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_moving
{
public:
  explicit Init_AX_moving(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_lock moving(::dynamixel_workbench_msgs::msg::AX::_moving_type arg)
  {
    msg_.moving = std::move(arg);
    return Init_AX_lock(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_registered
{
public:
  explicit Init_AX_registered(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_moving registered(::dynamixel_workbench_msgs::msg::AX::_registered_type arg)
  {
    msg_.registered = std::move(arg);
    return Init_AX_moving(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_present_temperature
{
public:
  explicit Init_AX_present_temperature(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_registered present_temperature(::dynamixel_workbench_msgs::msg::AX::_present_temperature_type arg)
  {
    msg_.present_temperature = std::move(arg);
    return Init_AX_registered(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_present_voltage
{
public:
  explicit Init_AX_present_voltage(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_present_temperature present_voltage(::dynamixel_workbench_msgs::msg::AX::_present_voltage_type arg)
  {
    msg_.present_voltage = std::move(arg);
    return Init_AX_present_temperature(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_present_load
{
public:
  explicit Init_AX_present_load(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_present_voltage present_load(::dynamixel_workbench_msgs::msg::AX::_present_load_type arg)
  {
    msg_.present_load = std::move(arg);
    return Init_AX_present_voltage(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_present_speed
{
public:
  explicit Init_AX_present_speed(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_present_load present_speed(::dynamixel_workbench_msgs::msg::AX::_present_speed_type arg)
  {
    msg_.present_speed = std::move(arg);
    return Init_AX_present_load(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_present_position
{
public:
  explicit Init_AX_present_position(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_present_speed present_position(::dynamixel_workbench_msgs::msg::AX::_present_position_type arg)
  {
    msg_.present_position = std::move(arg);
    return Init_AX_present_speed(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_torque_limit
{
public:
  explicit Init_AX_torque_limit(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_present_position torque_limit(::dynamixel_workbench_msgs::msg::AX::_torque_limit_type arg)
  {
    msg_.torque_limit = std::move(arg);
    return Init_AX_present_position(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_moving_speed
{
public:
  explicit Init_AX_moving_speed(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_torque_limit moving_speed(::dynamixel_workbench_msgs::msg::AX::_moving_speed_type arg)
  {
    msg_.moving_speed = std::move(arg);
    return Init_AX_torque_limit(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_goal_position
{
public:
  explicit Init_AX_goal_position(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_moving_speed goal_position(::dynamixel_workbench_msgs::msg::AX::_goal_position_type arg)
  {
    msg_.goal_position = std::move(arg);
    return Init_AX_moving_speed(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_ccw_compliance_slope
{
public:
  explicit Init_AX_ccw_compliance_slope(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_goal_position ccw_compliance_slope(::dynamixel_workbench_msgs::msg::AX::_ccw_compliance_slope_type arg)
  {
    msg_.ccw_compliance_slope = std::move(arg);
    return Init_AX_goal_position(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_cw_compliance_slope
{
public:
  explicit Init_AX_cw_compliance_slope(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_ccw_compliance_slope cw_compliance_slope(::dynamixel_workbench_msgs::msg::AX::_cw_compliance_slope_type arg)
  {
    msg_.cw_compliance_slope = std::move(arg);
    return Init_AX_ccw_compliance_slope(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_ccw_compliance_margin
{
public:
  explicit Init_AX_ccw_compliance_margin(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_cw_compliance_slope ccw_compliance_margin(::dynamixel_workbench_msgs::msg::AX::_ccw_compliance_margin_type arg)
  {
    msg_.ccw_compliance_margin = std::move(arg);
    return Init_AX_cw_compliance_slope(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_cw_compliance_margin
{
public:
  explicit Init_AX_cw_compliance_margin(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_ccw_compliance_margin cw_compliance_margin(::dynamixel_workbench_msgs::msg::AX::_cw_compliance_margin_type arg)
  {
    msg_.cw_compliance_margin = std::move(arg);
    return Init_AX_ccw_compliance_margin(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_led
{
public:
  explicit Init_AX_led(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_cw_compliance_margin led(::dynamixel_workbench_msgs::msg::AX::_led_type arg)
  {
    msg_.led = std::move(arg);
    return Init_AX_cw_compliance_margin(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_torque_enable
{
public:
  explicit Init_AX_torque_enable(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_led torque_enable(::dynamixel_workbench_msgs::msg::AX::_torque_enable_type arg)
  {
    msg_.torque_enable = std::move(arg);
    return Init_AX_led(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_shutdown
{
public:
  explicit Init_AX_shutdown(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_torque_enable shutdown(::dynamixel_workbench_msgs::msg::AX::_shutdown_type arg)
  {
    msg_.shutdown = std::move(arg);
    return Init_AX_torque_enable(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_alarm_led
{
public:
  explicit Init_AX_alarm_led(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_shutdown alarm_led(::dynamixel_workbench_msgs::msg::AX::_alarm_led_type arg)
  {
    msg_.alarm_led = std::move(arg);
    return Init_AX_shutdown(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_status_return_level
{
public:
  explicit Init_AX_status_return_level(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_alarm_led status_return_level(::dynamixel_workbench_msgs::msg::AX::_status_return_level_type arg)
  {
    msg_.status_return_level = std::move(arg);
    return Init_AX_alarm_led(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_max_torque
{
public:
  explicit Init_AX_max_torque(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_status_return_level max_torque(::dynamixel_workbench_msgs::msg::AX::_max_torque_type arg)
  {
    msg_.max_torque = std::move(arg);
    return Init_AX_status_return_level(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_max_voltage_limit
{
public:
  explicit Init_AX_max_voltage_limit(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_max_torque max_voltage_limit(::dynamixel_workbench_msgs::msg::AX::_max_voltage_limit_type arg)
  {
    msg_.max_voltage_limit = std::move(arg);
    return Init_AX_max_torque(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_min_voltage_limit
{
public:
  explicit Init_AX_min_voltage_limit(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_max_voltage_limit min_voltage_limit(::dynamixel_workbench_msgs::msg::AX::_min_voltage_limit_type arg)
  {
    msg_.min_voltage_limit = std::move(arg);
    return Init_AX_max_voltage_limit(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_temperature_limit
{
public:
  explicit Init_AX_temperature_limit(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_min_voltage_limit temperature_limit(::dynamixel_workbench_msgs::msg::AX::_temperature_limit_type arg)
  {
    msg_.temperature_limit = std::move(arg);
    return Init_AX_min_voltage_limit(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_ccw_angle_limit
{
public:
  explicit Init_AX_ccw_angle_limit(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_temperature_limit ccw_angle_limit(::dynamixel_workbench_msgs::msg::AX::_ccw_angle_limit_type arg)
  {
    msg_.ccw_angle_limit = std::move(arg);
    return Init_AX_temperature_limit(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_cw_angle_limit
{
public:
  explicit Init_AX_cw_angle_limit(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_ccw_angle_limit cw_angle_limit(::dynamixel_workbench_msgs::msg::AX::_cw_angle_limit_type arg)
  {
    msg_.cw_angle_limit = std::move(arg);
    return Init_AX_ccw_angle_limit(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_return_delay_time
{
public:
  explicit Init_AX_return_delay_time(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_cw_angle_limit return_delay_time(::dynamixel_workbench_msgs::msg::AX::_return_delay_time_type arg)
  {
    msg_.return_delay_time = std::move(arg);
    return Init_AX_cw_angle_limit(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_baud_rate
{
public:
  explicit Init_AX_baud_rate(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_return_delay_time baud_rate(::dynamixel_workbench_msgs::msg::AX::_baud_rate_type arg)
  {
    msg_.baud_rate = std::move(arg);
    return Init_AX_return_delay_time(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_model_id
{
public:
  explicit Init_AX_model_id(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_baud_rate model_id(::dynamixel_workbench_msgs::msg::AX::_model_id_type arg)
  {
    msg_.model_id = std::move(arg);
    return Init_AX_baud_rate(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_firmware_version
{
public:
  explicit Init_AX_firmware_version(::dynamixel_workbench_msgs::msg::AX & msg)
  : msg_(msg)
  {}
  Init_AX_model_id firmware_version(::dynamixel_workbench_msgs::msg::AX::_firmware_version_type arg)
  {
    msg_.firmware_version = std::move(arg);
    return Init_AX_model_id(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

class Init_AX_model_number
{
public:
  Init_AX_model_number()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_AX_firmware_version model_number(::dynamixel_workbench_msgs::msg::AX::_model_number_type arg)
  {
    msg_.model_number = std::move(arg);
    return Init_AX_firmware_version(msg_);
  }

private:
  ::dynamixel_workbench_msgs::msg::AX msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::dynamixel_workbench_msgs::msg::AX>()
{
  return dynamixel_workbench_msgs::msg::builder::Init_AX_model_number();
}

}  // namespace dynamixel_workbench_msgs

#endif  // DYNAMIXEL_WORKBENCH_MSGS__MSG__DETAIL__AX__BUILDER_HPP_
