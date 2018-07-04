#include <string>
#include <iostream>

#include "Debugger.h"

using namespace std;

bool Debugger::DEBUG_MODE = true;

void Debugger::log(string message)
{
	if (DEBUG_MODE)
	{
		cout << message << endl;
	}
}