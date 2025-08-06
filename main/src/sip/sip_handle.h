#ifndef SIP_HANDLE_H__
#define SIP_HANDLE_H__
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include "sip_sdk.h"
#include "sip_media_handle.h"

// 初始化
void init();

// 销毁
void destroy();

// ip发生改变
void handleIpChange();

// 注册账号
void registrar();

// 取消注册账号
void unRegistrar();

// 呼叫
void call(const char *username);

// IP呼叫
void callIP(const char *ip);

// 接听呼叫
void answer(unsigned code);

// 发送 Dtmf Info消息
void sendDtmfInfo(int type, const char *contentType, const char *content);

// 发送sip消息
void sendMessage(const const char *username, const char *content);

// 发送sip消息，ip发送
void sendMessageIP(const char *ip, const char *content);

// 挂断所有
void hangup(unsigned code);

// 挂断
void hangupUuid(unsigned code, sdk_uuid_t uuid);

// 打印pjsip信息
void dump();

#endif