#include "platform/platform.h"

#if T_PLATFORM_MAC

#include "core/logger.h"

#include <AppKit/AppKit.h>
#include <Foundation/Foundation.h>
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
- (void)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender {
}
@end

b8 init_platform(mem_arena* arena, platform_state* plat_state, const char* app_name, u32 x, u32 y, u32 width, u32 height) {
    plat_state->internal_state = PUSH_STRUCT_ZERO(arena, internal_state);
    internal_state* state = (internal_state*)plat_state->internal_state;

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
    [state->window makeKeyAndOrderFront:NSApp];

    NSString* ns_app_name = [NSString stringWithUTF8String:app_name];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];

    NSTimeInterval now = [[NSDate date] timeIntervalSince1970];
    clock_frequency = 1.0;
    start_time = [[NSDate date] retain];

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
    NSEvent* event;
    b8 quit_flagged = FALSE;

    while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                     untilDate:nil
                                        inMode:NSDefaultRunLoopMode
                                       dequeue:YES])) {
        switch ([event type]) {
            case NSEventTypeKeyDown:
            case NSEventTypeKeyUp:
            case NSEventTypeFlagsChanged:
                break;
            case NSEventTypeMouseMoved:
            case NSEventTypeLeftMouseDown:
            case NSEventTypeLeftMouseUp:
            case NSEventTypeRightMouseDown:
            case NSEventTypeRightMouseUp:
            case NSEventTypeOtherMouseDown:
            case NSEventTypeOtherMouseUp:
                break;
            case NSEventTypeLeftMouseDragged:
            case NSEventTypeRightMouseDragged:
            case NSEventTypeOtherMouseDragged:
                break;
            case NSEventTypeScrollWheel:
                break;
            case NSEventTypeQuit:
                quit_flagged = TRUE;
                break;
            default:
                break;
        }

        [NSApp sendEvent:event];
    }

    return !quit_flagged;
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

#endif
