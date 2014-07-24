/* This is the main application. 
Pattern: Singleton
File: MainApplicaion.mm
Written By: Nicolas Broeking */

#ifndef __MainApplication__
#define __MainApplication__

class MainApplication
{
public:
	static MainApplication* app;
	static MainApplication* getApplication();
	static void cleanUp();

	virtual void applicationWillLaunch();
	virtual void applicationWillTerminate();
	virtual void pleaseClose();

protected:
	MainApplication();
	~MainApplication();

	bool isRunning;
	bool shouldRun;

};
#endif
