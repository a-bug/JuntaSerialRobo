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

drivers::serial_driver::SerialBridgeNode sbd;

std::vector<joint> joints_select;

using std_msgs::msg::UInt8MultiArray;


UInt8MultiArray::SharedPtr serial_message;

void write_callback(const sensor_msgs::JointState::ConstPtr &msg)
{
  for (int i = 0; i < juntas_num; i++)
  {
    float rad_position = msg->position[i];
    float var_rad = 0.0;
    float var_steps = 0;
    std::ostringstream stm;
    // a primeira iteração serve para modificar o valor de last_position e evitar problemas com o 0.0
    if (joints_select[i].first_iteration)
    {
      joints_select[i].last_position = rad_position;
      joints_select[i].first_iteration = false;
    }
    else
    {
      // manda sinal ao motor se alterou a posição
      if (rad_position != joints_select[i].last_position)
      {
        // converte de rad para passos considerando 0.01 deg/step ou 0.0001745329 rad/step
        var_rad = rad_position - joints_select[i].last_position;
        var_steps = int(var_rad / 0.0001745329);
        // cria a mensagem a ser mandada pro motor via serial
        stm.str("");
        stm << (i + 1) << "DI" << var_steps << "\r";
        serial_message = stm.str();
        ROS_INFO_STREAM("Writing to serial port: " << serial_message);
        // envia a mensagem
        sbd.subscriber_callback(serial_message);
        ros::Duration(0.01).sleep();
        stm.str("");
        stm << (i + 1) << "FL\r";
        serial_message = stm.str();
        ROS_INFO_STREAM("Writing to serial port: " << serial_message);
        ROS_INFO_STREAM("----------------------------------");
        // envia a mensagem
        sbd.subscriber_callback(serial_message);
        rclcpp::Duration(1).nanoseconds();
        joints_select[i].last_position = rad_position;
      }
    }
  }
}