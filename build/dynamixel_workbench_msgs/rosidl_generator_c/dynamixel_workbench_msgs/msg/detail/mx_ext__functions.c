// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from dynamixel_workbench_msgs:msg/MXExt.idl
// generated code does not contain a copyright notice
#include "dynamixel_workbench_msgs/msg/detail/mx_ext__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
dynamixel_workbench_msgs__msg__MXExt__init(dynamixel_workbench_msgs__msg__MXExt * msg)
{
  if (!msg) {
    return false;
  }
  // model_number
  // firmware_version
  // model_id
  // baud_rate
  // return_delay_time
  // cw_angle_limit
  // ccw_angle_limit
  // temperature_limit
  // min_voltage_limit
  // max_voltage_limit
  // max_torque
  // status_return_level
  // alarm_led
  // shutdown
  // multi_turn_offset
  // resolution_divider
  // torque_enable
  // led
  // d_gain
  // i_gain
  // p_gain
  // goal_position
  // moving_speed
  // torque_limit
  // present_position
  // present_speed
  // present_load
  // present_voltage
  // present_temperature
  // registered
  // moving
  // lock
  // punch
  // current
  // torque_control_mode_enable
  // goal_torque
  // goal_acceleration
  return true;
}

void
dynamixel_workbench_msgs__msg__MXExt__fini(dynamixel_workbench_msgs__msg__MXExt * msg)
{
  if (!msg) {
    return;
  }
  // model_number
  // firmware_version
  // model_id
  // baud_rate
  // return_delay_time
  // cw_angle_limit
  // ccw_angle_limit
  // temperature_limit
  // min_voltage_limit
  // max_voltage_limit
  // max_torque
  // status_return_level
  // alarm_led
  // shutdown
  // multi_turn_offset
  // resolution_divider
  // torque_enable
  // led
  // d_gain
  // i_gain
  // p_gain
  // goal_position
  // moving_speed
  // torque_limit
  // present_position
  // present_speed
  // present_load
  // present_voltage
  // present_temperature
  // registered
  // moving
  // lock
  // punch
  // current
  // torque_control_mode_enable
  // goal_torque
  // goal_acceleration
}

