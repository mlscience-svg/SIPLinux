#ifndef SIP_HANDLE_H_
#define SIP_HANDLE_H_
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <string>
#include "sip_sdk.h"
#include "SipMediaHandle.hpp"
#include "utils/PCMUtils.hpp"

namespace sip
{
    // 初始化结束回调
    void on_init_completed(sdk_status_t state, const char *msg);

    // 注册服务器状态回调
    void on_registrar_state(sdk_status_t state);

    // 收到呼叫
    void on_incoming_call(sip_sdk_call_param call_param);

    // 收到dtmf info消息
    void on_dtmf_info(sip_sdk_dtmf_info_param dtmf_info_param);

    // 收到sip message消息
    void on_message(sip_sdk_message_param message_param);

    // 发送sip message消息状态回调
    void on_message_state(sdk_status_t state, sip_sdk_message_param message_param);

    // 呼叫状态回调
    void on_call_state(sdk_uuid_t call_uuid, sdk_status_t state);

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
    void call(std::string username, const std::map<std::string, std::string> &headers);

    // IP呼叫
    void callIP(std::string ip, const std::map<std::string, std::string> &headers);

    // 接听呼叫
    void answer(unsigned code);

    // 发送 Dtmf Info消息
    void sendDtmfInfo(int type, const std::string &contentType, const std::string &content);

    // 发送sip消息
    void sendMessage(const std::string &username, const std::string &content);

    // 发送sip消息，ip发送
    void sendMessageIP(const std::string &ip, const std::string &content);

    // 挂断所有
    void hangup(unsigned code);

    // 挂断
    void hangup(unsigned code, sdk_uuid_t uuid);

    // 打印pjsip信息
    void dump();
}

#endif