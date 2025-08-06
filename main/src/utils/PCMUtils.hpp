#ifndef PCM_UTILS_H_
#define PCM_UTILS_H_
#include <fstream>
#include <fcntl.h>
#include <cstdint>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <sip_sdk.h>
#include <mutex>
#include <stdlib.h>
#include <functional>
#include <map>

class PCMUtils
{
private:
    std::string file_path = "/data/pcm/";
    std::mutex mutex_pcm_file;
    std::map<sdk_uuid_t, std::ofstream> pcm_file_s;

public:
    PCMUtils();
    ~PCMUtils();
    sdk_uuid_t call_uuid;

    int write_pcm(void *buf, sdk_size_t size);
};

#endif