#include "AlsaUtils.hpp"
#include "sip_sdk_aec.h"
#include <mutex>
#include "PCMBufferQueue.h"
#include <thread>

static std::mutex mutex_play;
static std::mutex mutex_read;

static snd_pcm_t *play_pcm = nullptr;
static snd_pcm_uframes_t play_frames; // 播放帧数量

static snd_pcm_t *read_pcm = nullptr;
static snd_pcm_uframes_t read_frames; // 读取帧数量

static int open_play()
{
    std::lock_guard<std::mutex> lock(mutex_play);
    snd_pcm_hw_params_t *params;
    snd_pcm_format_t format;
    int format_bits = 16;
    int channels = 1;
    unsigned int default_samples = 16000;
    unsigned int rate = default_samples;
    snd_pcm_uframes_t tmp_buf_size;
    snd_pcm_uframes_t tmp_period_size;
    std::string devName = "play_pcm";
    int result;
    /* Open PCM for playback */
    printf("open_playback: Open playback device '%s'\n", devName.c_str());
    result = snd_pcm_open(&play_pcm, devName.c_str(), SND_PCM_STREAM_PLAYBACK, 0);
    if (result < 0)
    {
        printf("%s:%d \n", __FUNCTION__, __LINE__);
        goto error;
    }

    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(play_pcm, params);

    /* Set interleaved mode */
    snd_pcm_hw_params_set_access(play_pcm, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Set format */
    switch (format_bits)
    {
    case 8:
        printf("open_playback: set format SND_PCM_FORMAT_S8\n");
        format = SND_PCM_FORMAT_S8;
        break;
    case 16:
        printf("open_playback: set format SND_PCM_FORMAT_S16_LE\n");
        format = SND_PCM_FORMAT_S16_LE;
        break;
    case 24:
        printf("open_playback: set format SND_PCM_FORMAT_S24_LE\n");
        format = SND_PCM_FORMAT_S24_LE;
        break;
    case 32:
        printf("open_playback: set format SND_PCM_FORMAT_S32_LE\n");
        format = SND_PCM_FORMAT_S32_LE;
        break;
    default:
        printf("open_playback: set format SND_PCM_FORMAT_S16_LE\n");
        format = SND_PCM_FORMAT_S16_LE;
        break;
    }
    snd_pcm_hw_params_set_format(play_pcm, params, format);

    /* Set number of channels */
    printf("open_playback: set channels: %d\n", channels);
    result = snd_pcm_hw_params_set_channels(play_pcm, params, channels);
    if (result < 0)
    {
        printf("Unable to set a channel count of %d for playback device '%s'\n", channels, devName);
        goto error;
    }

    /* Set clock rate */
    printf("open_playback: set clock rate: %d\n", rate);
    snd_pcm_hw_params_set_rate_near(play_pcm, params, &rate, NULL);
    printf("open_playback: clock rate set to: %d\n", rate);

    /* Set period size to samples_per_frame frames. */
    play_frames = (snd_pcm_uframes_t)(default_samples * 20 / 1000) / channels;
    printf("open_playback: set period size: %d\n", play_frames);
    tmp_period_size = play_frames;
    snd_pcm_hw_params_set_period_size_near(play_pcm, params, &tmp_period_size, NULL);
    printf("open_playback: period size set to: %d\n", tmp_period_size);

    /* Set the sound device buffer size and latency */
    tmp_buf_size = (rate / 1000) * 140;
    if (tmp_buf_size < tmp_period_size * 2)
        tmp_buf_size = tmp_period_size * 2;

    snd_pcm_hw_params_set_buffer_size_near(play_pcm, params, &tmp_buf_size);
    printf("open_playback: buffer size set to: %d\n", (int)tmp_buf_size);

    /* Activate the parameters */
    result = snd_pcm_hw_params(play_pcm, params);
    if (result < 0)
    {
        printf("snd_pcm_hw_params error: %d\n", result);
        goto error;
    }
    printf("Opened device alsa(%s) for playing, sample rate=%d, ch=%d, bits=%d, period size=%d frames\n",
           devName,
           rate, channels,
           format_bits,
           play_frames);
    return 0;
error:
    if (play_pcm)
    {
        snd_pcm_drain(play_pcm);
        snd_pcm_close(play_pcm);
        play_pcm = NULL;
    }
    return -1;
}
static int open_read()
{
    std::lock_guard<std::mutex> lock(mutex_read);
    snd_pcm_hw_params_t *params;
    snd_pcm_format_t format;
    int format_bits = 16;
    int channels = 1;
    unsigned int default_samples = 16000;
    unsigned int rate = default_samples;
    snd_pcm_uframes_t tmp_buf_size;
    snd_pcm_uframes_t tmp_period_size;
    std::string devName = "play_pcm";
    int result;

    /* Open PCM for capture */
    printf("open_playback: Open playback device '%s'\n", devName.c_str());
    result = snd_pcm_open(&read_pcm, devName.c_str(), SND_PCM_STREAM_CAPTURE, 0);
    if (result < 0)
    {
        printf("%s:%d \n", __FUNCTION__, __LINE__);
        goto error;
    }

    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(read_pcm, params);

    /* Set interleaved mode */
    snd_pcm_hw_params_set_access(read_pcm, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Set format */
    switch (format_bits)
    {
    case 8:
        printf("open_playback: set format SND_PCM_FORMAT_S8\n");
        format = SND_PCM_FORMAT_S8;
        break;
    case 16:
        printf("open_playback: set format SND_PCM_FORMAT_S16_LE\n");
        format = SND_PCM_FORMAT_S16_LE;
        break;
    case 24:
        printf("open_playback: set format SND_PCM_FORMAT_S24_LE\n");
        format = SND_PCM_FORMAT_S24_LE;
        break;
    case 32:
        printf("open_playback: set format SND_PCM_FORMAT_S32_LE\n");
        format = SND_PCM_FORMAT_S32_LE;
        break;
    default:
        printf("open_playback: set format SND_PCM_FORMAT_S16_LE\n");
        format = SND_PCM_FORMAT_S16_LE;
        break;
    }
    snd_pcm_hw_params_set_format(read_pcm, params, format);

    /* Set number of channels */
    printf("open_capture: set channels: %d \n", channels);
    result = snd_pcm_hw_params_set_channels(read_pcm, params, channels);
    if (result < 0)
    {
        printf("Unable to set a channel count of %d for capture device '%s'\n", channels, devName.c_str());
        goto error;
    }

    /* Set clock rate */
    printf("open_capture: set clock rate: %d\n", rate);
    snd_pcm_hw_params_set_rate_near(read_pcm, params, &rate, NULL);
    printf("open_capture: clock rate set to: %d\n", rate);

    /* Set period size to samples_per_frame frames. */
    read_frames = (snd_pcm_uframes_t)(default_samples * 20 / 1000) / channels;
    printf("open_capture: set period size: %d \n", read_frames);
    tmp_period_size = read_frames;
    snd_pcm_hw_params_set_period_size_near(read_pcm, params, &tmp_period_size, NULL);
    printf("open_capture: period size set to: %d \n", tmp_period_size);

    /* Set the sound device buffer size and latency */
    tmp_buf_size = (rate / 1000) * 100;
    if (tmp_buf_size < tmp_period_size * 2)
        tmp_buf_size = tmp_period_size * 2;
    snd_pcm_hw_params_set_buffer_size_near(read_pcm, params, &tmp_buf_size);

    /* Activate the parameters */
    result = snd_pcm_hw_params(read_pcm, params);
    if (result < 0)
    {
        goto error;
    }
    return 0;
error:
    if (read_pcm)
    {
        snd_pcm_drain(read_pcm);
        snd_pcm_close(read_pcm);
        read_pcm = NULL;
    }
    return -1;
}

// 线程执行的函数
void threadFunction()
{
    while (1)
    {
        std::lock_guard<std::mutex> lock(mutex_read);
        snd_pcm_sframes_t result;
        // 去alsa驱动中取数据
        if (!read_pcm)
        {
            return;
        }
        uint8_t buf[640];
        result = snd_pcm_readi(read_pcm, buf, 256);
        if (result == -EPIPE)
        {
            printf("alsa read: overrun! \n");
            snd_pcm_prepare(read_pcm);
            continue;
        }
        else if (result < 0)
        {
            printf("alsa read: error reading data!\n");
            continue;
        }
        pcmBufferQueue.enqueue(buf, result * 2);
    }
}

int ualsa::open()
{
    open_play();
    open_read();
    // 启动线程
    // std::thread t(threadFunction);
    // t.detach();
    return 0;
}

int ualsa::play(uint8_t *data, size_t size)
{
    std::lock_guard<std::mutex> lock(mutex_play);
    if (!play_pcm)
    {
        return AUDIO_TIME_TYPE_INTERNAL;
    }
    int result = snd_pcm_writei(play_pcm, data, play_frames);
    if (result == -EPIPE)
    {
        printf("alsa play: underrun! \n");
        snd_pcm_prepare(play_pcm);
        return AUDIO_TIME_TYPE_AUTONOMIC;
    }
    else if (result < 0)
    {
        printf("alsa play: error writing data! \n");
        return AUDIO_TIME_TYPE_INTERNAL;
    }
    return AUDIO_TIME_TYPE_AUTONOMIC;
}

audio_media_frame *ualsa::read()
{
    std::lock_guard<std::mutex> lock(mutex_read);
    audio_media_frame *media_frame = NULL;
    snd_pcm_sframes_t result;
    // 去alsa驱动中取数据
    if (!read_pcm)
    {
        goto error;
    }
    media_frame = alloc_audio_media_frame(read_frames * 2);
    if (media_frame == NULL || media_frame->buf == NULL)
    {
        goto error;
    }
    result = snd_pcm_readi(read_pcm, media_frame->buf, read_frames);
    if (result == -EPIPE)
    {
        printf("alsa read: overrun! \n");
        snd_pcm_prepare(read_pcm);
        goto error;
    }
    else if (result < 0)
    {
        printf("alsa read: error reading data!\n");
        goto error;
    }
    return media_frame;
error:
    free_audio_media_frame(media_frame);
    return NULL;
}

void ualsa::read(void *buf, int size)
{
    std::lock_guard<std::mutex> lock(mutex_read);
    snd_pcm_sframes_t result;
    // 去alsa驱动中取数据
    if (!read_pcm)
    {
        return;
    }
    result = snd_pcm_readi(read_pcm, buf, read_frames);
    if (result == -EPIPE)
    {
        printf("alsa read: overrun! \n");
        snd_pcm_prepare(read_pcm);
        return;
    }
    else if (result < 0)
    {
        printf("alsa read: error reading data!\n");
        return;
    }
}

static void close_read()
{
    std::lock_guard<std::mutex> lock(mutex_read);
    if (read_pcm)
    {
        snd_pcm_drain(read_pcm);
        snd_pcm_close(read_pcm);
        read_pcm = nullptr;
    }
};

static void close_play()
{
    std::lock_guard<std::mutex> lock(mutex_play);
    if (play_pcm)
    {
        snd_pcm_drain(play_pcm);
        snd_pcm_close(play_pcm);
        play_pcm = nullptr;
    }
};

int ualsa::close()
{
    close_play();
    close_read();
}