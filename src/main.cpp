#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#define SCREEN_WIDTH 3240
#define SCREEN_HEIGHT 1920
//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, SCREEN_WIDTH,SCREEN_HEIGHT, OF_FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
