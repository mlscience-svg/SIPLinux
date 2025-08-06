#include <string>
#include "PCMUtils.hpp"

PCMUtils::PCMUtils()
{
}

PCMUtils::~PCMUtils()
{
}
int PCMUtils::write_pcm(void *buf, sdk_size_t size)
{
    std::lock_guard<std::mutex> lock(mutex_pcm_file);
    auto it = pcm_file_s.find(call_uuid);
    if (it == pcm_file_s.end())
    {
        // 如果没找到，创建一个新的文件流
        std::string file_name = "/data/pcm/" + std::to_string(call_uuid) + ".pcm";
        pcm_file_s[call_uuid].open(file_name, std::ios::binary);
        if (!pcm_file_s[call_uuid].is_open())
        {
            return -1;
        }
        it = pcm_file_s.find(call_uuid);
    }
    std::ofstream &h264_file = it->second;
    if (h264_file.is_open())
    {
        h264_file.write(reinterpret_cast<char *>(buf), size);
    }
}