#include "ofApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
   
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	//ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);
    
    /* 2画面表示（デュアルモニタ）
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 3840,1080, OF_WINDOW);
    ofSetWindowPosition(0, 0);
    */

    ofRunApp(new ofApp());

}
