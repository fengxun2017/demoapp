#include <sstream>
#include <iomanip> // 包含用于格式化输出的头文件
#include <string>
#include "logging.h"
#include "tcp_server.h"
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

    // DefaultFormatState defaultState = save_default_format(oss);

    // output hex
    for (uint32_t i = 0; i < len; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]) << " ";
    }

    // reset format
    // restore_default_format(oss, defaultState);
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
    tinynet::TcpServer tcp_server(&event_loop, "127.0.0.1", 14000, "echo_server");
    tcp_server.set_newconn_cb(new_conn_cb);
    tcp_server.set_onmessage_cb(on_message_cb);
    tcp_server.set_disconnected_cb(disconnected_cb);
    tcp_server.set_worker_thread_num(2);
    tcp_server.start();

    tinynet::TcpServer tcp_server2(&event_loop, "127.0.0.1", 14001, "echo_server2");
    tcp_server2.set_newconn_cb(new_conn_cb);
    tcp_server2.set_onmessage_cb(on_message_cb);
    tcp_server2.set_disconnected_cb(disconnected_cb);
    tcp_server2.start();

    event_loop.loop();

    return 0;
}