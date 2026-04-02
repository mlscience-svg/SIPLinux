#ifndef SIMULATE_CODE_H_
#define SIMULATE_CODE_H_

#include <stdio.h>
#include <string>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

class SimulateCode
{
public:
    SimulateCode(std::string path);
    ~SimulateCode();

    int h264_readnalu(uint8_t *p, size_t *size, int *type);

private:
    int find_start_code(uint8_t *buf, int start, int end);

private:
    uint8_t *h264_buffer = NULL;
    size_t h264_size = 0;
    int pos = 0;

    uint8_t *last_sps = NULL;
    size_t last_sps_size = 0;

    uint8_t *last_pps = NULL;
    size_t last_pps_size = 0;
};

#endif