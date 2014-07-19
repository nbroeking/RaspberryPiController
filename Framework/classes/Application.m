#import <stdio.h>
#import "Application.h"

@implementation Application

+(int)run: (int) argc : (char**)argv 
{
	fprintf(stderr, "Checking first cross compile application\n");
	return 0;
}
@end
