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

void new_conn_cb(tinynet::TcpConnection &conn)
{
    LOG(DEBUG) << "demoapp: new conn " << conn.get_name() << std::endl;
}

void on_message_cb(tinynet::TcpConnection &conn, const uint8_t *data, size_t size)
{
    std::ostringstream oss;

    LOG(DEBUG) <<"demp app: " conn.get_name() << " recv data:" << display_data(data, size).str() << std::endl;
}

int main(void)
{
    tinynet::EventLoop event_loop;
    tinynet::TcpServer tcp_server(&event_loop, "127.0.0.1", 14000, "test_server");
    tcp_server.set_newconn_cb(new_conn_cb);
    tcp_server.set_onmessage_cb(on_message_cb);
    tcp_server.start();
    event_loop.loop();

    return 0;
}