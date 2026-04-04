#include "types.h"
#include "core/arena.h"

typedef struct platform_state platform_state;
struct platform_state {
    void* internal_state;
};

b8 init_platform(mem_arena* arena, platform_state* plat_state, const char* app_name, u32 x, u32 y, u32 width, u32 height);

void destroy_platform(platform_state* plat_state);

b8 platform_pump_messages(platform_state* plat_state);

void platform_console_write(const char* message, u8 colour);
void platform_console_write_error(const char* message, u8 colour);
