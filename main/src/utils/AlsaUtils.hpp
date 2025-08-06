#ifndef ALSA_UTILS_H_
#define ALSA_UTILS_H_
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asoundlib.h"
#include "sip_sdk_media.h"

namespace ualsa
{
    int open();
    int play(uint8_t *data, size_t size);
    audio_media_frame *read();
    void read(void *buf, int size);
    int close();
}

#endif