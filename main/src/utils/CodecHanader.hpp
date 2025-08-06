#ifndef CODEC_HANADER_H_
#define CODEC_HANADER_H_

#include "sip_sdk_media.h"
#include <cstdio>
#include <memory>
#include <string.h>
#include <vector>
#include <iostream>
#include <queue>
#include <functional>

extern std::vector<std::function<void(uint8_t *data, size_t data_size, bool keyframe)>> onMediaCallbacks;

typedef struct h264_data
{
    bool keyframe;
    uint8_t *data;
    size_t data_size;
} h264_data;

class CodecHanader
{
public:
    sdk_uuid_t call_uuid = -1;

    // 取数据
    int dequeue(uint8_t *data, size_t &data_size, bool &keyframe);

    CodecHanader();
    ~CodecHanader();
    // 初始化
    bool init();
    // 结束释放
    void close();

private:
    std::function<void(uint8_t *data, size_t data_size, bool keyframe)> onMediaCall = nullptr;

private:
    std::queue<h264_data *> h264_data_queue;
};

#endif