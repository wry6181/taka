#include "logger.h"
#include "asserts.h"
#include "platform/platform.h"

// TODO: temporary
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 init_logging() {
    return TRUE;
}

void shutdown_logging() {
}

void log_output(log_level level, const char* massage, ...) {
    const char* prefixes[6] = {"[FATAL]", "[ERROR]", "[WARINING]", "[INFO]", "[DEBUG]", "[TRACE]"};
    b8 is_error = level < LOG_LEVEL_WARNING;

    const i32 msg_size = 32000;
    char out_buffer[msg_size];

    memset(out_buffer, 0, sizeof(out_buffer));

    __builtin_va_list arg_parms;
    va_start(arg_parms, massage);

    vsnprintf(out_buffer, msg_size, massage, arg_parms);

    va_end(arg_parms);

    char out_pre_buffer[msg_size];

    memset(out_pre_buffer, 0, sizeof(out_pre_buffer));

    sprintf(out_pre_buffer, "%s %s", prefixes[level], out_buffer);

    if(is_error) {
        platform_console_write_error(out_pre_buffer, level);
    } else {
        platform_console_write(out_pre_buffer, level);
    }

}

void report_assertion_failure(const char* expression, const char* message, const char* file, u32 line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d", expression, message, file, line);
}
