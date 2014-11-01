#include "ofApp.h"


void testApp::setup(){
    
    bgImgPath = "";
    showBgImg = false;
    rightClick = false;
    pImgPattern = 0;
    prevClickPoint = ofPoint(0,0);

    ofSetBackgroundAuto(false);
    ofDisableArbTex();

    oscReceiver.setup( PORT );          // OSC受信用ポート設定

	//cam.setNearClip(0.f);
	//cam.setFarClip(-1000.f);
	//cam.enableOrtho();
    cam.setupPerspective();
    //cam.setNearClip(10);
    //cam.setFarClip(1000);
    //cam.
    //cam.setFov(10.0);
	//cam.setPosition(0, 0, 0);
    cout << "cam:" << endl;
    cout << cam.getNearClip() << endl;
    cout << cam.getFarClip() << endl;
    cout << cam.getFov() << endl;
    cout << cam.getAspectRatio() << endl;
    cout << cam.getForceAspectRatio() << endl;
    //cam.setNearClip(0.8505);
    //cam.setFarClip(1578.5);
    
    
    screenFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	maskFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

    // GUI関係のセットアップ
    gui = new ofxUICanvas(0,0,600,400);
    //gui->setFont("GUI/NewMedia Fett.ttf");
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 7);
    gui->setColorBack(ofxUIColor(0,0,0,40));

    int w = 120;
    int h = 12;
    gui->addWidgetRight(new ofxUIButton(32, 24, false, "LOAD_PIXEL_ART"));
    gui->addWidgetRight(new ofxUIButton(32, 24, false, "SAVE_SCREENSHOT"));
    //gui->addWidgetRight(new ofxUIButton(32, 24, false, "BG FILE OPEN"));
    //gui->addWidgetRight(new ofxUIToggle(32, 24, false, "FULLSCREEN"));
    gui->addSpacer(600, 15);
    
    prmLst.push_back( AppParameter("SIZE", &typeid(float), "slider", 0, 50, 5, w, h, "bottom") );
    prmLst.push_back( AppParameter("DOT_SIZE", &typeid(float), "slider", 0, 5, 1, w, h, "right") );
    prmLst.push_back( AppParameter("POS_X", &typeid(float), "slider", -500, 500, 0, w, h, "bottom") );
    prmLst.push_back( AppParameter("POS_Y", &typeid(float), "slider", -500, 500, 0, w, h, "right") );
    prmLst.push_back( AppParameter("POS_Z", &typeid(float), "slider", -500, 0, 0, w, h, "right") );
    prmLst.push_back( AppParameter("ROTATE_X", &typeid(float), "slider", -360, 360, 0, w, h, "bottom") );
    prmLst.push_back( AppParameter("ROTATE_Y", &typeid(float), "slider", -360, 360, 0, w, h, "right") );
    prmLst.push_back( AppParameter("ROTATE_Z", &typeid(float), "slider", -360, 360, 0, w, h, "right") );
    prmLst.push_back( AppParameter("ADD_R", &typeid(float), "slider", -255, 255, 0, w, h, "bottom") );
    prmLst.push_back( AppParameter("ADD_G", &typeid(float), "slider", -255, 255, 0, w, h, "right") );
    prmLst.push_back( AppParameter("ADD_B", &typeid(float), "slider", -255, 255, 0, w, h, "right") );
    prmLst.push_back( AppParameter("ADD_A", &typeid(float), "slider", -255, 255, 0, w, h, "right") );
    prmLst.push_back( AppParameter("COLOR_H", &typeid(float), "slider", -0, 255, 0, w, h, "bottom") );
    prmLst.push_back( AppParameter("COLOR_S", &typeid(float), "slider", -255, 255, 0, w, h, "right") );
    prmLst.push_back( AppParameter("COLOR_B", &typeid(float), "slider", -255, 255, 0, w, h, "right") );
    prmLst.push_back( AppParameter("COLOR_A", &typeid(float), "slider", -255, 255, 0, w, h, "right") );
    prmLst.push_back( AppParameter("BG_H", &typeid(float), "slider", 0, 255, 0, w, h, "bottom") );
    prmLst.push_back( AppParameter("BG_S", &typeid(float), "slider", 0, 255, 0, w, h, "right") );
    prmLst.push_back( AppParameter("BG_B", &typeid(float), "slider", 0, 255, 128, w, h, "right") );
    prmLst.push_back( AppParameter("BG_A", &typeid(float), "slider", 0, 255, 255, w, h, "right") );
    prmLst.push_back( AppParameter("POS_RANDOMIZE", &typeid(float), "slider", 0, 5, 0, w, h, "bottom") );
    prmLst.push_back( AppParameter("RANDOMSEED", &typeid(int), "slider", 0, 100, 0, w, h, "bottom") );
    prmLst.push_back( AppParameter("PARTICLE_IMAGE", &typeid(int), "dropdownList", 0, 100, 0, w, h, "bottom") );
    
    // 素材の読み込み
	img.loadImage("pixelArt/default.png");
	//img.loadImage("face4_48x48.png");
	//img.loadImage("pixelArt/magica_madoka_henshin.png");
	//img.loadImage("dot.png");
	particleImg.loadImage("particleImg/default.tif");
	tileImg.loadImage("tileImg/default.png");
    
    sndMap["se_screen_shot"].loadSound("se_screen_shot.wav");
    
    for(int i=0; i<prmLst.size(); i++) {
        AppParameter* prm = &prmLst[i];
        ofxUIWidget* widget;
        
        bool addFlag = false;
        
        ofxUISlider* s;
        if (prm->uiType == "slider") {
            s = new ofxUISlider(prm->w, prm->h, prm->min, prm->max, prm->init, prm->name);
            s->setPadding(70);
            s->setDrawPadding(true);
            widget = (ofxUIWidget *)s;

            //widget = (ofxUIWidget*)w;
            addFlag = true;
        }

        if (addFlag) {
            if (prm->dir == "right") {
                gui->addWidgetRight(widget);
            } else {
                gui->addWidgetDown(widget);
            }
            if (*prm->type == typeid(int) ){
                s->setLabelPrecision(0);        // addWidgetのあとに呼ばないとエラー
            }

        }

        //gui->addWidgetDown(new ofxUILabel("OFXUI TUTORIAL", OFX_UI_FONT_LARGE));
    }

    gui->addSpacer(600, 15);

    {
        vector<string> items;
        
        ofDirectory ofDir;
        ofDir.listDir("./pixelArt/");
        
        //cout << "dir: ";
        
        vector<ofFile> files = ofDir.getFiles();
        vector<ofFile>::iterator itr = files.begin();  // イテレータのインスタンス化
        while( itr != files.end() )  // 末尾要素まで
        {
            if (!itr->isDirectory()) {
                //cout << itr->getFileName() << endl;  // *演算子で間接参照
                
                items.push_back(itr->getFileName());
            }
            ++itr;                 // イテレータを１つ進める
        }
        
        //items.push_back("FIRST ITEM");
        
        gui->addWidgetDown(new ofxUIDropDownList("PIXEL_ART", items, 150));
        
    }
    
    //gui->addSpacer();
    
    {
        vector<string> items;
        
        ofDirectory ofDir;
        ofDir.listDir("./particleImg/");
        
        vector<ofFile> files = ofDir.getFiles();
        vector<ofFile>::iterator itr = files.begin();  // イテレータのインスタンス化
        while( itr != files.end() )  // 末尾要素まで
        {
            if (!itr->isDirectory()) {
                //cout << itr->getFileName() << endl;  // *演算子で間接参照
                
                items.push_back(itr->getFileName());
            }
            ++itr;                 // イテレータを１つ進める
        }
    
        gui->addWidgetRight(new ofxUIDropDownList("PARTICLE_IMAGE", items,150));
    }
    
    //gui->addSpacer();

    /*
    {
        vector<string> items;
        
        ofDirectory ofDir;
        ofDir.listDir("./tileImg/");
        
        vector<ofFile> files = ofDir.getFiles();
        
        vector<ofFile>::iterator itr = files.begin();  // イテレータのインスタンス化
        while( itr != files.end() )  // 末尾要素まで
        {
            if (!itr->isDirectory()) {
                //cout << itr->getFileName() << endl;  // *演算子で間接参照
                
                items.push_back(itr->getFileName());
            }
            ++itr;                 // イテレータを１つ進める
        }
        
        gui->addWidgetRight(new ofxUIDropDownList("TILE IMAGE", items, 150));
    }
     */
    {
        vector<string> items;
        items.push_back("RANDOM");
        items.push_back("BRIGHTNESS/2");
        
        gui->addWidgetRight(new ofxUIDropDownList("P_IMG_PATTERN", items, 130));
    }
    
    {
        vector<string> items;
        items.push_back("NORMAL");
        items.push_back("ADD");
        items.push_back("MULTIPLY");
        items.push_back("SUB");
        items.push_back("SCREEN");
        items.push_back("ALPHA");
        
        gui->addWidgetRight(new ofxUIDropDownList("BLEND_MODE", items, 120));
    }
    
    _imgLoad();
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    for(int i=0; i<prmLst.size(); i++) {
        AppParameter* prm = &prmLst.at(i);
        prmMap[prm->name] = prm;
    }
    
    for(int i=0; i<prmLst.size(); i++) {
        AppParameter* prm = &prmLst[i];
        ofxUIWidget* widget;
        
        // 値の初期設定
        if (prm->uiType == "slider") {
            ofxUISlider* s;
            
            s = (ofxUISlider *)gui->getWidget(prm->name);
            
            s->setValue(prm->init);
            s->update();
            s->triggerSelf();
        }
        
    }
    
    //gui->loadSettings("GUI/guiSettings.xml");               // 最後に実行しないと実行時エラー

    // 今は隠しておきたいパラメータを下に列記
    gui->getWidget( prmMap["BG_A"]->name)->setVisible(false);
    //gui->getWidget( prmMap["COLOR_A"]->name)->setVisible(false);
}

