#include "SimulateCode.hpp"

SimulateCode::SimulateCode(std::string path)
{
    int h264_fd = open(path.c_str(), O_RDONLY, 0700);
    if (h264_fd < 0)
    {
        printf("open %s fail\n", path.c_str());
        return;
    }

    h264_size = lseek(h264_fd, 0, SEEK_END);
    lseek(h264_fd, 0, SEEK_SET);

    h264_buffer = (uint8_t *)malloc(h264_size);
    if (!h264_buffer)
    {
        close(h264_fd);
        return;
    }

    memset(h264_buffer, 0, h264_size);
    read(h264_fd, h264_buffer, h264_size);
    close(h264_fd);

    pos = find_start_code(h264_buffer, 0, h264_size);
    if (pos < 0)
    {
        printf("find start code error\n");
        return;
    }

    printf("h264 size = %zu\n", h264_size);
}

SimulateCode::~SimulateCode()
{
    if (h264_buffer)
        free(h264_buffer);

    if (last_sps)
        free(last_sps);

    if (last_pps)
        free(last_pps);
}

int SimulateCode::find_start_code(uint8_t *buf, int start, int end)
{
    for (int i = start; i < end - 4; i++)
    {
        if (buf[i] == 0 && buf[i + 1] == 0)
        {
            if (buf[i + 2] == 1 || (buf[i + 2] == 0 && buf[i + 3] == 1))
            {
                return i;
            }
        }
    }
    return -1;
}

int SimulateCode::h264_readnalu(uint8_t *p, size_t *size, int *type)
{
    if (!h264_buffer)
        return -1;

    int pos1 = find_start_code(h264_buffer, pos + 4, h264_size);
    if (pos1 < 0)
        pos1 = h264_size;

    int nalu_size = pos1 - pos;
    uint8_t *nalu = h264_buffer + pos;

    int start_code_len = 3;
    if (nalu[2] == 1)
        start_code_len = 3;
    else
        start_code_len = 4;

    int nalu_type = nalu[start_code_len] & 0x1f;

    // 保存 SPS
    if (nalu_type == 7)
    {
        if (last_sps)
            free(last_sps);

        last_sps = (uint8_t *)malloc(nalu_size);
        memcpy(last_sps, nalu, nalu_size);
        last_sps_size = nalu_size;

        pos = pos1;
        return 0;
    }

    // 保存 PPS
    if (nalu_type == 8)
    {
        if (last_pps)
            free(last_pps);

        last_pps = (uint8_t *)malloc(nalu_size);
        memcpy(last_pps, nalu, nalu_size);
        last_pps_size = nalu_size;

        pos = pos1;
        return 0;
    }

    // IDR 帧 -> 自动注入 SPS PPS
    if (nalu_type == 5 && last_sps && last_pps)
    {
        size_t total = 0;

        memcpy(p + total, last_sps, last_sps_size);
        total += last_sps_size;

        memcpy(p + total, last_pps, last_pps_size);
        total += last_pps_size;

        memcpy(p + total, nalu, nalu_size);
        total += nalu_size;

        *size = total;
        *type = 5;

        pos = pos1;
        return 1;
    }

    // 普通帧
    memcpy(p, nalu, nalu_size);
    *size = nalu_size;
    *type = nalu_type;

    pos = pos1;

    // 循环播放
    if (pos >= h264_size)
    {
        pos = find_start_code(h264_buffer, 0, h264_size);
    }

    return 1;
}