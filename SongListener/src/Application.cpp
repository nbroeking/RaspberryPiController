#include "ScopedLock.h"
#include "Event.h"
#include "Log.h"
#include "Application.h"
#include <fftw3.h>
#include <cstring>
#include <math.h>
using namespace std;

//Main Running Loop
int MainApplication::run()
{
	//No two threads can try and run the application at a time. It shouldn't
	// happen anyway but you never know what mistakes i will make.
	if( !runMutex.try_lock() || isRunning == true)
	{
		return 1;
	}
	m.lock();
	isRunning = true;
	//Initilization
	shouldRun.store(true);

    if (!(s = pa_simple_new(NULL, "Song Listener", PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        return 2;
    }
    m.unlock();
	while (shouldRun)
	{
	    uint8_t buf[BUFSIZE];
        
        /* Record some data ... */
        if (pa_simple_read(s, buf, sizeof(buf), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            return 3;
        }
        
        //THis is a bad check so we only do the fft on real data
        if( buf[0] != 0){

            memcpy(in, buf, BUFSIZE);

            for( int i =0; i < BUFSIZE; i++){
                in[i] = buf[i];
                in[i] *= 0.54f - 0.46f * cosf( (3.1415926 * 2.f * i) / (BUFSIZE - 1) ); 
            }

            float start = 0.0;

            fftw_execute(p);
              // I rewrite to out[i][0] squared absolute value of a complex number out[i].
            for (uint i = 0; i < BUFSIZE; ++i)
            {
                out[i][0] = out[i][0]*out[i][0] + out[i][1]*out[i][1];
            }
            
            for( int i = 0; i < BUFSIZE; i++){
                cerr << start << ", " <<  out[i][0] << endl;
                start += 10.7666;
            }
            exit(0);          
        }
        printf("Completed sample %d\n", buf[0]);   
        //Do a FFT 
    }
	m.lock();
	isRunning = false;
	m.unlock();

	runMutex.unlock();

    printf("Application Exiting Normally\n");
	return 0;
}

//Constructor
MainApplication::MainApplication():
m(),
q(),
runMutex()
{
	isRunning = false;
	shouldRun.store(true);

    s = NULL;
    
    p = fftw_plan_dft_r2c_1d(BUFSIZE, in, out, FFTW_ESTIMATE); 
}


MainApplication::~MainApplication()
{
    if( s != NULL){
        pa_simple_free(s);
    }
    else{
        printf("Application could not clean up pulse objects\n");
    }
    fftw_destroy_plan(p);
}
void MainApplication::pleaseDie()
{
//	SystemLog("Please Die in Main Application");
	//Add a close event to the application
    shouldRun.store(false);
}