//--------------------------------------------------------------
void testApp::update(){
    //cam.setFov(prmMap["CAM_FOV"]->floatVal);
    //cam.setNearClip(prmMap["CAM_NEAR"]->floatVal);
    //cam.setFarClip(prmMap["CAM_FAR"]->floatVal);

    rightClick = false;

    receiveOscMessage();

	stringstream ss;
	ss << "pixelFx ver.0.1 - fps:" << ofGetFrameRate();
	ofSetWindowTitle(ss.str());
    
    ofSeedRandom(prmMap["RANDOMSEED"]->intVal);
    
    // 高速化するために変数に展開（mapがなぜかめちゃくちゃ遅い）
    int imgWidth = img.getWidth();
    int imgHeight = img.getHeight();
    float dotSize = prmMap["DOT_SIZE"]->floatVal;
    float posRandomize = prmMap["POS_RANDOMIZE"]->floatVal;
    float colorH = prmMap["COLOR_H"]->floatVal;
    float colorS = prmMap["COLOR_S"]->floatVal;
    float colorB = prmMap["COLOR_B"]->floatVal;
    float colorA = prmMap["COLOR_A"]->floatVal;
    float addR = prmMap["ADD_R"]->floatVal;
    float addG = prmMap["ADD_G"]->floatVal;
    float addB = prmMap["ADD_B"]->floatVal;
    float addA = prmMap["ADD_A"]->floatVal;
    int particleImgNum = particleImg.getWidth() / particleImg.getHeight();
    int pSize = particleImg.getHeight();
    int pWidth = particleImg.getWidth();
    
    imgPixels =(int)img.getWidth() * (int)img.getHeight();
    
    for (int i=0; i<imgPixels; i++) {
        
        int p = i * 6;
        int x = i % imgWidth;
        int y = i / imgWidth;
		
        float rx = (ofRandomuf() - 0.5) * posRandomize;
        float ry = (ofRandomuf() - 0.5) * posRandomize;

        float west = (x - imgWidth/2.0 - dotSize/2.0+0.5)  + rx;
        float east = (x - imgWidth/2.0 + dotSize/2.0+0.5)  + rx;
        float north = (y - imgHeight/2.0 - dotSize/2.0+0.5)  + ry ;
        float south = (y - imgHeight/2.0 + dotSize/2.0+0.5 )  + ry;

        ///*
        ofColor c = imgColorLst[x+ y*imgWidth];
        
        if ((c.r ==0 && c.g ==0 && c.b ==0) || c.a == 0) {
            c = ofColor(0,0,0,0);
            //ofSetColor(c);
        } else {
            int r = c.r + addR;
            int g = c.g + addG;
            int b = c.b + addB;
            int a = c.a + addA + colorA;
            r = ofClamp(r, 0, 255);
            g = ofClamp(g, 0, 255);
            b = ofClamp(b, 0, 255);
            a = ofClamp(a, 0, 255);

            c = ofColor(r, g, b);
            
            float hsbH, hsbS, hsbB, hsbA;
            c.getHsb(hsbH, hsbS, hsbB);
            
            hsbH += colorH;
            hsbS += colorS;
            hsbB += colorB;
            c = ofColor::fromHsb(hsbH, hsbS, hsbB);
            c.a = a;
        
            if (pImgPattern == 1) {
                float b2 = (((int)c.getBrightness())/3)*3;
                if (b2 > 255) {
                    b2 = 255;
                }
                c = ofColor::fromHsb(hsbH, hsbS, b2);
            }
        }
        
        //billboards.setColor(p, c);
        //billboards.setColor(p+1, c);
        //billboards.setColor(p+2, c);
        //billboards.setColor(p+3, c);
        //billboards.setColor(p+4, c);
        //billboards.setColor(p+5, c);
        billboards.setColorForIndices(p,p+6,c);
        //*/
        
        billboards.setVertex(p, ofVec3f(west, south, 0));
        billboards.setVertex(p+1, ofVec3f(west, north, 0));
        billboards.setVertex(p+2, ofVec3f(east, south, 0));
        billboards.setVertex(p+3, ofVec3f(east, south, 0));
        billboards.setVertex(p+4, ofVec3f(west, north, 0));
        billboards.setVertex(p+5, ofVec3f(east, north, 0));
        
        /*
        billboards.setNormal(p, ofVec3f(12,0,0));
    	billboards.setNormal(p+1, ofVec3f(12,0,0));
    	billboards.setNormal(p+2, ofVec3f(12,0,0));
    	billboards.setNormal(p+3, ofVec3f(12,0,0));
    	billboards.setNormal(p+4, ofVec3f(12,0,0));
    	billboards.setNormal(p+5, ofVec3f(12,0,0));
         */

        
        if (pImgPattern == 0) {
        
            float srcX = ((int)ofRandom(particleImgNum)) / (float)particleImgNum;
            west = srcX;
            east = west + (1.0/particleImgNum);
            north = 0;
            south = 1;
        } else {
            //int penSlashNum = ((int)c.b / (255 / particleImgNum));
            /*
            if (penSlashNum > (pNum/2-1)) {
                penSlashNum = pNum/2-1;
            }
             */
            int pNum = pWidth / pSize / 2;
            west = (  ((int) ( (c.getBrightness() / 256.0) * pNum))*2  + (int)ofRandom(2))  / ((float)pNum*2) ;
            
            east = west + (1.0/particleImgNum);
            north = 0;
            south = 1;
        }
        billboards.setTexCoord(p, ofVec2f(west, south));
        billboards.setTexCoord(p+1, ofVec2f(west, north));
        billboards.setTexCoord(p+2, ofVec2f(east, south));
        billboards.setTexCoord(p+3, ofVec2f(east, south));
        billboards.setTexCoord(p+4, ofVec2f(west, north));
        billboards.setTexCoord(p+5, ofVec2f(east, north));
	}

    ofSetColor(255);
}


