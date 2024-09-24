#include "logging.h"
#include "tcp_server.h"
#include "event_loop.h"


void new_conn_cb(tinynet::TcpConnection &conn)
{
    LOG(DEBUG) << "new conn:" << conn.get_name() << std::endl;
}

int main(void)
{
    tinynet::EventLoop event_loop;
    tinynet::TcpServer tcp_server(&event_loop, "127.0.0.1", 14000, "test_server");

    tcp_server.start();
    tcp_server.set_newconn_cb(new_conn_cb);
    event_loop.loop();

    return 0;
}