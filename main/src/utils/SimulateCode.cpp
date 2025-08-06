#include <cstdlib>
#include "SimulateCode.hpp"

SimulateCode::SimulateCode(std::string path)
{
    h264_fd = open(path.c_str(), O_RDONLY, 0700);
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
        printf("find start code error: %d\n", pos);
        return;
    }
    printf("pos = %d\n", pos);
    printf("h264 size = %zu\n", h264_size);
}

SimulateCode::~SimulateCode()
{
    if (h264_buffer)
    {
        free(h264_buffer);
    }
}

int SimulateCode::find_start_code(uint8_t *buf, int start, int end)
{
    int pos = -1, i = 0;

    for (i = start; i < end - 4; i++)
    {
        if (buf[i] == 0 && buf[i + 1] == 0)
        {
            if (buf[i + 2] == 1 || (i + 3 < end && buf[i + 2] == 0 && buf[i + 3] == 1))
            {
                pos = i;
                break;
            }
        }

        // if (buf[i] == 0 && buf[i+1] == 0  && buf[i+2] == 1)
        //{
        //	pos = i;
        //	break;
        // }
    }
    return pos;
}

int SimulateCode::h264_readnalu(uint8_t *p, size_t *size, int *type)
{
    if (h264_buffer == NULL)
    {
        return -1;
    }

    int pos1 = find_start_code(h264_buffer, pos + 4, h264_size);
    if (pos1 < 0)
    {
        pos1 = h264_size;
    }

    if (pos1 - pos)
    {
        // p[0] = 0; //start code is 001, here to 0001
        memcpy(p, h264_buffer + pos, pos1 - pos);
        *size = pos1 - pos;
        *type = p[4] & 0x1f;

        // printf("pos0=%d pos1=%d type=%d\n", pos, pos1, *type);

        pos = pos1;
        return 1;
    }

    if ((pos1 == h264_size))
    {
        pos = find_start_code(h264_buffer, 0, h264_size);
        printf("pos = %d\n", pos);
    }

    return 0;
}
