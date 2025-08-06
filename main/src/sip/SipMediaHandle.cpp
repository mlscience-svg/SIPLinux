#include "SipMediaHandle.hpp"
#include "utils/AlsaUtils.hpp"

static std::mutex mutex_file_write;
namespace sipmedia
{

    /**
     * 编解码器初始化
     */
    sdk_status_t codec_init(sdk_uuid_t call_uuid,
                            video_media_config *vid_media_config,
                            void **user_data)
    {
        vid_media_config->fps = 25;
        vid_media_config->width = 640;
        vid_media_config->height = 480;
        vid_media_config->min_block_datas = 60;

        /* 这里初始化编解码器 这里是文件中读取模拟编码器*/
        SimulateCode *simulateCode = new SimulateCode("/data/test.h264");
        simulateCode->call_uuid = call_uuid;
        // user_data 你的私有数据
        *user_data = simulateCode;
        return SDK_SUCCESS;
    }

    /**
     * 编码回调
     * 这里放入H.264数据
     * user_data: 前面你的私有数据
     * buf: 回调buf，需要将 H.264 数据拷贝进来
     * buf_size: H.264 数据大小
     * is_keyframe: H.264 数据是否是关键帧，是：SDK_TRUE 不是: SDK_FLASE
     * required_keyframe: 需要强制返回关键帧
     */
    uint8_t *buffer = (uint8_t *)malloc(1920 * 1080);
    size_t buffer_size = 0;
    int frame_type = 0;
    sdk_status_t codec_encode(void *user_data,
                              sdk_timestamp_t timestamp,
                              void *buf,
                              unsigned *buf_size,
                              sdk_bool_t *is_keyframe,
                              sdk_bool_t required_keyframe)
    {
        if (required_keyframe)
        {
            // 这里最好强制编码关键帧，这个时候对方客户端是需要关键帧的
        }
        memset(buffer, 0, 1920 * 1080);
        SimulateCode *simulateCode = (SimulateCode *)user_data;
        // 从文件中读取帧
        int ret = simulateCode->h264_readnalu(buffer, &buffer_size, &frame_type);
        if (ret <= 0)
        {
            printf("ReadNalu error 1\n");
            usleep(1000 * 1000);
            return SDK_ERROR_COMMON;
        }
        memcpy(buf, buffer, buffer_size);
        *buf_size = buffer_size;
        *is_keyframe = frame_type;
        return SDK_SUCCESS;
    }

    /**
     * 收到H.264数据
     * 这里将每路流的H.264数据保存到文件中
     * 可以安装 ffmpeg 使用ffplay播放
     *
     * call_uuid: 呼叫uuid,记录每一个呼叫
     * user_data: 前面你加入的私有数据
     * data: H.264 数据
     * data_size: H.264 数据大小
     */
    std::map<sdk_uuid_t, std::ofstream> h264_files;
    sdk_status_t codec_decode(void *user_data,
                              sdk_timestamp_t timestamp,
                              int type,
                              unsigned char *data,
                              unsigned data_size)
    {
        printf("---------------------------------------------timestamp: %llu\n", timestamp);
        mutex_file_write.lock();
        SimulateCode *simulateCode = (SimulateCode *)user_data;
        auto it = h264_files.find(simulateCode->call_uuid);
        if (it == h264_files.end())
        {
            // 如果没找到，创建一个新的文件流
            std::string file_name = "/data/h264/" + std::to_string(simulateCode->call_uuid) + ".h264";
            h264_files[simulateCode->call_uuid].open(file_name, std::ios::binary);
            if (!h264_files[simulateCode->call_uuid].is_open())
            {
                return -1;
            }
            it = h264_files.find(simulateCode->call_uuid);
        }
        std::ofstream &h264_file = it->second;
        if (h264_file.is_open())
        {
            h264_file.write(reinterpret_cast<char *>(data), data_size);
        }
        mutex_file_write.unlock();
        return SDK_SUCCESS;
    }

    /**
     * 释放销毁编解码器
     * user_data: 你加入的私有数据
     **/
    sdk_status_t codec_deinit(void *user_data)
    {
        SimulateCode *simulateCode = (SimulateCode *)user_data;
        delete simulateCode;
        return SDK_SUCCESS;
    }

    audio_media_frame *audio_frame_from_stream()
    {
        return ualsa::read();
    }

    void audio_frame_from_stream_v2(void *buf, int size)
    {
        ualsa::read(buf, size);
    }

    sdk_status_t on_call_audio_media_stream(audio_media_frame media_frame)
    {
        return ualsa::play((uint8_t *)media_frame.buf, media_frame.size);
    }

    void init()
    {
        /* 音频时钟速率 */
        sip_media_config.audio_clock_rate = 16000;
        sip_media_config.mic_gain = 1;
        sip_media_config.speaker_gain = 1;
        // 噪音抑制
        sip_media_config.ns_enable = SDK_TRUE;
        // 回音消除
        sip_media_config.aec_enable = SDK_TRUE;
        // 回音消除时间
        sip_media_config.aec_elimination_time = 30;
        // 自动增益
        sip_media_config.agc_enable = SDK_TRUE;

        sip_media_config.video_op.codec_init = codec_init;
        sip_media_config.video_op.codec_deinit = codec_deinit;
        // sip_media_config.audio_op.read_audio_frame_from_stream = audio_frame_from_stream;
        sip_media_config.audio_op.read_audio_frame_from_stream_v2 = audio_frame_from_stream_v2;
        sip_media_config.audio_op.write_audio_media_stream = on_call_audio_media_stream;
        // 组合sps pps idr
        sip_media_config.combin_sps_pps_idr = SDK_TRUE;
        // 是否开启解码
        if (INIT_ENABLE_DECODE)
        {
            sip_media_config.not_enable_decode = SDK_FALSE;
            sip_media_config.video_op.codec_decode = codec_decode;
        }
        else
        {
            sip_media_config.not_enable_decode = SDK_TRUE;
        }
        // 是否开启编码
        if (INIT_ENABLE_ENCODE)
        {
            sip_media_config.not_enable_encode = SDK_FALSE;
            sip_media_config.video_op.codec_encode = codec_encode;
        }
        else
        {
            sip_media_config.not_enable_encode = SDK_TRUE;
        }
    }
}