void testApp::_imgLoad(){

    imgPixels = img.getWidth() * img.getHeight();
    
    imgColorLst.clear();
    for(int i=0; i<imgPixels; i++) {
        imgColorLst.push_back( img.getColor( i%(int)img.getWidth(), i/(int)img.getWidth() ) );
    }
    
    int particleImgNum = particleImg.getWidth() / particleImg.getHeight();
    
    billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_TRIANGLES);
    
    //billboards.clear();
    billboards.clearVertices();
    billboards.clearColors();
    billboards.clearNormals();
    billboards.clearTexCoords();
    billboards.clearIndices();
    
	// ------------------------- billboard particles
    billboards.usingTextures();
	for (int i=0; i<imgPixels; i++) {
        int x = i % (int)img.getWidth();
        int y = i / (int)img.getWidth();        
        
        int rate = 1;
        float west = x*rate;
        float east = (x+1)*rate;
        float north = y*rate;
        float south = (y+1)*rate;

        ofColor c = img.getColor(x, y);
        billboards.addColor(c);
        billboards.addColor(c);
        billboards.addColor(c);
        billboards.addColor(c);
        billboards.addColor(c);
        billboards.addColor(c);
        
        billboards.addIndex(i*6);
        billboards.addIndex(i*6+1);
        billboards.addIndex(i*6+2);
        billboards.addIndex(i*6+3);
        billboards.addIndex(i*6+4);
        billboards.addIndex(i*6+5);

		//billboards.getVertices()[i].set(x * 30+ofRandom(10), y * 30+ofRandom(10), 0);
        //billboards.getTexCoords()[i].set(i*1+ofRandom(10),i*3+ofRandom(10) );
        ofPushMatrix();
        {
            //ofTranslate(x, y);
            billboards.addVertex(ofVec3f(west, south, 0));
            billboards.addVertex(ofVec3f(west, north, 0));
            billboards.addVertex(ofVec3f(east, south, 0));
            
            billboards.addVertex(ofVec3f(east, south, 0));
            billboards.addVertex(ofVec3f(west, north, 0));
            billboards.addVertex(ofVec3f(east, north, 0));
            
            float srcX = ((int)ofRandom(particleImgNum)) / (float)particleImgNum;
            west = srcX;
            east = west + (1.0/particleImgNum);
            north = 0;
            south = 1;
            //cout << particleImgNum;

            billboards.addTexCoord(ofVec2f(west, south));
            billboards.addTexCoord(ofVec2f(west, north));
            billboards.addTexCoord(ofVec2f(east, south));
            billboards.addTexCoord(ofVec2f(east, south));
            billboards.addTexCoord(ofVec2f(west, north));
            billboards.addTexCoord(ofVec2f(east, north));
        }
        ofPopMatrix();
		
        //billboards.getColors()[i].set(c);
        
    	billboards.addNormal(ofVec3f(12,0,0));
    	billboards.addNormal(ofVec3f(12,0,0));
    	billboards.addNormal(ofVec3f(12,0,0));
    	billboards.addNormal(ofVec3f(12,0,0));
    	billboards.addNormal(ofVec3f(12,0,0));
    	billboards.addNormal(ofVec3f(12,0,0));

		//billboards.getColors()[i].set(ofColor::fromHsb(ofRandom(96, 160), 255, 255));
	    //billboardSizeTarget[i] = ofRandom(4, 64);
        
		//cout << x << endl;
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofSetColor(255);
    
    //screenFbo.begin();
    cam.begin();
    
	//ofEnableAlphaBlending();
	//ofEnableAntiAliasing();
    //ofEnableSmoothing();
    //glEnable(GL_MULTISAMPLE);
    //glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    
    //ofSetupScreenOrtho();

    //ofClear(0,0,0,255);
	ofClear(ofColor::fromHsb(prmMap["BG_H"]->floatVal, prmMap["BG_S"]->floatVal, prmMap["BG_B"]->floatVal, prmMap["BG_A"]->floatVal));
    
    if (showBgImg) {
        float imgAspect = img.getWidth() / img.getHeight();
        bgImg.draw(0,0,ofGetWidth(), ofGetWidth() / imgAspect);
    }

    if (blendMode == 0) {
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    } else if (blendMode == 1) {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
    } else if (blendMode == 2) {
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    } else if (blendMode == 3) {
        ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    } else if (blendMode == 4) {
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    } else if (blendMode == 5) {
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    }

    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); ニアレストネイバー拡大
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    /*
	for(int i=0; i<(ofGetHeight()/tileImg.getHeight()); i++) {
		for(int j=0; j<(ofGetWidth()/tileImg.getWidth()); j++) {
			tileImg.draw(j*tileImg.getWidth(), i*tileImg.getHeight(), tileImg.getWidth(), tileImg.getHeight());
		}
	}
     */
    
    ofPushMatrix();
    
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 500);
    
 //   ofScale(prmMap["SIZE"]->floatVal,　prmMap["SIZE"]->floatVal);
    ofTranslate(prmMap["POS_X"]->floatVal, prmMap["POS_Y"]->floatVal, prmMap["POS_Z"]->floatVal);

    ofScale(prmMap["SIZE"]->floatVal, prmMap["SIZE"]->floatVal);

    ofRotateX(prmMap["ROTATE_X"]->floatVal);
    ofRotateY(prmMap["ROTATE_Y"]->floatVal);
    ofRotateZ(prmMap["ROTATE_Z"]->floatVal);

	particleImg.getTextureReference().bind();
	//billboards.draw(OF_MESH_WIREFRAME);
	billboards.draw();
	particleImg.getTextureReference().unbind();
    
    ofPopMatrix();
    
    cam.end();
    //screenFbo.end();

    //ofEnableAlphaBlending();
	//ofEnableAntiAliasing();

    //ofBackground(0);
    
    //_drawTransparentTile();

    //screenFbo.draw(0, 0);

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    
    return;
    
    
    /*

	maskFbo.begin();
	//ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    
    
	//ofEnableAlphaBlending();
	//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
	//ofBackground(192,128,160);
    //int bgColor = 224;
    //ofBackground(bgColor);
    
    ofPushMatrix();
    
    //ofTranslate(-img.getWidth()/2*size,-img.getHeight()/2*size);
	//		ofRotate(180,rotateX,rotateY,rotateZ);
    ofRotateX(rotateX);
    ofRotateY(rotateY);
    ofRotateZ(rotateZ);
	for(int i=0; i<img.getHeight(); i++) {
		for(int j=0; j<img.getWidth(); j++) {
            
			ofColor c = img.getColor(j, i);
            float h, s, b;
            c.getHsb(h, s, b);
            
			if ((c.r ==0 && c.g ==0 && c.b ==0) || c.a == 0) {
				c = ofColor(0,0,0,0);
				ofSetColor(c);
			} else {
                c.r += addR;
                c.g += addG;
                c.b += addB;
                if (c.r > 255) {
                    c.r = 255;
                }
				//ofSetColor(c);
                
                float b2 = (((int)b)/3)*3;
                if (b2 > 255) {
                    b2 = 255;
                }
                
                ofSetColor(ofColor::fromHsb(h, s, b2));
			}
            
            //cout << " " << c.r << " " << c.g << " " << c.b << endl;

			//ofRect(ofPoint(j*pitch, i*pitch), size, size);

			ofPushMatrix();
			ofTranslate(j*pitch+posX + ofRandomuf()*posRandomize, i*pitch+posY + ofRandomuf()*posRandomize, ofRandomuf()*posRandomize);
			ofScale(size/256,size/256);
            
            
            
			//ofRotate(45,0,0,1);
            int pSize = particleImg.getHeight();
            int pWidth = particleImg.getWidth();
			//particleImg.draw(-particleImg.getWidth()/2,-particleImg.getHeight()/2, pSize, pSize);
			//particleImg.draw(-particleImg.getWidth()/2,-particleImg.getHeight()/2);
            
            //particleImg.drawSubsection(0, 0, pSize, pSize, pSize*(int)ofRandom(pWidth/pSize), 0, pSize, pSize);
                int tNum = (int)pWidth/(int)pSize;
            
                //int penSlashNum = abs(((int)bgColor-(int)b))/tNum;
                int penSlashNum = tNum/2 - ((int)b / tNum) + 4;
            if (penSlashNum > (tNum/2-1)) {
                penSlashNum = tNum/2-1;
            }
            
            ofRect(-particleImg.getWidth()/2,-particleImg.getHeight()/2, pSize, pSize);
                //particleImg.drawSubsection(0, 0, pSize, pSize, (penSlashNum * 2 + (int)ofRandom(2)) * pSize, 0, pSize, pSize);
            
            //cout << pSize;
			ofPopMatrix();
		}
	}
    ofPopMatrix();
	
	maskFbo.end();

	ofSetColor( ofColor::fromHsb(compH, compS, compB) );

	
	ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	maskFbo.draw(0,0);

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    
    //soundInput.drawMicLevelMeter(800, 200);        // 動作チェック用。（なくても動作します） tanaka add
    
    {
        stringstream ss;
        ss << "ofScr_" << ofGetFrameNum() << ".png";
        //ofSaveScreen(ss.str());
        //ofSaveFrame();
    }
	
	//particleImg.draw(0, 0);
     
     */

}

