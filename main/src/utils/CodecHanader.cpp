#include <algorithm>
#include "CodecHanader.hpp"

std::vector<std::function<void(uint8_t *data, size_t data_size, bool keyframe)>> onMediaCallbacks;

h264_data *h264_data_malloc(size_t size)
{
    h264_data *data = (h264_data *)malloc(sizeof(h264_data));
    data->data = (uint8_t *)malloc(size);
    return data;
}

void h264_data_free(h264_data *data)
{
    if (!data)
    {
        return;
    }
    if (data->data)
    {
        free(data->data);
        data->data = NULL;
    }
    free(data);
    data = NULL;
}

// 类初始化
CodecHanader::CodecHanader()
{
}

// 类销毁
CodecHanader::~CodecHanader()
{
    // 最好观察是否有打印，防止没有释放
    printf("~CodecHanader");
}

bool CodecHanader::init()
{
    onMediaCall = [this](uint8_t *data, size_t data_size, bool keyframe)
    {
        // 这里数据加入队列
        if (h264_data_queue.size() > 2)
        {
            // 队列满
            return -1;
        }
        // 为解码数据分配内存
        h264_data *h_data = h264_data_malloc(data_size);
        memcpy(h_data->data, data, data_size);
        h_data->data_size = data_size;
        h_data->keyframe = keyframe;
        h264_data_queue.push(h_data);
        // lambda end
    };
    // 这里注册回调函数
    onMediaCallbacks.push_back(onMediaCall);
    return true;
}

int CodecHanader::dequeue(uint8_t *data, size_t &data_size, bool &keyframe)
{
    if (h264_data_queue.empty())
    {
        return -1;
    }
    // 弹出数据
    h264_data *item = h264_data_queue.front();
    h264_data_queue.pop();
    keyframe = item->keyframe;
    data_size = item->data_size;
    memcpy(data, item->data, item->data_size);
    // 释放数据
    h264_data_free(item);
    return 0;
}

void CodecHanader::close()
{
    // 释放所有未使用的数据
    while (!h264_data_queue.empty())
    {
        h264_data *data = h264_data_queue.front();
        h264_data_queue.pop();
        h264_data_free(data);
    }
    // 这里销毁
    if (onMediaCall)
    {
        // 从回调列表中移除当前回调
        onMediaCallbacks.erase(std::remove_if(onMediaCallbacks.begin(), onMediaCallbacks.end(),
                                              [this](const std::function<void(uint8_t *, size_t, bool)> &callback)
                                              {
                                                  return callback.target<void (*)(uint8_t *, size_t, bool)>() == onMediaCall.target<void (*)(uint8_t *, size_t, bool)>();
                                              }),
                               onMediaCallbacks.end());
        onMediaCall = nullptr; // 清除当前回调
    }
}