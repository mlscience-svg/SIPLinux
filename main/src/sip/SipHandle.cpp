#include "SipHandle.hpp"
#include "curl_license.h"
#include "utils/AlsaUtils.hpp"

#define SDK_LICENSE_CLIENT_ID "1364998327518760960"
#define SDK_LICENSE_CLIENT_SECRET "fe6a46f2e71857a93963eec77d67b04b"
#define SDK_LICENSE_SERVER_URL "https://api.mlscience.cn/"
#define SDK_LICENSE_AUTH_FILE "/data/device.json"

namespace sip
{

    /**
     * 初始化结束回调
     */
    void on_init_completed(sdk_status_t state, const char *msg)
    {
        // 初始化成功可以去注册一下
        registrar();
        printf("on_init_completed\n");
    }

    /**
     * 初始化失败
     */
    void on_stop_completed()
    {
        printf("on_stop_completed\n");
    }

    /**
     * 注册服务器状态回调
     *
     * 具体查阅sip状态码
     */
    void on_registrar_state(sdk_status_t state)
    {
    }

    /**
     * 收到呼叫
     */
    void on_incoming_call(sip_sdk_call_param call_param)
    {
        printf("on_incoming_call call_type: %d\n", call_param.call_type);
        printf("on_incoming_call username: %s\n", call_param.username);
        printf("on_incoming_call remote_ip: %s\n", call_param.remote_ip);
        for (int i = 0; i < SDK_MAX_CUSTOM_HEADERS; i++)
        {
            if (call_param.headers[i].key && call_param.headers[i].value)
            {
                printf("on_incoming_call header key: |%s|   value: |%s|\n", call_param.headers[i].key, call_param.headers[i].value);
            }
        }
        printf(" call_uuid: %llu\n", call_param.call_uuid);
    }

    /**
     * 收到dtmf info消息
     */
    void on_dtmf_info(sip_sdk_dtmf_info_param dtmf_info_param)
    {
        printf("dtmf_info_param: %llu content: %s\n", dtmf_info_param.call_uuid, dtmf_info_param.content);
    }

    /**
     * 收到sip message消息
     */
    void on_message(sip_sdk_message_param message_param)
    {
        printf("on_message message_type: %d\n", message_param.message_type);
        printf("on_message username: %s\n", message_param.username);
        printf("on_message remote_ip: %s\n", message_param.remote_ip);
        printf("on_message content: %s\n", message_param.content);
    }

    /**
     * 发送sip message消息状态回调
     */
    void on_message_state(sdk_status_t state, sip_sdk_message_param message_param)
    {
        printf("on_message_state state: %d\n", state);
        printf("on_message_state message_type: %d\n", message_param.message_type);
        printf("on_message_state username: %s\n", message_param.username);
        printf("on_message_state remote_ip: %s\n", message_param.remote_ip);
        printf("on_message_state content: %s\n", message_param.content);
    }

    static sdk_status_t on_port_stream(void *user_data, audio_media_frame media_frame)
    {
        // 下面是将数据每一路通话声音写入文件
        if (media_frame.size > 0 && media_frame.buf != NULL)
        {
            PCMUtils *pcm_utils = (PCMUtils *)user_data;
            pcm_utils->write_pcm(media_frame.buf, media_frame.size);
        }
    }

    static sdk_status_t on_destroy(void *user_data)
    {
        PCMUtils *pcm_utils = (PCMUtils *)user_data;
        delete pcm_utils;
    }

    /**
     * 呼叫状态回调
     *
     * 默认状态
     * CALL_STATE_NULL
     *
     * 呼叫状态 正在呼叫
     * CALL_STATE_CALLING
     *
     * 呼叫状态 正在被叫叫
     * CALL_STATE_INCOMING
     *
     * 呼叫状态 回铃音
     * CALL_STATE_EARLY
     *
     * 呼叫状态 正在连接
     * CALL_STATE_CONNECTING
     *
     * 呼叫状态 连接成功
     * CALL_STATE_CONFIRMED
     *
     * 呼叫状态 断开连接
     * CALL_STATE_DISCONNECTED
     */
    std::map<sdk_uuid_t, void *> audio_port_s;
    void on_call_state(sdk_uuid_t call_uuid, sdk_status_t state)
    {
        if (state == CALL_STATE_CONFIRMED)
        {
            // 开启alsa
            ualsa::open();
        }

        // 下面的代码可以单独获取每一路声音
        //  if (state == CALL_STATE_CONFIRMED)
        //  {
        //      int media_id = media_connect_id(call_uuid);
        //      if (media_id > 0)
        //      {
        //          PCMUtils *pcm_utils = new PCMUtils();
        //          pcm_utils->call_uuid = call_uuid;
        //          std::string name = "port:" + std::to_string(call_uuid);
        //          sip_sdk_media_audio_port_param port_param;
        //          port_param.media_id = media_id;
        //          port_param.name = (char *)name.c_str();
        //          port_param.user_data = pcm_utils;
        //          port_param.clock_rate = 16000;
        //          port_param.channel_count = 1;
        //          port_param.bits_per_sample = 16;
        //          port_param.samples_per_frame = 320;
        //          port_param.on_port_stream = on_port_stream; // 回调音频数据
        //          // port_param.get_port_stream;                 // 获取音频数据
        //          port_param.on_destroy = on_destroy; // 销毁回调
        //          void *audio_port = create_custom_audio_port(port_param);
        //          if (audio_port)
        //          {
        //              audio_port_s[call_uuid] = audio_port;
        //          }
        //          else
        //          {
        //              delete pcm_utils;
        //          }
        //      }
        //  }
        //  else if (state == CALL_STATE_DISCONNECTED)
        //  {
        //      if (audio_port_s.count(call_uuid) > 0)
        //      {
        //          // 键存在，可以安全地访问值
        //          void *audio_port = audio_port_s[call_uuid];
        //          destroy_custom_audio_port(audio_port);
        //      }
        //  }
    }

