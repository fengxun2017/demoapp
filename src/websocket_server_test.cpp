#include <sstream>
#include <iomanip> // 包含用于格式化输出的头文件
#include "ws_server.h"
#include "ws_connection.h"
#include "event_loop.h"
#include "logging.h"

std::ostringstream display_data(const uint8_t *data, uint32_t len) {
    std::ostringstream oss;
    for (uint32_t i = 0; i < len; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]) << " ";
    }
    return oss;
}

void new_conn_cb(tinynet::WsConnPtr &ws_conn)
{
    LOG(DEBUG) << "demo_websocket_server: new conn " << ws_conn->get_name() << std::endl;
}

void disconnected_cb(tinynet::WsConnPtr &ws_conn)
{
    LOG(DEBUG) << "demo_websocket_server: " << ws_conn->get_name() << " disconnected" << std::endl;
}

void on_message_cb(tinynet::WsConnPtr &ws_conn, const uint8_t *data, size_t size)
{
    std::ostringstream oss;
    LOG(DEBUG) <<"demo_websocket_server: " << ws_conn->get_name() << " recv data:" << display_data(data, size).str() << std::endl;

    ws_conn->write_data(data, size);
}

int main(void)
{
    tinynet::EventLoop event_loop;
    tinynet::WebSocketServer server(&event_loop, "127.0.0.1", 8070, "demo_websocket_server");
    server.set_newconn_cb(new_conn_cb);
    server.set_onmessage_cb(on_message_cb);
    server.set_disconnected_cb(disconnected_cb);
    server.start();
    event_loop.loop();

    return 0;
}