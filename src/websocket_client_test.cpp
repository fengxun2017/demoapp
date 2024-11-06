#include <sstream>
#include <iomanip> // 包含用于格式化输出的头文件
#include <thread>
#include <chrono>
#include "ws_client.h"
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

void client_thread(tinynet::WsConnPtr &ws_conn, int index)
{
    uint8_t test_data[][5] = {{11,12,13,14,15}, {1,2,3,4,5}};
    uint8_t segment1[4] = {1,1,1,1};
    uint8_t segment2[4] = {2,2,2,2};
    uint8_t segment3[4] = {3,3,3,3};

    while (1)
    {
        if (nullptr != ws_conn)
        {
            if(index==2)
            {
                LOG(DEBUG) << ws_conn->get_name() << " send segment1 :" << display_data(segment1, 4).str() << std::endl;
                ws_conn->write_data(segment1, 4, tinynet::WebSocket::OPCODE_TEXT, false);
                LOG(DEBUG) << ws_conn->get_name() << " send segment2 :" << display_data(segment2, 4).str() << std::endl;
                ws_conn->write_data(segment2, 4, tinynet::WebSocket::OPCODE_CONT, false);
                LOG(DEBUG) << ws_conn->get_name() << " send segment3 :" << display_data(segment3, 4).str() << std::endl;
                ws_conn->write_data(segment3, 4, tinynet::WebSocket::OPCODE_CONT, true);
            }
            else
            {
                LOG(DEBUG) << ws_conn->get_name() << " send data :" << display_data(test_data[index%2], 5).str() << std::endl;
                ws_conn->write_data(test_data[index%2], 5);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}

void new_conn_cb(tinynet::WsConnPtr &ws_conn)
{
    static int index = 0;
    LOG(DEBUG) << "demo_ws_client: new conn " << ws_conn->get_name() << std::endl;
    std::thread t(client_thread, std::ref(ws_conn), index++);
    t.detach();
}

void disconnected_cb(tinynet::WsConnPtr &ws_conn)
{
    LOG(DEBUG) << "demo_ws_client: " << ws_conn->get_name() << " disconnected" << std::endl;
}

void on_message_cb(tinynet::WsConnPtr &ws_conn, const uint8_t *data, size_t size)
{
    std::ostringstream oss;
    LOG(DEBUG) <<"demo_ws_client: " << ws_conn->get_name() << " recv data:" << display_data(data, size).str() << std::endl;
}

int main(void)
{
    tinynet::EventLoop event_loop;
    tinynet::WsClient client(&event_loop, "ws_client");
    client.set_newconn_cb(new_conn_cb);
    client.set_onmessage_cb(on_message_cb);
    client.set_disconnected_cb(disconnected_cb);
    client.connect("192.168.56.103", 8070);

    tinynet::WsClient client2(&event_loop, "ws_client2");
    client2.set_newconn_cb(new_conn_cb);
    client2.set_onmessage_cb(on_message_cb);
    client2.set_disconnected_cb(disconnected_cb);
    client2.connect("192.168.56.103", 8070);

    tinynet::WsClient client3(&event_loop, "ws_client3");
    client3.set_newconn_cb(new_conn_cb);
    client3.set_onmessage_cb(on_message_cb);
    client3.set_disconnected_cb(disconnected_cb);
    client3.connect("192.168.56.103", 8070);

    event_loop.loop();
    return 0;
}