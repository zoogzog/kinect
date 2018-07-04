#pragma once

#include <string>

using namespace std;

class Debugger
{
private:
	static bool DEBUG_MODE;
	
public:
	static void log(string message);
};