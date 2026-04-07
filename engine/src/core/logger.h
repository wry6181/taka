#pragma once

#include <types.h>

#define LOG_WARNING_ENABLE 1
#define LOG_INFO_ENABLE 1
#define LOG_DEBUG_ENABLE 1
#define LOG_TRACE_ENABLE 1

#if T_RELEASE == 1
#define LOG_DEBUG_ENABLE 0
#define LOG_TRACE_ENABLE 0
#endif

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE,
} log_level;

b8 init_logging();
void distroy_logging();

void log_output(log_level level, const char* msg, ...);

#define T_FATAL(msg, ...) log_output(LOG_LEVEL_FATAL, msg, ##__VA_ARGS__);
#define T_ERROR(msg, ...) log_output(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__);

#if LOG_WARNING_ENABLE == 1
#define T_WARNING(msg, ...) log_output(LOG_LEVEL_WARNING, msg, ##__VA_ARGS__);
#else
#define T_WARNING(msg, ...)
#endif

#if LOG_INFO_ENABLE == 1
#define T_INFO(msg, ...) log_output(LOG_LEVEL_INFO, msg, ##__VA_ARGS__);
#else
#define T_INFO(msg, ...)
#endif

#if LOG_DEBUG_ENABLE == 1
#define T_DEBUG(msg, ...) log_output(LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__);
#else
#define T_DEBUG(msg, ...)
#endif

#if LOG_TRACE_ENABLE == 1
#define T_TRACE(msg, ...) log_output(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__);
#else
#define T_TRACE(msg, ...)
#endif
