#pragma once

#include <map>
#include <string>
#include <sstream>
#include <iostream>
//#include <fstream>
#include <algorithm>
#include <cmath>
#include <filesystem> // std::tr2::sys::path etc.
#include <random>

#include <boost/any.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/array.hpp>

#include "ofMain.h"
#include "ofUtils.h"
#include "ofVbo.h"

#include "ofxUI.h"
#include "ofxOsc.h"

#include "zlib.h"
#include "zconf.h"
#include "png.h"

#include "ofxTrueTypeFontUC.h"

//#pragma comment (lib, "zlib.lib")
//#pragma comment (lib, "libpng16.lib")

#define PORT 7778                       // OSCメッセージの受信に使うポート番号（送信側と揃える）
#define NUM_BILLBOARDS 40000
#define CS 16                       // ChipSize
#define CENTER -1
#define UP 0
#define UP_RIGHT 1
#define RIGHT 2
#define DOWN_RIGHT 3
#define DOWN 4
#define DOWN_LEFT 5
#define LEFT 6
#define UP_LEFT 7
#define ACT_NONE 0
#define ACT_ATTACK 1


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



class Char {

public:
    map<string, ofImage*> imgMap;
    map<string, string> partsMap;
    map<string, vector<vector<unsigned char>> > imgMapPalette;
    map<string, vector<vector<unsigned char>> > indexImgMap;
    int x, y, z;
    int animCount;
    int dir;
    int count;
    int action;
    int actCount, actTime;
};



class ofApp : public ofBaseApp{

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
    float camRotX, camRotY, camRotZ;
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
    stringstream ss;
    vector<string> charPartsPathList;
    vector<string> charPartsDrawOrder;

    ofPoint prevClickPoint;
    ofImage img, particleImg, tileImg, bgImg, bgParticleImg;
    ofImage texture;
    ofImage imgHero;
    map<string, ofImage> imgCharPartsMap;
    map<string, vector<vector<unsigned char>> > indexImgMap;
    map<string, vector<vector<unsigned char>> > imgMapPalette;
    map<string, map<string, ofImage> > charPartsMap;
    map<string, int> key;
    map<string, int> keyOnce;

    map<string, boost::any> oscPrm;
    map<string, ofSoundPlayer> sndMap;
    map<string, AppParameter *> prmMap;
    vector<AppParameter> prmLst;
    vector <ofColor> imgColorLst;
    vector <Char> charList;
	// map<string, boost::any> prmValLst;

    ofCamera cam;
    ofEasyCam easyCam;
    ofFbo screenFbo;
    ofFbo maskFbo;
    ofVboMesh billboards;						//ofVbo billboards;
    ofVec3f billboardVels[NUM_BILLBOARDS];
    ofShader billboardShader;
    ofTrueTypeFont ttFont;
    ofxTrueTypeFontUC font;
    ofxTrueTypeFontUC boldFont;
    ofxTrueTypeFontUC pxFont;
    ofxTrueTypeFontUC prettyFont;

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



map<string, string> getDirectoryFileListRecursive(string targetDir);
vector<vector<unsigned char>> getPaletteFromPNG(string filePath);
vector<vector<unsigned char>> getPngIndexImage(string filePath);
void pngReadFunction(png_struct *png,png_bytep buf,png_size_t size);
void trace(stringstream *ss);
void trace(string s);
int weaponAttackDeg(int count);