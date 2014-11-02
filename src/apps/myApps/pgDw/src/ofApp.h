#pragma once

#include <map>
#include <string>
#include <sstream>
#include <boost/any.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "ofMain.h"
#include "ofUtils.h"
#include "ofVbo.h"

#include "ofxUI.h"
#include "ofxOsc.h"

#define PORT 7778                       // OSCメッセージの受信に使うポート番号（送信側と揃える）
#define NUM_BILLBOARDS 40000


class Parameter {

public:
    int intVal;
	float floatVal;
    float max, min, init;
    string name;   
    
};


class AppParameter:public Parameter {

public:
    int x, y, w, h;
    int fontSize;
    string dir, uiType;
    const type_info* type;

	AppParameter(string _name, const type_info* _type, string _uiType, int _min=0, int _max=100, int _init=0, int _w=100, int _h=40, string _dir="bottom") {
        
        name = _name;
        type = _type;
        uiType = _uiType;
        min = _min;
        max = _max;
        init = _init;
        w = _w;
        h = _h;
        dir = _dir;
        
        floatVal = 0;
        intVal = 0;
        
        /*
        floatValue = (float)_value;
        intValue = (int)_value;
		*/
    }

};


class testApp : public ofBaseApp{

public:
    int imgPixels;
    int screenShotCounter;
    int blendMode;
    int addR, addG, addB;
    int compH, compS, compB, compA;
    int pImgPattern;
    int prevClickButton;
    float size, pitch;
    float posX, posY, posZ;
    float rotateX, rotateY, rotateZ;
    float prevPosX, prevPosY, prevPosZ;
    float prevRotateX, prevRotateY, prevRotateZ;
    float bgH, bgS, bgB;
    float posRandomize;
    float posRandomSeed;
    float billboardSizeTarget[NUM_BILLBOARDS];	// billboard particles
    bool showBgImg;
    bool rightClick;
    string bgImgPath;
    string prevScreenShotDateTime;

    ofPoint prevClickPoint;
    ofImage img, particleImg, tileImg, bgImg;
    ofImage texture;
    map<string, boost::any> oscPrm;
    map<string, ofSoundPlayer> sndMap;
    map<string, AppParameter *> prmMap;
    vector<AppParameter> prmLst;
    vector <ofColor> imgColorLst;
	// map<string, boost::any> prmValLst;

    ofCamera cam;
    ofFbo screenFbo;
    ofFbo maskFbo;
    ofVboMesh billboards;						//ofVbo billboards;
    ofVec3f billboardVels[NUM_BILLBOARDS];
    ofShader billboardShader;

	ofxOscReceiver oscReceiver;       //OSCメッセージを受信するインスタンス
    ofxUICanvas *gui;

	// ---------------------------------------------

    void setup();
    void update();
    void draw();
	
    void exit();
    void guiEvent(ofxUIEventArgs &e);
    void receiveOscMessage();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

	void _imgLoad();
    void _drawTransparentTile();
    void _saveScreenShot();

};

