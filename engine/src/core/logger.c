#include "logger.h"
#include "asserts.h"

// TODO: temporary
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 initialize_logging() {
    return TRUE;
}

void shutdown_logging() {
}

void log_output(log_level level, const char* massage, ...) {
    const char* prefixes[6] = {"[FATAL]", "[ERROR]", "[WARINING]", "[INFO]", "[DEBUG]", "[TRACE]"};
    b8 is_error = level < 2;

    char out_buffer[32000];

    memset(out_buffer, 0, sizeof(out_buffer));

    __builtin_va_list arg_parms;
    va_start(arg_parms, massage);

    vsnprintf(out_buffer, 32000, massage, arg_parms);

    va_end(arg_parms);

    char out_pre_buffer[32000];

    memset(out_pre_buffer, 0, sizeof(out_pre_buffer));

    sprintf(out_pre_buffer, "%s %s\n", prefixes[level], out_buffer);

    printf("%s", out_pre_buffer);

}

void report_assertion_failure(const char* expression, const char* message, const char* file, u32 line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
}
