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
//#include "ofxSoundInput.h"
#include "ofxOsc.h"
#define PORT 7778           // OSCメッセージの受信に使うポート番号（送信側と揃える）

#define NUM_BILLBOARDS 40000



class Parameter {
public:
    float max, min;
    float init;
    
    string name;
    
    float floatVal;
    int intVal;
    
};


class AppParameter:public Parameter {
public:
    int posX, posY, w, h, fontSize;
    const type_info* type;
    string dir;
    string uiType;
    
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
        intValue = (int)_value;*/
    }
    
        
};


class testApp : public ofBaseApp{
public:

    ofxOscReceiver oscReceiver;       //OSCメッセージを受信するインスタンス
    ofImage img;
    ofImage particleImg;
    ofImage tileImg;
    ofImage bgImg;
    map<string, boost::any> oscPrm;
    map<string, ofSoundPlayer> sndMap;

    vector<AppParameter> prmLst;
//    map<string, boost::any> prmValLst;
    map<string, AppParameter *> prmMap;

    ofCamera    cam;

    int screenShotCounter;
    string prevScreenShotDateTime;
    
    vector <ofColor> imgColorLst;
    

    float size, pitch;
    float rotateX, rotateY, rotateZ;
    float bgH, bgS, bgB;
    int blendMode;

    string bgImgPath;
    bool showBgImg;
    bool rightClick;

    int compH, compS, compB, compA;
    int addR, addG, addB;
    
    ofPoint prevClickPoint;
    int prevClickButton;
    float prevRotateX, prevRotateY, prevRotateZ;
    float prevPosX, prevPosY, prevPosZ;
    
    int pImgPattern;

    
    ofFbo screenFbo;

    ofFbo maskFbo;
    
    // billboard particles
    float billboardSizeTarget[NUM_BILLBOARDS];
    
    ofShader billboardShader;
    ofImage texture;
    
    //ofVbo billboards;
    ofVboMesh billboards;
    ofVec3f billboardVels[NUM_BILLBOARDS];

    int imgPixels;

    ofxUICanvas *gui;
    void exit(); //iOedC…ewiECμC?CμC?C§C±C?C…C≫CECAC?cIeuC∑CECcEREAEiEgEAEEEg 
    void guiEvent(ofxUIEventArgs &e); 

    float posX, posY, posZ;
    float posRandomize;
    float posRandomSeed;

    void setup();
    void update();
    void draw();

    void _imgLoad();
    void _drawTransparentTile();
    void _saveScreenShot();

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


};

