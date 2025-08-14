#include <unistd.h>
#include "sip/SipHandle.hpp"

int main()
{
    bool test = false;
    // 初始化
    sip::init();
    int i = 0;
    while (1)
    {
        i++;
        sleep(1);
        if (i == 5)
        {
            // 3s后测试呼叫
            std::map<std::string, std::string> headers;
            // 这里呼叫多个
            sip::call("test2", headers);
            // sip::callIP("10.200.0.6", headers);
        }

        // if (i % 2 == 0)
        // {
        //     if (!test)
        //     {
        //         printf("---------------------------------------: sip::init()\n");
        //         test = true;
        //         sip::init();
        //     }
        // }
        // else
        // {
        //     if (test)
        //     {
        //         test = false;
        //         sip::destroy();
        //         printf("---------------------------------------: sip::destroy()\n");
        //     }
        // }
    }

    return 0;
}