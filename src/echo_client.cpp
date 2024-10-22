#include <sstream>
#include <iomanip> // 包含用于格式化输出的头文件
#include <string>
#include <thread>
#include "logging.h"
#include "tcp_client.h"
#include "event_loop.h"

struct DefaultFormatState {
    std::ios_base::fmtflags flags;
    std::streamsize precision;
};
DefaultFormatState save_default_format(std::ostringstream &oss) {
    return { oss.flags(), oss.precision() };
}

void restore_default_format(std::ostringstream &oss, const DefaultFormatState& state) {
    oss.flags(state.flags);
    oss.precision(state.precision);
}

std::ostringstream display_data(const uint8_t *data, uint32_t len) {
    std::ostringstream oss;

    DefaultFormatState defaultState = save_default_format(oss);

    // 修改输出格式为十六进制
    for (uint32_t i = 0; i < len; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]) << " ";
    }
    oss << std::endl;

    // 在函数退出时恢复默认的输出格式
    restore_default_format(oss, defaultState);
    return oss;
}

tinynet::TcpConnPtr server_conn = nullptr;
void new_conn_cb(tinynet::TcpConnPtr &conn)
{
    LOG(DEBUG) << "echo_client: new conn " << conn->get_name() << std::endl;

    server_conn = conn;
}

void disconnected_cb(tinynet::TcpConnPtr &conn)
{
    LOG(DEBUG) << "echo_client: " << conn->get_name() << "disconnected" << std::endl;
}

void on_message_cb(tinynet::TcpConnPtr &conn, const uint8_t *data, size_t size)
{
    std::ostringstream oss;
    LOG(DEBUG) <<"echo_client: " << conn->get_name() << " recv data:" << display_data(data, size).str() << std::endl;
}

void client_thread(void)
{
    uint8_t test_data[] = {1,2,3,4,5};

    LOG(DEBUG) << "start client_thread" << std::endl;
    while (1)
    {
        if (nullptr != server_conn)
        {
            server_conn->write_data(test_data, sizeof(test_data));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}

int main(void)
{
    tinynet::EventLoop event_loop;
    tinynet::TcpClient tcp_client(&event_loop, "echo_client");
    tcp_client.set_newconn_cb(new_conn_cb);
    tcp_client.set_onmessage_cb(on_message_cb);
    tcp_client.set_disconnected_cb(disconnected_cb);
    tcp_client.connect("127.0.0.1", 14000);


    std::thread t1 = std::thread(client_thread);

    event_loop.loop();

    return 0;
}