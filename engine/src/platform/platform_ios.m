#include "platform/platform.h"

#if T_PLATFORM_IOS

#include "core/logger.h"
#include "core/input.h"

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#include <stdlib.h>
#include <string.h>

typedef struct internal_state internal_state;
struct internal_state {
    UIWindow* window;
    UIViewController* view_controller;
    UIView* touch_view;
};

static f64 clock_frequency;
static NSDate* start_time;

@interface TouchInputView : UIView
@end

@implementation TouchInputView

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event {
    for (UITouch* touch in touches) {
        CGPoint location = [touch locationInView:self];
        input_process_button(BUTTON_LEFT, TRUE);
        input_process_mouse_move((i16)location.x, (i16)location.y);
    }
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event {
    for (UITouch* touch in touches) {
        CGPoint location = [touch locationInView:self];
        input_process_mouse_move((i16)location.x, (i16)location.y);
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event {
    for (UITouch* touch in touches) {
        CGPoint location = [touch locationInView:self];
        input_process_button(BUTTON_LEFT, FALSE);
        input_process_mouse_move((i16)location.x, (i16)location.y);
    }
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event {
    for (UITouch* touch in touches) {
        input_process_button(BUTTON_LEFT, FALSE);
    }
}

@end

@interface AppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) UIWindow* window;
@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
}

- (void)applicationWillTerminate:(UIApplication *)application {
}

@end

b8 init_platform(mem_arena* arena, platform_state* plat_state, const char* app_name, u32 x, u32 y, u32 width, u32 height) {
    plat_state->internal_state = PUSH_STRUCT_ZERO(arena, internal_state);
    internal_state* state = (internal_state*)plat_state->internal_state;

    CGRect screen_bounds = [[UIScreen mainScreen] bounds];
    state->window = [[UIWindow alloc] initWithFrame:screen_bounds];

    state->touch_view = [[TouchInputView alloc] initWithFrame:screen_bounds];
    state->touch_view.backgroundColor = [UIColor blackColor];
    state->touch_view.multipleTouchEnabled = YES;
    state->touch_view.userInteractionEnabled = YES;

    state->view_controller = [[UIViewController alloc] init];
    state->view_controller.view.backgroundColor = [UIColor blackColor];
    [state->view_controller.view addSubview:state->touch_view];
    
    state->window.rootViewController = state->view_controller;
    [state->window makeKeyAndVisible];

    clock_frequency = 1.0;
    start_time = [NSDate date];

    return TRUE;
}

void destroy_platform(platform_state* plat_state) {
    internal_state* state = (internal_state*)plat_state->internal_state;
    if (state->window) {
        state->window = nil;
    }
}

b8 platform_pump_messages(platform_state* plat_state) {
    return TRUE;
}

void platform_console_write(const char* message, u8 colour) {
    const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m\n", colour_strings[colour], message);
    fflush(stdout);
}

void platform_console_write_error(const char* message, u8 colour) {
    const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    fprintf(stderr, "\033[%sm%s\033[0m\n", colour_strings[colour], message);
    fflush(stderr);
}

f64 platform_get_absolute_time() {
    return [[NSDate date] timeIntervalSince1970];
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
