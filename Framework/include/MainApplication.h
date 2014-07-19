/* This is the main application. 
Pattern: Singleton
File: MainApplicaion.mm
Written By: Nicolas Broeking */

@interface MainApplication
{
	static MainApplication* app;
}

+(MainApplication*) getApp;
+(void)cleanUp;
@end
