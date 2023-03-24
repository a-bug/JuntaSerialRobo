// Autoria de Michael
// Com pedaços de código de PVD



#include <rclcpp/rclcpp.hpp>
//#include <serial/serial.h>
#include <serial_driver/serial_bridge_node.hpp>
#include <std_msgs/msg/string.h>
#include <std_msgs/msg/empty.h>
#include <std_msgs/msg/float64.h>
#include <sensor_msgs/msg/joint_state.h>

#include <string>
#include <sstream>
#include <vector>

#define juntas_num 6

class joint
{
public:
  joint()
  {
    last_position = 0.0;
    first_iteration = true;
  }
  ~joint() {}

public:
  float last_position;
  bool first_iteration;
};

//serial::Serial ser;

drivers::serial_driver::SerialPort sp;



std::vector<joint> joints_select;

using std_msgs::msg::UInt8MultiArray;

UInt8MultiArray::SharedPtr serial_message;


