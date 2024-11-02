#include <sstream>
#include <iomanip> // 包含用于格式化输出的头文件
#include "ws_server.h"
#include "event_loop.h"
#include "logging.h"

std::ostringstream display_data(const uint8_t *data, uint32_t len) {
    std::ostringstream oss;
    for (uint32_t i = 0; i < len; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]) << " ";
    }
    return oss;
}

void new_conn_cb(tinynet::TcpConnPtr &conn)
{
    LOG(DEBUG) << "echo_server: new conn " << conn->get_name() << std::endl;
}

void disconnected_cb(tinynet::TcpConnPtr &conn)
{
    LOG(DEBUG) << "echo_server: " << conn->get_name() << " disconnected" << std::endl;
}

void on_message_cb(tinynet::TcpConnPtr &conn, const uint8_t *data, size_t size)
{
    std::ostringstream oss;
    LOG(DEBUG) <<"echo_server: " << conn->get_name() << " recv data:" << display_data(data, size).str() << std::endl;

    conn->write_data(data, size);
}

int main(void)
{
    tinynet::EventLoop event_loop;
    tinynet::WebSocketServer server(&event_loop, "192.168.56.103", 8070, "websocket_server");
    server.set_newconn_cb(new_conn_cb);
    server.set_onmessage_cb(on_message_cb);
    server.set_disconnected_cb(disconnected_cb);
    server.start();
    event_loop.loop();

    return 0;
}