
#include <string>
#include <thread>
#include "can.h"
#include "logging.h"

static bool g_wakeup = false;

void enable_wakeup(void)
{
    g_wakeup = true;
}

void disable_wakeup(void)
{
    g_wakeup = false;
}

bool check_wakeup(void)
{
    return g_wakeup;
}

void wakeup_thread(void)
{
    enable_wakeup();

    tinynet::Can can("can_ccc_001");
    if (true == can.bind("can_ccc_001"))
    {
        uint8_t wakeup_msg[8] = {0x08, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        uint32_t wakeup_canid = 0x508;
        while(1)
        {
            if (check_wakeup())
            {
                LOG(DEBUG) << "send wakeup msg" << std::endl;
                can.write_data(wakeup_canid, wakeup_msg, sizeof(wakeup_msg));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    else {
        LOG(ERROR) << "can bind failed" << std::endl;
    }
}

int main(void)
{
    LOG(DEBUG) << "start can_wakeup" << std::endl;
    std::thread t1 = std::thread(wakeup_thread);
    
    t1.join();

    return 0;
}