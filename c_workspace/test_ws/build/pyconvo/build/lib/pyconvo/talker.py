import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from std_msgs.msg import UInt8MultiArray


class Talker(Node):

    def __init__(self):
        super().__init__('talker')
        self.publisher_ = self.create_publisher(UInt8MultiArray, 'serial_write', 10)
        self.callback

    def callback(self):
        msg = UInt8MultiArray()
        msg.data = [72, 82, 13, 83, 75, 13, 77, 86, 13, 82, 76, 115, 13, 69, 71, 13, 73, 70, 13, 80, 82, 13, 80, 82, 53, 13, 80, 77, 13, 82, 86, 13, 83, 82, 45, 49, 13, 83, 86, 13, 83, 82, 49, 48, 48, 13, 83, 86]
        pub=True
        if(pub):
            self.publisher_.publish(msg)
            pub=False

def main(args=None):
    rclpy.init(args=args)

    talker = Talker()

    rclpy.spin(talker)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    talker.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
