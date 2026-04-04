#include "types.h"
#include <core/asserts.h>
#include <core/logger.h>
#include <limits.h>
#include <platform/platform.h>


int main(void) {
    T_FATAL("A test message: %f", 3.14f);
    T_ERROR("A test message: %f", 3.14f);
    T_WARNING ("A test message: %f", 3.14f);
    T_INFO("A test message: %f", 3.14f);
    T_DEBUG("A test message: %f", 3.14f);
    T_TRACE("A test message: %f", 3.14f);

    T_ASSERT(2 < 3);
    T_ASSERT_MSG(3 < 5, "3 smaller than 5");

    mem_arena* arena = arena_create(MByte(100));

    platform_state state = {0};
    if(init_platform(arena, &state, "TAKA APP", 100, 100, 1280, 720)) {
        while(TRUE) {
            platform_pump_messages(&state);
        }
    }
    destroy_platform(&state);
}
