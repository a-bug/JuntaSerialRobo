
from example_interfaces.srv import AddTwoInts
from rcl_interfaces.msg import SetParametersResult

import rclpy
from rclpy.executors import ExternalShutdownException
from rclpy.executors import SingleThreadedExecutor
from rclpy.node import Node
from rclpy.parameter import Parameter
from rclpy.qos import qos_profile_system_default
from rclpy.service_introspection import ServiceIntrospectionState


def check_parameter(parameter_list, parameter_name):
    result = SetParametersResult()
    result.successful = True
    for param in parameter_list:
        if param.name != parameter_name:
            continue

        if param.type_ != Parameter.Type.STRING:
            result.successful = False
            result.reason = 'must be a string'
            break

        if param.value not in ('off', 'metadata', 'contents'):
            result.successful = False
            result.reason = "must be one of 'off', 'metadata', or 'contents"
            break

    return result


class IntrospectionClientNode(Node):

    def on_set_parameters_callback(self, parameter_list):
        return check_parameter(parameter_list, 'client_configure_introspection')

    def on_post_set_parameters_callback(self, parameter_list):
        for param in parameter_list:
            if param.name != 'client_configure_introspection':
                continue

            introspection_state = ServiceIntrospectionState.OFF
            if param.value == 'off':
                introspection_state = ServiceIntrospectionState.OFF
            elif param.value == 'metadata':
                introspection_state = ServiceIntrospectionState.METADATA
            elif param.value == 'contents':
                introspection_state = ServiceIntrospectionState.CONTENTS

            self.cli.configure_introspection(self.get_clock(), qos_profile_system_default,
                                             introspection_state)
            break

    def __init__(self):
        super().__init__('introspection_client')

        self.cli = self.create_client(AddTwoInts, 'add_two_ints')

        self.add_on_set_parameters_callback(self.on_set_parameters_callback)
        self.add_post_set_parameters_callback(self.on_post_set_parameters_callback)
        self.declare_parameter('client_configure_introspection', 'off')

        self.timer = self.create_timer(0.5, self.timer_callback)
        self.future = None

    def timer_callback(self):
        if not self.cli.service_is_ready():
            return

        if self.future is None:
            req = AddTwoInts.Request()
            req.a = 2
            req.b = 3

            self.future = self.cli.call_async(req)

            return

        if not self.future.done():
            return

        if self.future.result() is not None:
            self.get_logger().info('Result of add_two_ints: %d' % self.future.result().sum)
        else:
            self.get_logger().error('Exception calling service: %r' % self.future.exception())

        self.future = None


class IntrospectionServiceNode(Node):

    def on_set_parameters_callback(self, parameter_list):
        return check_parameter(parameter_list, 'service_configure_introspection')

    def on_post_set_parameters_callback(self, parameter_list):
        for param in parameter_list:
            if param.name != 'service_configure_introspection':
                continue

            introspection_state = ServiceIntrospectionState.OFF
            if param.value == 'off':
                introspection_state = ServiceIntrospectionState.OFF
            elif param.value == 'metadata':
                introspection_state = ServiceIntrospectionState.METADATA
            elif param.value == 'contents':
                introspection_state = ServiceIntrospectionState.CONTENTS

            self.srv.configure_introspection(self.get_clock(), qos_profile_system_default,
                                             introspection_state)
            break

    def __init__(self):
        super().__init__('introspection_service')

        self.srv = self.create_service(AddTwoInts, 'add_two_ints', self.add_two_ints_callback)

        self.add_on_set_parameters_callback(self.on_set_parameters_callback)
        self.add_post_set_parameters_callback(self.on_post_set_parameters_callback)
        self.declare_parameter('service_configure_introspection', 'off')

    def add_two_ints_callback(self, request, response):
        response.sum = request.a + request.b
        self.get_logger().info('Incoming request\na: %d b: %d' % (request.a, request.b))

        return response


def main(args=None):
    rclpy.init(args=args)

    service_node = IntrospectionServiceNode()

    client_node = IntrospectionClientNode()

    executor = SingleThreadedExecutor()
    executor.add_node(service_node)
    executor.add_node(client_node)

    try:
        executor.spin()
    except (KeyboardInterrupt, ExternalShutdownException):
        executor.remove_node(client_node)
        executor.remove_node(service_node)
        executor.shutdown()
        service_node.destroy_node()
        client_node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()