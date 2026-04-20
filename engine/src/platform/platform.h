#include "types.h"
#include "core/arena.h"

typedef struct platform_state platform_state;
struct platform_state {
    void* internal_state;
};

b8 init_platform(mem_arena* arena, platform_state* plat_state, const char* app_name, u32 x, u32 y, u32 width, u32 height);

void destroy_platform(platform_state* plat_state);

b8 platform_pump_messages(platform_state* plat_state);

void platform_quit(void);

void* platform_allocate(u64 size, b8 aligned);
void platform_free(void* block, b8 aligned);
void* platform_zero_memory(void* block, u64 size);
void* platform_copy_memory(void* dest, const void* source, u64 size);
void* platform_set_memory(void* dest, i32 value, u64 size);

void platform_console_write(const char* message, u8 colour);
void platform_console_write_error(const char* message, u8 colour);

void platform_sleep(u64 ms);