// 半透明を表す市松模様を描く
void testApp::_drawTransparentTile() {

    int size = 25;
    for(int i=0; i<=ofGetHeight()/size; i++) {
        for(int j=0; j<=ofGetWidth()/size; j++) {
            if ((i+j)%2 == 0) {
                ofSetColor(255,255,255,255);
            } else {
                ofSetColor(255,255,255,192);
            }
            ofRect(j*size, i*size, size, size);
        }
    }

}

void testApp::receiveOscMessage() {
    
    //現在順番待ちのOSCメッセージがあるか確認
    while( oscReceiver.hasWaitingMessages() ) {
        
        ofxOscMessage m;
        oscReceiver.getNextMessage( &m );   //次のメッセージを取得

        string appId = "/pixelFx/";
        string objId = "main/";
        
        string header = appId + objId;
        
        for(int i=0; i<prmLst.size(); i++) {
            if ( m.getAddress() == header+prmLst[i].name ){
                
                cout << "OSC mes: " << m.getAddress() << endl;
                
                if (prmLst[i].uiType == "slider") {
                
                    ofxUISlider *w = (ofxUISlider *)gui->getWidget(prmLst[i].name);

                    float n = 0;
                    if (w->getMin()>=0) {
                    
                        if (m.getArgAsFloat(0) >= 0) {
                            n =(m.getArgAsFloat(0) * (w->getMax() - w->getMin())) + w->getMin();
                        } else {
                            n = 0;
                        }
                        
                    } else {
                        if (m.getArgAsFloat(0) >= 0) {
                            n = abs(m.getArgAsFloat(0) * w->getMax());
                        } else {
                            n = -1 * abs(m.getArgAsFloat(0) * w->getMin());
                        }
                    }
                    oscPrm[prmLst[i].name] = n;

                    w->setValue(n);
                    //w->update();
                    w->triggerSelf();       // 変数に反映
                    //cout << "OSC / " << prmLst[i] << ":" << n << endl;
                } else if (prmLst[i].uiType == "dropdownList") {
                    
                    ofxUIDropDownList *w = (ofxUIDropDownList *)gui->getWidget(prmLst[i].name);
                    int n = m.getArgAsInt32(0);    // ドロップダウンリストの何番目かを指定
                    
                    oscPrm[prmLst[i].name] = n;
                    //w->clearToggles();
                    //w->activateToggle("char.tif");
                    //w->setState(n);
                    //w->stateChange();
                    //w->update();
                    vector<ofxUILabelToggle *> toggles = w->getToggles();
                    for(int i=0; i<toggles.size(); i++) {
                        cout << "t:" << toggles[i]->getName() << endl;
                    }
                    if (n < 0) {
                        n = 0;
                    }
                    if (n >= toggles.size()) {
                        n = toggles.size() - 1;
                    }
                    w->triggerEvent(toggles[n]);
                    //w->triggerSelf();
                    //w->setValue(n);                    
                    
                }
                
            }
        }
        
        
        /*
        //stringstream address;
        //address << appId << objId << ;

        //cout << header + "af";

        if ( m.getAddress() == header+"POS_X" ){
            
            float vol = m.getArgAsFloat( 0 );       // 値を取得
        }
         */
        
        //OSCメッセージをそのままコンソールに出力
        //dumpOsc(m);   // 本番では重いのでコメントアウト
    }
    
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == ' ') {
        
        if(gui->isEnabled()) {
            gui->disable();
        } else {
            gui->enable();
        }
        
    } else if (key == 's') {
        _saveScreenShot();
    }
}

