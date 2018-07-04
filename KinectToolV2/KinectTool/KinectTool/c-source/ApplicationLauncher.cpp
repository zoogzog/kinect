#include "ofMain.h"

#include "ApplicationMain.h"

int main() 
{
	ofSetupOpenGL(512, 192 * 2, OF_WINDOW);

	ofRunApp(new ApplicationMain());

}
