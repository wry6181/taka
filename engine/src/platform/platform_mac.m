#include "platform/platform.h"

#if T_PLATFORM_MAC

#include "core/logger.h"
#include "core/event.h"
#include "core/input.h"

#include <AppKit/AppKit.h>
#include <Foundation/Foundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <stdlib.h>
#include <string.h>

typedef struct internal_state internal_state;
struct internal_state {
    NSWindow* window;
    CGEventSourceRef event_source;
};

static f64 clock_frequency;
static NSDate* start_time;

static keys mac_translate_keycode(u16 keycode, u32 modifiers);

@interface AppDelegate : NSObject <NSApplicationDelegate>
@end

@implementation AppDelegate
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender {
    return YES;  // Quit when window closed
}
- (void)applicationWillTerminate:(NSApplication *)sender {
    // Will be called when quit is requested
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
        
        NSLog(@"Window created: %@", state->window);

        [NSApp setDelegate:[[AppDelegate alloc] init]];
        [NSApp finishLaunching];
        [NSApp activateIgnoringOtherApps:YES];
        
        // Use local event monitors - work when window is focused
        [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyDown | NSEventMaskKeyUp
                                                        handler:^NSEvent*(NSEvent* event) {
            if ([event type] == NSEventTypeKeyDown || [event type] == NSEventTypeKeyUp) {
                keys key = mac_translate_keycode([event keyCode], [event modifierFlags]);
                if (key != 0) {
                    input_process_key(key, ([event type] == NSEventTypeKeyDown));
                }
            }
            return event;
        }];

        // Run NSApp run loop on main thread (required for event monitors)
        [NSApp run];

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
        // Run the run loop for a short time to process events
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.001, false);
        return TRUE;
    }
}

void platform_quit(void) {
    [NSApp stop:nil];
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

static keys mac_translate_keycode(u16 keycode, u32 modifiers) {
    switch (keycode) {
        case 0x24: return KEY_ENTER;
        case 0x30: return KEY_TAB;
        case 0x31: return KEY_SPACE;
        case 0x33: return KEY_BACKSPACE;
        case 0x35: return KEY_ESCAPE;
        case 0x7A: return KEY_F1;
        case 0x78: return KEY_F2;
        case 0x63: return KEY_F3;
        case 0x76: return KEY_F4;
        case 0x60: return KEY_F5;
        case 0x61: return KEY_F6;
        case 0x62: return KEY_F7;
        case 0x64: return KEY_F8;
        case 0x65: return KEY_F9;
        case 0x6D: return KEY_F10;
        case 0x67: return KEY_F11;
        case 0x6F: return KEY_F12;

        case 0x7E: return KEY_UP;
        case 0x7D: return KEY_DOWN;
        case 0x7B: return KEY_LEFT;
        case 0x7C: return KEY_RIGHT;

        case 0x74: return KEY_PRIOR;
        case 0x79: return KEY_NEXT;
        case 0x73: return KEY_HOME;
        case 0x77: return KEY_END;

        case 0x00: return KEY_A;
        case 0x0B: return KEY_B;
        case 0x08: return KEY_C;
        case 0x02: return KEY_D;
        case 0x0E: return KEY_E;
        case 0x03: return KEY_F;
        case 0x05: return KEY_G;
        case 0x04: return KEY_H;
        case 0x22: return KEY_I;
        case 0x26: return KEY_J;
        case 0x28: return KEY_K;
        case 0x25: return KEY_L;
        case 0x2E: return KEY_M;
        case 0x2D: return KEY_N;
        case 0x1F: return KEY_O;
        case 0x23: return KEY_P;
        case 0x0C: return KEY_Q;
        case 0x0F: return KEY_R;
        case 0x01: return KEY_S;
        case 0x11: return KEY_T;
        case 0x20: return KEY_U;
        case 0x09: return KEY_V;
        case 0x0D: return KEY_W;
        case 0x07: return KEY_X;
        case 0x10: return KEY_Y;
        case 0x06: return KEY_Z;

        case 0x52: return KEY_NUMPAD0;
        case 0x53: return KEY_NUMPAD1;
        case 0x54: return KEY_NUMPAD2;
        case 0x55: return KEY_NUMPAD3;
        case 0x56: return KEY_NUMPAD4;
        case 0x57: return KEY_NUMPAD5;
        case 0x58: return KEY_NUMPAD6;
        case 0x59: return KEY_NUMPAD7;
        case 0x5B: return KEY_NUMPAD8;
        case 0x5C: return KEY_NUMPAD9;

        case 0x38: return KEY_LSHIFT;
        case 0x3C: return KEY_RSHIFT;
        case 0x3B: return KEY_LCONTROL;
        case 0x3E: return KEY_RCONTROL;
        case 0x3A: return KEY_LMENU;
        case 0x3D: return KEY_RMENU;

        case 0x39: return KEY_CAPITAL;
        case 0x42: return KEY_NUMLOCK;
        case 0x43: return KEY_SCROLL;

        case 0x72: return KEY_INSERT;
        case 0x75: return KEY_DELETE;

        case 0x18: return KEY_MULTIPLY;

        case 0x29: return KEY_SEMICOLON;
        case 0x27: if(modifiers & NSEventModifierFlagShift) return KEY_MINUS; return KEY_COMMA;

        default: return 0;
    }
}

#endif