bool
dynamixel_workbench_msgs__msg__MXExt__are_equal(const dynamixel_workbench_msgs__msg__MXExt * lhs, const dynamixel_workbench_msgs__msg__MXExt * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // model_number
  if (lhs->model_number != rhs->model_number) {
    return false;
  }
  // firmware_version
  if (lhs->firmware_version != rhs->firmware_version) {
    return false;
  }
  // model_id
  if (lhs->model_id != rhs->model_id) {
    return false;
  }
  // baud_rate
  if (lhs->baud_rate != rhs->baud_rate) {
    return false;
  }
  // return_delay_time
  if (lhs->return_delay_time != rhs->return_delay_time) {
    return false;
  }
  // cw_angle_limit
  if (lhs->cw_angle_limit != rhs->cw_angle_limit) {
    return false;
  }
  // ccw_angle_limit
  if (lhs->ccw_angle_limit != rhs->ccw_angle_limit) {
    return false;
  }
  // temperature_limit
  if (lhs->temperature_limit != rhs->temperature_limit) {
    return false;
  }
  // min_voltage_limit
  if (lhs->min_voltage_limit != rhs->min_voltage_limit) {
    return false;
  }
  // max_voltage_limit
  if (lhs->max_voltage_limit != rhs->max_voltage_limit) {
    return false;
  }
  // max_torque
  if (lhs->max_torque != rhs->max_torque) {
    return false;
  }
  // status_return_level
  if (lhs->status_return_level != rhs->status_return_level) {
    return false;
  }
  // alarm_led
  if (lhs->alarm_led != rhs->alarm_led) {
    return false;
  }
  // shutdown
  if (lhs->shutdown != rhs->shutdown) {
    return false;
  }
  // multi_turn_offset
  if (lhs->multi_turn_offset != rhs->multi_turn_offset) {
    return false;
  }
  // resolution_divider
  if (lhs->resolution_divider != rhs->resolution_divider) {
    return false;
  }
  // torque_enable
  if (lhs->torque_enable != rhs->torque_enable) {
    return false;
  }
  // led
  if (lhs->led != rhs->led) {
    return false;
  }
  // d_gain
  if (lhs->d_gain != rhs->d_gain) {
    return false;
  }
  // i_gain
  if (lhs->i_gain != rhs->i_gain) {
    return false;
  }
  // p_gain
  if (lhs->p_gain != rhs->p_gain) {
    return false;
  }
  // goal_position
  if (lhs->goal_position != rhs->goal_position) {
    return false;
  }
  // moving_speed
  if (lhs->moving_speed != rhs->moving_speed) {
    return false;
  }
  // torque_limit
  if (lhs->torque_limit != rhs->torque_limit) {
    return false;
  }
  // present_position
  if (lhs->present_position != rhs->present_position) {
    return false;
  }
  // present_speed
  if (lhs->present_speed != rhs->present_speed) {
    return false;
  }
  // present_load
  if (lhs->present_load != rhs->present_load) {
    return false;
  }
  // present_voltage
  if (lhs->present_voltage != rhs->present_voltage) {
    return false;
  }
  // present_temperature
  if (lhs->present_temperature != rhs->present_temperature) {
    return false;
  }
  // registered
  if (lhs->registered != rhs->registered) {
    return false;
  }
  // moving
  if (lhs->moving != rhs->moving) {
    return false;
  }
  // lock
  if (lhs->lock != rhs->lock) {
    return false;
  }
  // punch
  if (lhs->punch != rhs->punch) {
    return false;
  }
  // current
  if (lhs->current != rhs->current) {
    return false;
  }
  // torque_control_mode_enable
  if (lhs->torque_control_mode_enable != rhs->torque_control_mode_enable) {
    return false;
  }
  // goal_torque
  if (lhs->goal_torque != rhs->goal_torque) {
    return false;
  }
  // goal_acceleration
  if (lhs->goal_acceleration != rhs->goal_acceleration) {
    return false;
  }
  return true;
}

bool
dynamixel_workbench_msgs__msg__MXExt__copy(
  const dynamixel_workbench_msgs__msg__MXExt * input,
  dynamixel_workbench_msgs__msg__MXExt * output)
{
  if (!input || !output) {
    return false;
  }
  // model_number
  output->model_number = input->model_number;
  // firmware_version
  output->firmware_version = input->firmware_version;
  // model_id
  output->model_id = input->model_id;
  // baud_rate
  output->baud_rate = input->baud_rate;
  // return_delay_time
  output->return_delay_time = input->return_delay_time;
  // cw_angle_limit
  output->cw_angle_limit = input->cw_angle_limit;
  // ccw_angle_limit
  output->ccw_angle_limit = input->ccw_angle_limit;
  // temperature_limit
  output->temperature_limit = input->temperature_limit;
  // min_voltage_limit
  output->min_voltage_limit = input->min_voltage_limit;
  // max_voltage_limit
  output->max_voltage_limit = input->max_voltage_limit;
  // max_torque
  output->max_torque = input->max_torque;
  // status_return_level
  output->status_return_level = input->status_return_level;
  // alarm_led
  output->alarm_led = input->alarm_led;
  // shutdown
  output->shutdown = input->shutdown;
  // multi_turn_offset
  output->multi_turn_offset = input->multi_turn_offset;
  // resolution_divider
  output->resolution_divider = input->resolution_divider;
  // torque_enable
  output->torque_enable = input->torque_enable;
  // led
  output->led = input->led;
  // d_gain
  output->d_gain = input->d_gain;
  // i_gain
  output->i_gain = input->i_gain;
  // p_gain
  output->p_gain = input->p_gain;
  // goal_position
  output->goal_position = input->goal_position;
  // moving_speed
  output->moving_speed = input->moving_speed;
  // torque_limit
  output->torque_limit = input->torque_limit;
  // present_position
  output->present_position = input->present_position;
  // present_speed
  output->present_speed = input->present_speed;
  // present_load
  output->present_load = input->present_load;
  // present_voltage
  output->present_voltage = input->present_voltage;
  // present_temperature
  output->present_temperature = input->present_temperature;
  // registered
  output->registered = input->registered;
  // moving
  output->moving = input->moving;
  // lock
  output->lock = input->lock;
  // punch
  output->punch = input->punch;
  // current
  output->current = input->current;
  // torque_control_mode_enable
  output->torque_control_mode_enable = input->torque_control_mode_enable;
  // goal_torque
  output->goal_torque = input->goal_torque;
  // goal_acceleration
  output->goal_acceleration = input->goal_acceleration;
  return true;
}

