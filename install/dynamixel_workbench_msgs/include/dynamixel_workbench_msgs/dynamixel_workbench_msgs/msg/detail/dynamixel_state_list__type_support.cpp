// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from dynamixel_workbench_msgs:msg/DynamixelStateList.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "dynamixel_workbench_msgs/msg/detail/dynamixel_state_list__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace dynamixel_workbench_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void DynamixelStateList_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) dynamixel_workbench_msgs::msg::DynamixelStateList(_init);
}

void DynamixelStateList_fini_function(void * message_memory)
{
  auto typed_message = static_cast<dynamixel_workbench_msgs::msg::DynamixelStateList *>(message_memory);
  typed_message->~DynamixelStateList();
}

size_t size_function__DynamixelStateList__dynamixel_state(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<dynamixel_workbench_msgs::msg::DynamixelState> *>(untyped_member);
  return member->size();
}

const void * get_const_function__DynamixelStateList__dynamixel_state(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<dynamixel_workbench_msgs::msg::DynamixelState> *>(untyped_member);
  return &member[index];
}

void * get_function__DynamixelStateList__dynamixel_state(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<dynamixel_workbench_msgs::msg::DynamixelState> *>(untyped_member);
  return &member[index];
}

void fetch_function__DynamixelStateList__dynamixel_state(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const dynamixel_workbench_msgs::msg::DynamixelState *>(
    get_const_function__DynamixelStateList__dynamixel_state(untyped_member, index));
  auto & value = *reinterpret_cast<dynamixel_workbench_msgs::msg::DynamixelState *>(untyped_value);
  value = item;
}

void assign_function__DynamixelStateList__dynamixel_state(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<dynamixel_workbench_msgs::msg::DynamixelState *>(
    get_function__DynamixelStateList__dynamixel_state(untyped_member, index));
  const auto & value = *reinterpret_cast<const dynamixel_workbench_msgs::msg::DynamixelState *>(untyped_value);
  item = value;
}

void resize_function__DynamixelStateList__dynamixel_state(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<dynamixel_workbench_msgs::msg::DynamixelState> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember DynamixelStateList_message_member_array[1] = {
  {
    "dynamixel_state",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<dynamixel_workbench_msgs::msg::DynamixelState>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(dynamixel_workbench_msgs::msg::DynamixelStateList, dynamixel_state),  // bytes offset in struct
    nullptr,  // default value
    size_function__DynamixelStateList__dynamixel_state,  // size() function pointer
    get_const_function__DynamixelStateList__dynamixel_state,  // get_const(index) function pointer
    get_function__DynamixelStateList__dynamixel_state,  // get(index) function pointer
    fetch_function__DynamixelStateList__dynamixel_state,  // fetch(index, &value) function pointer
    assign_function__DynamixelStateList__dynamixel_state,  // assign(index, value) function pointer
    resize_function__DynamixelStateList__dynamixel_state  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers DynamixelStateList_message_members = {
  "dynamixel_workbench_msgs::msg",  // message namespace
  "DynamixelStateList",  // message name
  1,  // number of fields
  sizeof(dynamixel_workbench_msgs::msg::DynamixelStateList),
  DynamixelStateList_message_member_array,  // message members
  DynamixelStateList_init_function,  // function to initialize message memory (memory has to be allocated)
  DynamixelStateList_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t DynamixelStateList_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &DynamixelStateList_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace dynamixel_workbench_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<dynamixel_workbench_msgs::msg::DynamixelStateList>()
{
  return &::dynamixel_workbench_msgs::msg::rosidl_typesupport_introspection_cpp::DynamixelStateList_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, dynamixel_workbench_msgs, msg, DynamixelStateList)() {
  return &::dynamixel_workbench_msgs::msg::rosidl_typesupport_introspection_cpp::DynamixelStateList_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
