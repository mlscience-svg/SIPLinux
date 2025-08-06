#ifndef CURL_LICENSE_H_
#define CURL_LICENSE_H_
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif

    bool sync_token(const char *url, const char *client_id, const char *secret, char *info);

#ifdef __cplusplus
}
#endif

#endif