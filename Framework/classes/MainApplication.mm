#import "MainApplication.h"

@interface MainApplication()
-(MainApplication*)init;
-(void)dealloc;
@end

@implementation MainApplication
+(MainApplication*) getApp
{
	if( app == NULL)
	{
		app = [[MainApplication alloc] init];
	}
	return app;	
}
+(void)cleanUp
{
	delete app;
	app = NULL;
}
-(MainApplication*)init
{
	//Initilize the event queues
	return self;
}
-(void)dealloc
{
	//kill the event queues
}

@end
