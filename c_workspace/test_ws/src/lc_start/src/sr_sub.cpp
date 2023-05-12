//Subscriber for read_serial

#include <memory>
#include <iostream>

#include "rcl/types.h"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/serialization.hpp"
#include "rclcpp_components/register_node_macro.hpp"

#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int8_multi_array.hpp"

#include "rosidl_typesupport_cpp/message_type_support.hpp"

class Subscriber : public rclcpp::Node
{
  public:
    Subscriber()
    : Node("listener")
    {
      setvbuf(stdout, NULL, _IONBF, BUFSIZ);
      auto callback =
      [](const std_msgs::msg::UInt8MultiArray::SharedPtr msg) -> void
      {
        // Print the serialized data message in HEX representation
        // This output corresponds to what you would see in e.g. Wireshark
        // when tracing the RTPS packets.
        
        std::cout << "I heard data of length: " << msg->size() << std::endl;
        for (size_t i = 0; i < msg->size(); ++i) {
          printf("%02x ", msg->get_rcl_serialized_message().buffer[i]);
        }
        printf("\n");

        // In order to deserialize the message we have to manually create a ROS 2
        // message in which we want to convert the serialized data.
        
        // using MessageT = std_msgs::msg::String;
        // MessageT string_msg;
        // auto serializer = rclcpp::Serialization<MessageT>();
        // serializer.deserialize_message(msg.get(), &string_msg);
        // Finally print the ROS 2 message data
        
        // std::cout << "serialized data after deserialization: " << string_msg.data << std::endl;
        
        
      };

      sub_ = create_subscription<std_msgs::msg::UInt8MultiArray>("serial_read", 10, callback);
    }
  private:
    rclcpp::Subscription<std_msgs::msg::UInt8MultiArray>::SharedPtr sub_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Subscriber>());
  rclcpp::shutdown();
  return 0;
}
