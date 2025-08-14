#ifndef CODEC_HANADER_H_
#define CODEC_HANADER_H_

#include "sip_sdk_media.h"
#include <vector>
#include <mutex>
#include "H264BufferQueue.h"

typedef struct h264_data
{
    bool keyframe;
    uint8_t *data;
    size_t data_size;
} h264_data;

class CodecHanader
{
public:
    // 创建解码处理类
    static CodecHanader *create(const sdk_uuid_t &call_uuid);
    // 加入编码数据
    static int enqueue(uint8_t *data, uint32_t &size, bool &keyframe);

    bool is_valid = false;
    sdk_uuid_t call_uuid = -1;

    CodecHanader(const sdk_uuid_t &uuid);
    ~CodecHanader();
    bool dequeue(uint8_t *buf, unsigned *size, bool *isKeyframe);

private:
    static std::mutex mutex;                     // 用于同步对 chVector 的访问
    static std::vector<CodecHanader *> chVector; // 存储所有 CodecHanader 实例
    H264BufferQueue *encodeQueue = nullptr;      // 编码队列
};

#endif // CODEC_HANADER_H_
