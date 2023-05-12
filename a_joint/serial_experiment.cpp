// Autoria de Michael
// Com pedaços de código de PVD



#include <rclcpp/rclcpp.hpp>
//#include <serial/serial.h>
#include "serial_driver/serial_driver.hpp"
#include <std_msgs/msg/string.h>
#include <std_msgs/msg/empty.h>
#include <std_msgs/msg/float64.h>
#include <sensor_msgs/msg/joint_state.h>

#include <string>
#include <sstream>
#include <vector>

#define juntas_num 6

using drivers::serial_driver::FlowControl;
using drivers::serial_driver::Parity;
using drivers::serial_driver::SerialDriver;
using drivers::serial_driver::SerialPortConfig;
using drivers::serial_driver::StopBits;

static constexpr const char * dev_name = "/dev/ttyS0";
static constexpr uint32_t baud = 9600;
static constexpr FlowControl fc = FlowControl::NONE;
static constexpr Parity pt = Parity::NONE;
static constexpr StopBits sb = StopBits::ONE;

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


std::vector<joint> joints_select;

using std_msgs::msg::UInt8MultiArray;

UInt8MultiArray::SharedPtr serial_message;


int main())
{
  IoContext ctx;
  SerialPortConfig config(baud, fc, pt, sb);
  SerialDriver driver(ctx);

  driver.init_port(dev_name, config);

}

