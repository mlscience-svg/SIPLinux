#ifndef PCM_BUFFER_QUEUE_H_
#define PCM_BUFFER_QUEUE_H_

#include <cstdint>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <cstring>
#include <cstdlib>

class PCMBufferQueue
{
public:
    void enqueue(uint8_t *buffer, int32_t size)
    {
        size_t combined_size = previous_index + size;

        if (combined_size < 640)
        {
            // 拼接后的数据不足 640，不加入队列，保存到下一帧
            std::memcpy(previous_data + previous_index, buffer, size);
            previous_index = combined_size;
            return;
        }

        uint8_t *output_data = (uint8_t *)malloc(640);
        // 如果拼接后的数据大小大于等于 640
        size_t remainingSpace = 640 - previous_index;
        std::memcpy(output_data, previous_data, previous_index);
        std::memcpy(output_data + previous_index, buffer, remainingSpace);

        // 剩余部分存入上一帧的前部分，作为下次拼接的起始数据
        std::memcpy(previous_data, buffer + remainingSpace, (combined_size - 640));
        previous_index = combined_size - 640;

        // 加入队列
        {
            std::unique_lock<std::mutex> lock(mutex);
            if (buffer_queue.size() >= 5)
            {
                // 删除最旧的数据
                uint8_t *buffer = buffer_queue.front();
                free(buffer);
                buffer_queue.pop();
            }
            buffer_queue.push(output_data);
        }
    }

    void dequeue(uint8_t *buf)
    {
        std::unique_lock<std::mutex> lock(mutex);
        if (buffer_queue.empty())
        {
            return;
        }
        uint8_t *buffer = buffer_queue.front();
        memcpy(buf, buffer, 640);
        free(buffer);
        buffer_queue.pop();
    }

    void flush()
    {
        std::unique_lock<std::mutex> lock(mutex);
        while (!buffer_queue.empty())
        {
            uint8_t *buffer = buffer_queue.front();
            free(buffer);
            buffer_queue.pop();
        }
    }

    ~PCMBufferQueue()
    {
        flush(); // 清理队列中的数据
    }

private:
    std::mutex mutex;

    uint8_t previous_data[640] = {0}; // 上一帧的 PCM 数据（uint8_t 类型）
    int32_t previous_index = 0;       // 上一帧数据的大小

    std::queue<uint8_t *> buffer_queue; // 用于存储 PCM 数据的队列
};

extern PCMBufferQueue pcmBufferQueue;

#endif // PCM_BUFFER_QUEUE_H_