dynamixel_workbench_msgs__msg__MXExt *
dynamixel_workbench_msgs__msg__MXExt__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  dynamixel_workbench_msgs__msg__MXExt * msg = (dynamixel_workbench_msgs__msg__MXExt *)allocator.allocate(sizeof(dynamixel_workbench_msgs__msg__MXExt), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(dynamixel_workbench_msgs__msg__MXExt));
  bool success = dynamixel_workbench_msgs__msg__MXExt__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
dynamixel_workbench_msgs__msg__MXExt__destroy(dynamixel_workbench_msgs__msg__MXExt * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    dynamixel_workbench_msgs__msg__MXExt__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
dynamixel_workbench_msgs__msg__MXExt__Sequence__init(dynamixel_workbench_msgs__msg__MXExt__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  dynamixel_workbench_msgs__msg__MXExt * data = NULL;

  if (size) {
    data = (dynamixel_workbench_msgs__msg__MXExt *)allocator.zero_allocate(size, sizeof(dynamixel_workbench_msgs__msg__MXExt), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = dynamixel_workbench_msgs__msg__MXExt__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        dynamixel_workbench_msgs__msg__MXExt__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
dynamixel_workbench_msgs__msg__MXExt__Sequence__fini(dynamixel_workbench_msgs__msg__MXExt__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      dynamixel_workbench_msgs__msg__MXExt__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

dynamixel_workbench_msgs__msg__MXExt__Sequence *
dynamixel_workbench_msgs__msg__MXExt__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  dynamixel_workbench_msgs__msg__MXExt__Sequence * array = (dynamixel_workbench_msgs__msg__MXExt__Sequence *)allocator.allocate(sizeof(dynamixel_workbench_msgs__msg__MXExt__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = dynamixel_workbench_msgs__msg__MXExt__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
dynamixel_workbench_msgs__msg__MXExt__Sequence__destroy(dynamixel_workbench_msgs__msg__MXExt__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    dynamixel_workbench_msgs__msg__MXExt__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
dynamixel_workbench_msgs__msg__MXExt__Sequence__are_equal(const dynamixel_workbench_msgs__msg__MXExt__Sequence * lhs, const dynamixel_workbench_msgs__msg__MXExt__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!dynamixel_workbench_msgs__msg__MXExt__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
dynamixel_workbench_msgs__msg__MXExt__Sequence__copy(
  const dynamixel_workbench_msgs__msg__MXExt__Sequence * input,
  dynamixel_workbench_msgs__msg__MXExt__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(dynamixel_workbench_msgs__msg__MXExt);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    dynamixel_workbench_msgs__msg__MXExt * data =
      (dynamixel_workbench_msgs__msg__MXExt *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!dynamixel_workbench_msgs__msg__MXExt__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          dynamixel_workbench_msgs__msg__MXExt__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!dynamixel_workbench_msgs__msg__MXExt__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
