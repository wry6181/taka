#include "platform/platform.h"

#if T_PLATFORM_MAC

#include "core/logger.h"

#include <AppKit/AppKit.h>
#include <Foundation/Foundation.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <stdlib.h>
#include <string.h>

typedef struct internal_state internal_state;
struct internal_state {
    NSWindow* window;
};

static f64 clock_frequency;
static NSDate* start_time;

@interface AppDelegate : NSObject <NSApplicationDelegate>
@end

@implementation AppDelegate
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender {
    return NO;
}
@end

b8 init_platform(mem_arena* arena, platform_state* plat_state, const char* app_name, u32 x, u32 y, u32 width, u32 height) {
    @autoreleasepool {
        plat_state->internal_state = PUSH_STRUCT_ZERO(arena, internal_state);
        internal_state* state = (internal_state*)plat_state->internal_state;

        static BOOL app_initialized = NO;
        if (!app_initialized) {
            [NSApplication sharedApplication];
            app_initialized = YES;
        }

        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        NSRect content_rect = NSMakeRect(x, y, width, height);

        u32 window_style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
                           NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;

        state->window = [[NSWindow alloc]
            initWithContentRect:content_rect
                      styleMask:window_style
                        backing:NSBackingStoreBuffered
                          defer:NO];

        [state->window setTitle:[NSString stringWithUTF8String:app_name]];
        [state->window center];
        [state->window makeKeyAndOrderFront:nil];
        [state->window orderFront:nil];

        [NSApp setDelegate:[[AppDelegate alloc] init]];
        [NSApp finishLaunching];
        [NSApp activateIgnoringOtherApps:YES];

        NSMenu* main_menu = [[NSMenu alloc] init];
        NSMenuItem* app_item = [[NSMenuItem alloc] init];
        NSMenu* app_menu = [[NSMenu alloc] init];
        [app_item setSubmenu:app_menu];
        [main_menu addItem:app_item];
        [NSApp setMainMenu:main_menu];

        clock_frequency = 1.0;
        start_time = [NSDate date];
    }

    return TRUE;
}

void destroy_platform(platform_state* plat_state) {
    internal_state* state = (internal_state*)plat_state->internal_state;
    if (state->window) {
        [state->window close];
        state->window = nil;
    }
}

b8 platform_pump_messages(platform_state* plat_state) {
    @autoreleasepool {
        NSEvent* event;

        while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                         untilDate:nil
                                            inMode:NSDefaultRunLoopMode
                                           dequeue:YES])) {
            [NSApp sendEvent:event];
        }

        return TRUE;
    }
}

void platform_console_write(const char* message, u8 colour) {
    const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m\n", colour_strings[colour], message);
    fflush(stdout);
}

void platform_console_write_error(const char* message, u8 colour) {
    const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m\n", colour_strings[colour], message);
    fflush(stdout);
}

f64 platform_get_absolute_time() {
    NSTimeInterval now = [[NSDate date] timeIntervalSince1970];
    return now;
}

void platform_sleep(u64 ms) {
    usleep(ms * 1000);
}

void* platform_allocate(u64 size, b8 aligned) {
    if (aligned) {
        void* ptr = NULL;
        posix_memalign(&ptr, 16, size);
        return ptr;
    }
    return malloc(size);
}

void platform_free(void* block, b8 aligned) {
    free(block);
}

void* platform_zero_memory(void* block, u64 size) {
    memset(block, 0, size);
    return block;
}

void* platform_copy_memory(void* dest, const void* source, u64 size) {
    memcpy(dest, source, size);
    return dest;
}

void* platform_set_memory(void* dest, i32 value, u64 size) {
    memset(dest, value, size);
    return dest;
}

#endif
