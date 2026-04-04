#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) UIWindow* window;
@property (strong, nonatomic) UIViewController* view_controller;
@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    CGRect screen_bounds = [[UIScreen mainScreen] bounds];
    self.window = [[UIWindow alloc] initWithFrame:screen_bounds];

    self.view_controller = [[UIViewController alloc] init];
    self.view_controller.view.backgroundColor = [UIColor blackColor];
    
    self.window.rootViewController = self.view_controller;
    [self.window makeKeyAndVisible];
    
    NSLog(@"Taka iOS platform initialized");
    
    return YES;
}

@end

int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