    void expire_warning_callback(time_t expire_time, time_t current_time)
    {
        printf("expire_warning_callback: %ld  %ld\n", expire_time, current_time);
    }

    /**
     * 注册回调函数
     */
    static sip_sdk_observer sdk_observer = {
        nullptr, // 配置后日志统一输出，自己打印
        &on_init_completed,
        &on_stop_completed,
        &on_registrar_state,
        &on_incoming_call,
        &on_dtmf_info,
        &on_message,
        &on_message_state,
        &on_call_state,
        &expire_warning_callback,
    };

    /**
     * 初始化sdk
     */
    void init()
    {
        // 设备唯一ID
        std::string dev_uuid = "test123456";
        // 注册 SDK
        sdk_status_t status = sip_sdk_register(SDK_LICENSE_CLIENT_ID,
                                               SDK_LICENSE_CLIENT_SECRET,
                                               dev_uuid.c_str(),
                                               SDK_LICENSE_AUTH_FILE,
                                               SDK_LICENSE_TYPE_AUTHORIZATION_TIME);
        if (status != 0)
        {
            // 检查文件是否存在
            std::ifstream infile(SDK_LICENSE_AUTH_FILE);
            if (infile.good())
            {
                return; // 文件已存在，直接返回
            }
            infile.close();

            // 获取license
            char info[1024] = {0};
            bool ret = sync_token(SDK_LICENSE_SERVER_URL, SDK_LICENSE_CLIENT_ID, SDK_LICENSE_CLIENT_SECRET, info);
            if (!ret)
            {
                printf("sip sdk license error\n");
                return;
            }

            // 文件不存在，写入license
            std::ofstream outfile(SDK_LICENSE_AUTH_FILE);
            if (!outfile)
            {
                printf("Failed to create file: %s\n", SDK_LICENSE_AUTH_FILE);
                return;
            }
            outfile << info;
            outfile.close();
            system("sync");
        }

        sip_sdk_config.sdk_run = SDK_TRUE;
        sip_sdk_config.port = 58581;
        memset(sip_sdk_config.public_addr, 0, 64);
        sip_sdk_config.log_level = 4;
        sip_sdk_config.video_enable = SDK_TRUE;
        sip_sdk_config.video_out_auto_transmit = SDK_TRUE;
        sip_sdk_config.allow_multiple_connections = SDK_FALSE;
        char *user_agent = "linux-sdk-1.0";
        memset(sip_sdk_config.user_agent, 0, 32);
        memcpy(sip_sdk_config.user_agent, user_agent, strlen(user_agent));
        sip_sdk_config.sdk_observer = &sdk_observer;
        sip_sdk_config.transport_stun_server = SDK_FALSE;
        sip_sdk_config.does_it_support_broadcast = SDK_TRUE;
        sip_sdk_stun_config stun_cfg = {
            1, // 如果不需要Stun不配置或者设置0
            {
                "120.79.7.237:3478",
            },
            SDK_FALSE // enable_ipv6
        };
        sip_sdk_config.stun_config = stun_cfg;
        // 初始化媒体
        sipmedia::init();
        // 初始化sdk
        sip_sdk_init();
    }

    /**
     * 销毁sdk
     */
    void destroy()
    {
        sip_sdk_destroy();
    }

    /**
     * IP发生改变需要回调，或者重启sdk
     */
    void handleIpChange()
    {
        sip_sdk_handle_ip_change();
    }