void testApp::_saveScreenShot() {
    
    //ofSaveFrame();
    stringstream ss;
    ss << "record/";
    ss << ofGetYear();
    ss << boost::format( "%02d" ) % ofGetMonth();
    ss << boost::format( "%02d" ) % ofGetDay();
    ss << "_";
    ss << boost::format( "%02d" ) % ofGetHours();
    ss << boost::format( "%02d" ) % ofGetMinutes();
    ss << boost::format( "%02d" ) % ofGetSeconds();
    
    // 撮影日時が同じ場合はファイル名に連番を付ける
    string dateTime = ss.str();
    if (dateTime == prevScreenShotDateTime) {
        screenShotCounter++;
    } else {
        screenShotCounter = 0;
    }
    ss << "_" << (screenShotCounter+1);
    ss << ".png";
    
        ofSaveScreen(ss.str());
    /*
    ofPixels ofp;
    screenFbo.readToPixels(ofp);
    ofSaveImage(ofp, ss.str());
     */
    
    sndMap["se_screen_shot"].play();
    
    prevScreenShotDateTime = dateTime;

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
    if (!(x < 600 && y < 400) || gui->isEnabled() == false) {
        if (button == 0) { // left click
            /*
            prmMap["ROTATE_X"]->floatVal = prevRotateX + (x - prevClickPoint.x);
            prmMap["ROTATE_Y"]->floatVal = prevRotateY + (y - prevClickPoint.y);
             */
            
            ofxUISlider* s = (ofxUISlider*)gui->getWidget("ROTATE_X");
            s->setValue(prevRotateX + (y - prevClickPoint.y));
            s->update();
            s->triggerSelf();
            
            s = (ofxUISlider*)gui->getWidget("ROTATE_Y");
            s->setValue(prevRotateY + (x - prevClickPoint.x));
            s->update();
            s->triggerSelf();
            
        } else if (button == 2) {
            /*
            prmMap["ROTATE_X"] = prevRotateX + (x - prevClickPoint.x);
            prmMap["ROTATE_Y"] = prevRotateY + (y - prevClickPoint.y);
             */
            
            ofxUISlider* s = (ofxUISlider*)gui->getWidget("POS_X");
            s->setValue(prevPosX + (x - prevClickPoint.x));
            s->update();
            s->triggerSelf();
            
            s = (ofxUISlider*)gui->getWidget("POS_Y");
            s->setValue(prevPosY + (y - prevClickPoint.y));
            s->update();
            s->triggerSelf();
        }
    }
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (button == 2) {
        rightClick = true;
    }
    
    if (!(x < 600 && y < 400) || gui->isEnabled() == false) {
        prevClickPoint = ofPoint(x, y);
        prevClickButton = button;
        if (button == 0) {
            prevRotateX = prmMap["ROTATE_X"]->floatVal;
            prevRotateY = prmMap["ROTATE_Y"]->floatVal;
            prevRotateZ = prmMap["ROTATE_Z"]->floatVal;
        } else if (button == 1) {
            prevPosX = prmMap["ROTATE_X"]->floatVal;
            prevPosY = prmMap["ROTATE_Y"]->floatVal;
            prevPosZ = prmMap["ROTATE_Z"]->floatVal;
       }
    }
    
    
    cout << "pressed";

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if (button == 2) {
        rightClick = true;
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::exit(){
    gui->saveSettings("GUI/guiSettings.xml"); 
    delete gui; 
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    
    if(e.widget->getName() == "BG FILE OPEN")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        
        //size = button->getScaledValue();
        if (!button->getValue()) {
            ofFileDialogResult t = ofSystemLoadDialog("Select background image file");
            //ofSystemTextBoxDialog(t.getPath());
            if (t.getPath() != "") {
                bgImgPath = t.getPath();
                bgImg.loadImage(bgImgPath);
                showBgImg = true;
            }
        }
        
    }
    else if (e.widget->getName() == "LOAD_PIXEL_ART")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        
        if (!button->getValue()) {
            ofFileDialogResult t = ofSystemLoadDialog("Select pixel art file");
            //ofSystemTextBoxDialog(t.getPath());
            if (t.getPath() != "") {
                string imgPath = t.getPath();
                img.loadImage(imgPath);
                _imgLoad();
            }
        }
        
    }
    else if (e.widget->getName() == "SAVE_SCREENSHOT")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        
        if (!button->getValue()) {
            _saveScreenShot();
        }        
    }
    else if(e.widget->getName() == "FULLSCREEN")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());   
    }
    else if(e.widget->getName() == "BLEND_MODE")
    {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        
        for(int i=0; i<selected.size(); i++) {
            stringstream ss;
            ss << selected[i]->getName();
            if ( ss.str() == "NORMAL") {
                blendMode = 0;
            } else if (ss.str() == "ADD") {
                blendMode = 1;
            } else if (ss.str() == "MULTIPLY") {
                blendMode = 2;
            } else if (ss.str() == "SUB") {
                blendMode = 3;
            } else if (ss.str() == "SCREEN") {
                blendMode = 4;
            } else if (ss.str() == "ALPHA") {
                blendMode = 5;
            }
            
        }
        cout << blendMode << endl;
        
    }
    else if(e.widget->getName() == "BLEND_MODE")
    {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        
        for(int i=0; i<selected.size(); i++) {
            stringstream ss;
            ss << selected[i]->getName();
            if ( ss.str() == "NORMAL") {
                blendMode = 0;
            } else if (ss.str() == "ADD") {
                blendMode = 1;
            } else if (ss.str() == "MULTIPLY") {
                blendMode = 2;
            } else if (ss.str() == "SUB") {
                blendMode = 3;
            } else if (ss.str() == "SCREEN") {
                blendMode = 4;
            } else if (ss.str() == "ALPHA") {
                blendMode = 5;
            }
            
        }
        cout << blendMode << endl;
        
    }
    
    else if(e.widget->getName() == "P_IMG_PATTERN") {
        
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        
        for(int i=0; i<selected.size(); i++) {
            stringstream ss;
            ss << selected[i]->getName();
            
            if ( ss.str() == "RANDOM") {
                pImgPattern = 0;
            } else if (ss.str() == "BRIGHTNESS/2") {
                pImgPattern = 1;
            }
            
        }
    }
    
    else if(e.widget->getName() == "PARTICLE_IMAGE") {
        
        cout<< "PART_IMAGE!"<<endl;
        
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        
        for(int i=0; i<selected.size(); i++) {
            stringstream ss;
            ss << "particleImg/" << selected[i]->getName();
            
            cout << "prtImage: " << ss.str() << endl;
            
            //ofImage tImg;
            //tImg.loadImage(ss.str());
            
            //particleImg.
            
            particleImg.loadImage(ss.str());
            
            _imgLoad();
            
            
            //ofColor centerColor = particleImg.getColor(particleImg.getWidth()/2, particleImg.getWidth()/2);
            
            
        }
    }
    else if(e.widget->getName() == "TILE_IMAGE") {
        
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        for(int i=0; i<selected.size(); i++) {
        
            stringstream ss;
            ss << "tileImg/" << selected[i]->getName();
            
            tileImg.loadImage(ss.str());
        }
    }
    else if(e.widget->getName() == "PIXEL_ART") {
        
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        
        for(int i=0; i<selected.size(); i++) {
            stringstream ss;
            ss << "pixelArt/" << selected[i]->getName();
            
            img.loadImage(ss.str());
            _imgLoad();
        }
        
    } else {
        if (e.widget->getKind() == OFX_UI_WIDGET_SLIDER_H) {
            ofxUISlider* slider = (ofxUISlider *) e.widget;
            //cout << slider->getName() << endl;
            //cout << typeid(*prmMap[name]).name() << endl;
            string name = (string)slider->getName();

            if (!rightClick) {
                
                if (*prmMap[name]->type == typeid(float)) {
                    prmMap[name]->floatVal = slider->getScaledValue();
                    
                    //cout << "float" << name << ":" << prmMap[name]->floatVal << endl;
                } else if (*prmMap[name]->type == typeid(int)) {
                    string label = slider->getLabel()->getLabel();      // PITCH: 0 のような文字列
                    
                    vector<string> strLst;
                    boost::algorithm::split( strLst, label, boost::algorithm::is_space() );
                    prmMap[name]->intVal = ofToInt( strLst[1] );
                    
                    //cout << "int " << name << ":" << prmMap[name]->intVal << endl;
                }
            }
            else {
                prmMap[name]->intVal = prmMap[name]->init;
                prmMap[name]->floatVal = prmMap[name]->init;
                slider->setValue(prmMap[name]->init);
            }
        }
    }
}


