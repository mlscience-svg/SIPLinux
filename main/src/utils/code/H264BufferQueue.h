#ifndef H264_BUFFER_QUEUE_H_
#define H264_BUFFER_QUEUE_H_

#include <cstdint>
#include <mutex>
#include <queue>
#include <cstring>

struct H264BufferInfo
{
    uint32_t size = 0;         // 缓冲区大小
    uint8_t *buffer = nullptr; // 数据
    bool isKeyframe = false;   // 是否关键帧
};

class H264BufferQueue
{
public:
    explicit H264BufferQueue(size_t maxSize = 2) : max_queue_size(maxSize) {}

    // 入队
    void enqueue(const uint8_t *buffer, uint32_t size, bool isKeyframe)
    {
        H264BufferInfo *info = (H264BufferInfo *)malloc(sizeof(H264BufferInfo));
        if (!info)
            return;

        info->buffer = (uint8_t *)malloc(size);
        if (!info->buffer)
        {
            free(info);
            return;
        }

        memcpy(info->buffer, buffer, size);
        info->size = size;
        info->isKeyframe = isKeyframe;

        std::unique_lock<std::mutex> lock(mutex);
        if (buffer_queue.size() >= max_queue_size)
        {
            printf("buffer_queue.size() >= max_queue_size\n\n");
            freeBufferInfo(buffer_queue.front());
            buffer_queue.pop();
        }
        buffer_queue.push(info);
    }

    // 出队
    bool dequeue(uint8_t *buf, unsigned *size, bool *isKeyframe)
    {
        std::unique_lock<std::mutex> lock(mutex);
        if (buffer_queue.empty())
            return false;

        H264BufferInfo *info = buffer_queue.front();
        *size = info->size;
        *isKeyframe = info->isKeyframe;
        memcpy(buf, info->buffer, info->size);

        freeBufferInfo(info);
        buffer_queue.pop();
        return true;
    }

    // 清空队列
    void flush()
    {
        std::unique_lock<std::mutex> lock(mutex);
        while (!buffer_queue.empty())
        {
            freeBufferInfo(buffer_queue.front());
            buffer_queue.pop();
        }
    }

    // 析构函数：清理资源
    ~H264BufferQueue()
    {
        flush();
        printf("~H264BufferQueue()\n\n");
    }

private:
    // 释放单个 H264BufferInfo 资源
    void freeBufferInfo(H264BufferInfo *info)
    {
        if (info)
        {
            if (info->buffer)
                free(info->buffer);
            free(info);
        }
    }

private:
    std::mutex mutex;                          // 用于线程同步
    std::queue<H264BufferInfo *> buffer_queue; // 存储 H264 数据
    size_t max_queue_size;                     // 队列最大大小
};

#endif // H264_BUFFER_QUEUE_H_