    /**
     * 注册本地账号，注册到服务器
     */
    void registrar()
    {
        // 初始化本地账号（这里初始化username，会将本地sip消息体中from由原有的sip:+ip改为sip:+username)
        sip_sdk_local_config local_config = {
            .username = "test",
            .proxy = NULL,
            .proxy_port = 0,
            .enable_stream_control = SDK_FALSE,
            .stream_elapsed = 2,
            .start_keyframe_count = 120,
            .start_keyframe_interval = 1000,
        };
        local_account(local_config);
        // 注册到服务器
        sip_sdk_registrar_config registrar_config = {
            .domain = "test.com",
            .username = "test",
            .password = "123456",
            .transport = "tcp",
            .server_addr = "43.160.204.96",
            .server_port = 5060,
            .headers = {},
            .proxy = "43.160.204.96",
            .proxy_port = 5060,
            .enable_stream_control = SDK_FALSE,
            .stream_elapsed = 5,
            .start_keyframe_count = 10,
            .start_keyframe_interval = 1000,
            .turn_config = {
                .enable = SDK_TRUE, // 如果不需要Turn不配置或者设置SDK_FALSE
                .server = "120.79.7.237:3478",
                .realm = "120.79.7.237",
                .username = "test",
                .password = "test",
            }};
        registrar_account(registrar_config);
    }

    /**
     * 解除注册到服务器
     */
    void unRegistrar()
    {
        unregistrar();
    }

    /**
     * 通过服务器呼叫
     * username: 对方用户名
     * headers: 自定义头信息
     */
    void call(std::string username, const std::map<std::string, std::string> &headers)
    {
        if (username.empty())
        {
            return;
        }
        /* 服务器呼叫 返回call_uuid */
        sip_sdk_call_param sscp = {
            SDK_CALL_TYPE_SERVER,
            (char *)username.c_str(),
            NULL,
        };
        size_t i = 0;
        for (const auto &header : headers)
        {
            sscp.headers[i].key = const_cast<char *>(header.first.c_str());
            sscp.headers[i].value = const_cast<char *>(header.second.c_str());
            i++;
        }
        sscp.transmit_sound = SDK_TRUE;
        sscp.transmit_video = SDK_TRUE;
        make_call(&sscp);
        // 这里拿到呼叫uuid
    }

    /**
     * 通过服IP呼叫
     * ip: 对方IP
     * headers: 自定义头信息
     */
    void callIP(std::string ip, const std::map<std::string, std::string> &headers)
    {
        if (ip.empty())
        {
            return;
        }
        /* IP呼叫 返回call_uuid */
        sip_sdk_call_param sscp = {
            SDK_CALL_TYPE_IP,
            NULL,
            (char *)ip.c_str(),
        };
        size_t i = 0;
        for (const auto &header : headers)
        {
            sscp.headers[i].key = const_cast<char *>(header.first.c_str());
            sscp.headers[i].value = const_cast<char *>(header.second.c_str());
            i++;
        }
        sscp.transmit_sound = SDK_TRUE;
        sscp.transmit_video = SDK_TRUE;
        make_call(&sscp);
        // 这里拿到呼叫uuid
    }

    /**
     * 接听呼叫
     * code: 接听状态码，正常接听200，先通媒体183
     */
    void answer(unsigned code)
    {
        /* 接听呼叫 */
        sip_sdk_answer_param answer_param = {
            code, // 接听状态码
            0,    // 呼叫uuid，为0表示接听所有被叫
            SDK_TRUE,
            SDK_TRUE,
        };
        answer_call(answer_param);
    }

    /**
     * 发送info消息
     * type: 消息类型
     * contentType: 内容类型
     * content: 内容（除自定义类型外、其他的类型内容只能是一个字节）
     */
    void sendDtmfInfo(int type, const std::string &contentType, const std::string &content)
    {
        sip_sdk_dtmf_info_param dtmf_info_param = {
            type,
            0,
            (char *)contentType.c_str(),
            (char *)content.c_str(),
        };
        send_dtmf_info(dtmf_info_param);
    }

    /**
     * 通过服务器发送sip message消息
     * username: 对方账号
     * content: 内容
     */
    void sendMessage(const std::string &username, const std::string &content)
    {
        sip_sdk_message_param message_param = {
            SDK_MESSAGE_TYPE_SERVER,
            (char *)username.c_str(),
            NULL,
            (char *)content.c_str(),
        };
        send_message(message_param);
    }

    /**
     * 通过IP发送sip message消息
     * username: 对方账号
     * content: 内容
     */
    void sendMessageIP(const std::string &ip, const std::string &content)
    {
        sip_sdk_message_param message_param = {
            SDK_MESSAGE_TYPE_IP,
            NULL,
            (char *)ip.c_str(),
            (char *)content.c_str(),
        };
        send_message(message_param);
    }

    /**
     * 挂断所有呼叫
     * code: 挂断状态码，正常挂断200
     */
    void hangup(unsigned code)
    {
        hangup(code, 0);
    }

    /**
     * 挂断 call_uuid 对应的呼叫
     * code: 挂断状态码，正常挂断200
     * uuid: call_uuid
     */
    void hangup(unsigned code, sdk_uuid_t uuid)
    {
        sip_sdk_hangup_param hangup_param = {code, uuid};
        hangup_call(hangup_param);
    }

    /**
     *  打印SDK信息，包括所有内存使用信息
     */
    void dump()
    {
        dump_info();
    }
}