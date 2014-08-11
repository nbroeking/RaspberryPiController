#include "Log.h"

using namespace std;

void SystemLog(std::string log)
{
	cout << "Pi Log: " << log << endl;
}
void SystemError(std::string log)
{
	cerr << "Pi Log Error: " << log << endl;
}
