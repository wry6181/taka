#pragma once

#include "core/logger.h"
#include <types.h>

#define ASSERT_ENABLE

#ifdef ASSERT_ENABLE
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

void report_assertion_failure(const char* expression, const char* massage, const char* file, u32 line);

#define T_ASSERT(exp)                                                   \
{                                                                       \
    if(exp) {                                                           \
    } else {                                                            \
        report_assertion_failure(#exp, "", __FILE__, __LINE__);            \
        debugBreak();                                                   \
    }                                                                   \
}

#define T_ASSERT_MSG(exp, message)                                      \
{                                                                       \
    if(exp) {                                                           \
    } else {                                                            \
        report_assertion_failure(#exp, message, __FILE__, __LINE__);       \
        debugBreak();                                                   \
    }                                                                   \
}

#ifdef _DEBUG
#define T_ASSERT_DEBUG(exp)                                             \
{                                                                       \
    if(exp) {                                                           \
    } else {                                                            \
        report_assertion_failure(#exp, "", __FILE__, __LINE__);            \
        debugBreak();                                                   \
    }                                                                   \
}
#else
#define T_ASSERT_DEBUG(exp)
#endif

#else
#define T_ASSERT(exp)
#define T_ASSERT_MSG(exp, message)
#define T_ASSERT_DEBUG(exp)
#endif
