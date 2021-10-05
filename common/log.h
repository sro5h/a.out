#ifndef COMMON_LOG_H
#define COMMON_LOG_H

#include <log.c/src/log.h>

#define aout_logt(...) log_trace(__VA_ARGS__)
#define aout_logd(...) log_debug(__VA_ARGS__)
#define aout_logi(...) log_info(__VA_ARGS__)
#define aout_logw(...) log_warn(__VA_ARGS__)
#define aout_loge(...) log_error(__VA_ARGS__)
#define aout_logf(...) log_fatal(__VA_ARGS__)

#endif
