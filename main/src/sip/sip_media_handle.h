#ifndef SIP_MEDIA_HANDLE_H__
#define SIP_MEDIA_HANDLE_H__
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include "sip_sdk.h"
#include "sip_sdk_media.h"

// 是否开启编码（如果只需要解码，关闭可以省内存）
#define INIT_ENABLE_ENCODE 1
// 是否开启解码（如果只需要编码，关闭可以省内存）
#define INIT_ENABLE_DECODE 1

// 初始化
void media_init();

#endif