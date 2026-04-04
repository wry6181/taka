#include "platform/platform.h"

#if T_PLATFORM_IOS

#include "core/logger.h"

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#include <stdlib.h>
#include <string.h>

typedef struct internal_state internal_state;
struct internal_state {
    UIWindow* window;
    UIViewController* view_controller;
};

static f64 clock_frequency;
static NSDate* start_time;

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

    state->view_controller = [[UIViewController alloc] init];
    state->view_controller.view.backgroundColor = [UIColor blackColor];
    
    state->window.rootViewController = state->view_controller;
    [state->window makeKeyAndVisible];

    clock_frequency = 1.0;
    start_time = [NSDate date];

    return TRUE;
}

void destroy_platform(platform_state* plat_state) {
    internal_state* state = (internal_state*)plat_state->internal_state;
    if (state->window) {
        [state->window release];
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

#endif
