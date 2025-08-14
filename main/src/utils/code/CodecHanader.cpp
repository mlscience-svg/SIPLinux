#include "CodecHanader.hpp"
#include <algorithm>
#include <memory>

std::mutex CodecHanader::mutex;
std::vector<CodecHanader *> CodecHanader::chVector;

// 创建解码处理类
CodecHanader *CodecHanader::create(const sdk_uuid_t &call_uuid)
{
    std::unique_lock<std::mutex> lock(mutex);
    CodecHanader *instance = new CodecHanader(call_uuid);
    if (!instance->is_valid)
    {
        delete instance;
        return nullptr;
    }
    // 加入实体
    chVector.push_back(instance);
    return instance;
}

// 类初始化
CodecHanader::CodecHanader(const sdk_uuid_t &call_uuid) : call_uuid(call_uuid)
{
    // 初始化编码队列
    encodeQueue = new H264BufferQueue();
    // 构造成功
    is_valid = true;
}

// 类销毁
CodecHanader::~CodecHanader()
{
    std::unique_lock<std::mutex> lock(mutex);
    // 释放编码队列
    if (encodeQueue)
    {
        delete encodeQueue;
        encodeQueue = nullptr;
    }
    // 删除当前实例
    auto it = std::find(chVector.begin(), chVector.end(), this);
    if (it != chVector.end())
    {
        chVector.erase(it); // 从容器中移除当前对象
    }
    printf("~CodecHanader()\n\n");
}

// 加入编码数据到所有 CodecHanader 实例
int CodecHanader::enqueue(uint8_t *data, uint32_t &size, bool &keyframe)
{
    std::unique_lock<std::mutex> lock(mutex); // 锁住 chVector，防止在访问期间其他线程修改
    for (auto &item : chVector)
    {
        if (item->encodeQueue)
        {
            item->encodeQueue->enqueue(data, size, keyframe);
        }
    }
    return 0;
}

// 获取编码数据
bool CodecHanader::dequeue(uint8_t *buf, unsigned *size, bool *isKeyframe)
{
    if (encodeQueue)
    {
        return encodeQueue->dequeue(buf, size, isKeyframe);
    }
    return false;
}
