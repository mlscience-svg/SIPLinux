#include "sip_media_handle.h"

/**
 * 编解码器初始化
 */
sdk_status_t codec_init(sdk_uuid_t call_uuid,
                        video_media_config *vid_media_config,
                        void **user_data)
{
    /* 这里初始化编解码器 这里是文件中读取模拟编码器*/
    // SimulateCode *simulateCode = new SimulateCode("/data/test.h264");
    // // user_data 你的私有数据
    // *user_data = simulateCode;

    vid_media_config->fps = 15;
    vid_media_config->width = 640;
    vid_media_config->height = 480;
    vid_media_config->min_block_datas = 30;

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
// uint8_t *buffer = (uint8_t *)malloc(640 * 480);
// size_t buffer_size = 0;
// int frame_type = 0;
// 组合一个空 H.264 i帧数据，用于侦测数据
static uint8_t empty_i_frame_nalu[] = {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};
static sdk_status_t codec_encode(void *user_data,
                                 sdk_timestamp_t timestamp,
                                 void *buf,
                                 unsigned *buf_size,
                                 sdk_bool_t *is_keyframe,
                                 sdk_bool_t required_keyframe)
{
    *buf_size = 7;
    *is_keyframe = SDK_TRUE;
    memcpy(buf, empty_i_frame_nalu, 7);
    printf("-------------------------------------------------------------: 发送侦测帧\n");
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
sdk_status_t codec_decode(void *user_data, sdk_timestamp_t timestamp, unsigned char *data, unsigned data_size)
{
    return 0; // 假设成功返回0，而不是-1
}

/**
 * 释放销毁编解码器
 * user_data: 你加入的私有数据
 **/
static sdk_status_t codec_deinit(void *user_data)
{
    // SimulateCode *simulateCode = (SimulateCode *)user_data;
    // delete simulateCode;
    return SDK_SUCCESS;
}

/**
 * 返回音频数据
 */
static audio_media_frame *audio_frame_from_stream()
{
    // 音频数据大小,需要根据实际大小填写
    // int size = 0;
    // 分配一个音频结构体
    // audio_media_frame *media_frame = alloc_audio_media_frame(size);
    // if (media_frame == NULL || media_frame->buf == NULL)
    // {
    //     goto error;
    // }

    // 加入数据
    // memcpy(media_frame->buf, buf, size);

    // 正常返回不需要释放，发生任何错误没有正常返回需要自己释放media_frame
    // return media_frame;
    return NULL;

error:
    // free_audio_media_frame(media_frame);
    return NULL;
}

/**
 * 接收音频数据并播放
 */
static sdk_status_t on_call_audio_media_stream(audio_media_frame media_frame)
{
    // media_frame.buf 数据
    // media_frame.size 数据大小
    // 播放成功返回SDK_SUCCESS，失败返回 SDK_ERROR_COMMON
    return SDK_ERROR_COMMON;
}

void media_init()
{
    // 音频时钟速率
    sip_media_config.audio_clock_rate = 16000;
    // mic 增益
    sip_media_config.mic_gain = 1;
    // speaker 增益
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
    sip_media_config.audio_op.read_audio_frame_from_stream = audio_frame_from_stream;
    sip_media_config.audio_op.write_audio_media_stream = on_call_audio_media_stream;
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
