#include <iostream>
#include <amqpcpp.h>
#include "conn_handler.h"

int
main(void)
{
    auto evbase = event_base_new();
    LibEventHandlerMyError hndl(evbase);

    //AMQP::TcpConnection connection(&hndl, AMQP::Address("amqp://admin:Dynatrace@ec2-52-210-238-117.eu-west-1.compute.amazonaws.com:5672"));
	AMQP::TcpConnection connection(&hndl, AMQP::Address("amqp://userxxx:passxxx@rabbitmq:5672"));
    AMQP::TcpChannel channel(&connection);
    channel.onError([&evbase](const char* message)
        {
            std::cout << "Channel error: " << message << std::endl;
            event_base_loopbreak(evbase);
        });
    channel.declareQueue("hello", AMQP::passive)
        .onSuccess
        (
            [&connection](const std::string &name,
                          uint32_t messagecount,
                          uint32_t consumercount)
            {
                std::cout << "Queue: " << name << std::endl;
            }
        )
        .onFinalize
        (
            [&connection]()
            {
                std::cout << "Finalize." << std::endl;
                connection.close();
            }
        );
    channel.publish("", "hello", "Hello, world!");

    event_base_dispatch(evbase);
    event_base_free(evbase);

    return 0;
}
