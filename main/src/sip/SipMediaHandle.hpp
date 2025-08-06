#ifndef SIP_MEDIA_HANDLE_H_
#define SIP_MEDIA_HANDLE_H_
#include <mutex>
#include <stdlib.h>
#include <functional>
#include <map>
#include <fstream>
#include "sip_sdk_media.h"
#include "utils/SimulateCode.hpp"

// 是否开启编码（如果只需要解码，关闭可以省内存）
#define INIT_ENABLE_ENCODE 1
// 是否开启解码（如果只需要编码，关闭可以省内存）
#define INIT_ENABLE_DECODE 0

namespace sipmedia
{
    // 初始化
    void init();
}

#endif