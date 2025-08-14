#ifndef SIMULATE_CODE_H_
#define SIMULATE_CODE_H_
#include <fstream>
#include <fcntl.h>
#include <cstdint>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <sip_sdk.h>
class SimulateCode
{
private:
    int h264_fd;
    uint8_t *h264_buffer = nullptr;
    int pos = 0;
    size_t h264_size;

public:
    SimulateCode(std::string path);
    ~SimulateCode();

    int find_start_code(uint8_t *buf, int start, int end);
    int h264_readnalu(uint8_t *p, size_t *size, int *type);
};

#endif