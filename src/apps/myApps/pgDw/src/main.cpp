#include "ofApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	//ofAppGlutWindow window; // create a window
    
    //window.setGlutDisplayString("rgb alpha double samples>=4 depth"); // add for antialiasing
    //window.setGlutDisplayString("rgba double samples>=4");
    
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(1200, 700, OF_WINDOW);
	ofRunApp(new testApp()); // start the app
}
