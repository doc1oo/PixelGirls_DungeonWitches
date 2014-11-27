#include "ofApp.h"

void ofApp::setup(){
    // 標準入出力をファイルに変更
    //ofstream ofs("debug.log");
    //cout.rdbuf(ofs.rdbuf());
    /*

    streambuf *old = cout.rdbuf(); // <-- save        
    stringstream ss;

    cout.rdbuf (ss.rdbuf());       // <-- redirect
    */

    // 変数の初期値設定 -----------------------------------------------

    bgImgPath = "";
    showBgImg = false;
    rightClick = false;
    pImgPattern = 0;
    size = 10;
    prevClickPoint = ofPoint(0,0);
    charPartsPathList.push_back("body");
    charPartsPathList.push_back("face");
    charPartsPathList.push_back("eye");
    charPartsPathList.push_back("hair");
    charPartsPathList.push_back("hairAcce");

    srand(time(NULL));
    charPartsDrawOrder.push_back("backAcce");
    charPartsDrawOrder.push_back("weapon");
    charPartsDrawOrder.push_back("body");
    charPartsDrawOrder.push_back("face");
    charPartsDrawOrder.push_back("eye");
    charPartsDrawOrder.push_back("hair");
    charPartsDrawOrder.push_back("hairAcce");

    // GUI関係の情報設定 -----------------------------------------------
    {
        int w = 120;
        int h = 12;
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
    }
    
    // 素材ファイルの読み込み ------------------------------------------------

	img.loadImage("pixelArt/default.png");
	particleImg.loadImage("particleImg/default.tif");
	tileImg.loadImage("tileImg/default.png");
    sndMap["se_screen_shot"].loadSound("se_screen_shot.wav");

    auto charPartsPathMap = getDirectoryFileListRecursive("./data/img/charParts/");

    for (auto path : charPartsPathMap) {

        cout << "vec-output: " << path.first << " " << path.second << endl;

        auto p = std::tr2::sys::path(path.second);
        string parentDirName = p.parent_path().filename();
        string fileName = p.stem();

        ofImage tImg;
        auto res = tImg.loadImage("../" + path.second);
        cout << "loadImage: [" << parentDirName << "][" << fileName << "] " << "../" << path.second;
            
        if (res) {
            cout << " - OK" << endl;
            charPartsMap[parentDirName][fileName] = tImg;
            cout << parentDirName << fileName << endl;
            indexImgMap[parentDirName + fileName] = getPngIndexImage(path.second);//getIndexImageFromPNG(path.second);

            imgMapPalette[parentDirName + fileName] = getPaletteFromPNG(path.second);

            cout << endl;

        } else {
            cout << " - NG";
        }

    }


    charList.clear();
    // random charMake
    for (int charCount = 0; charCount < 40; charCount++) {

        Char tChar = Char();

        auto itr = charPartsMap.begin();
        for(int i=0; i<charPartsMap.size(); i++) {
            string index;
            index = itr->first;

            map<string,  ofImage> tMap = itr->second;

            vector<string> nameList;
            
            for(auto t : tMap) {
                nameList.push_back(t.first);
            }
            string imgFileName = nameList[rand()%tMap.size()];
            //if (itr->first == "body") imgFileName = "blazer" ;
            //if (itr->first == "weapon") imgFileName = "nihontou" ;
            //if (itr->first == "backAcce") imgFileName = "none" ;
            //if (itr->first == "hairAcce") imgFileName = "null" ;

            tChar.partsMap[index] = imgFileName;
            tChar.imgMap[index] = &charPartsMap[index][imgFileName];
            tChar.indexImgMap[index] = indexImgMap[index+imgFileName];
            tChar.imgMapPalette[index] = imgMapPalette[index+imgFileName];

            tChar.x = (charCount%6+0.6)*200;//ofRandom(-50, 1200);
            tChar.y = (charCount/6+0.5)*140;//ofRandom(-30, 700);
            tChar.z = 0;//ofRandom(-100, 1200);

            itr++;
        }

        charList.push_back(tChar);
    }

    /* ランダム選択処理
    for(int i=0; i<(int)ofRandom(charPartsPathMap.size()); i++) {
        sel = itr->first;
        cout << sel << endl;
        itr++;
    }*/
    /*
    cout << "random image: " << sel << endl;
    img = imgCharPartsMap[sel];
    */
    
    // 初期化 -----------------------------------------------------------------
    
    screenFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	maskFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

    gui = new ofxUICanvas(0,0,600,400);
    ofSetBackgroundAuto(false);
    ofDisableArbTex();

    oscReceiver.setup( PORT );          // OSC受信用ポート設定

	//cam.setNearClip(0.f);
	//cam.setFarClip(-1000.f);
	//cam.enableOrtho();
    
    //cam.setupPerspective();
    
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
    

    // UI部品を追加 ----------------------------------------------------------

    //gui->setFont("GUI/NewMedia Fett.ttf");
    gui->setFontSize(OFX_UI_FONT_MEDIUM, 7);
    gui->setColorBack(ofxUIColor(0,0,0,40));
    gui->addWidgetRight(new ofxUIButton(32, 24, false, "LOAD_PIXEL_ART"));
    gui->addWidgetRight(new ofxUIButton(32, 24, false, "SAVE_SCREENSHOT"));
    //gui->addWidgetRight(new ofxUIButton(32, 24, false, "BG FILE OPEN"));
    //gui->addWidgetRight(new ofxUIToggle(32, 24, false, "FULLSCREEN"));
    gui->addSpacer(600, 15);
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
    
    
    ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
    
    // パラメータの初期値の設定

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
    
    // 今は隠しておきたいパラメータを下に列記
    gui->getWidget( prmMap["BG_A"]->name)->setVisible(false);

    //gui->loadSettings("GUI/guiSettings.xml");               // 最後に実行しないと実行時エラー
    //gui->getWidget( prmMap["COLOR_A"]->name)->setVisible(false);

    // --------------------------------------------------------------------------
    
    _imgLoad();

    // ファイルを閉じる
}

//--------------------------------------------------------------
void ofApp::update(){

    //cam.setFov(prmMap["CAM_FOV"]->floatVal);
    //cam.setNearClip(prmMap["CAM_NEAR"]->floatVal);
    //cam.setFarClip(prmMap["CAM_FAR"]->floatVal);

    rightClick = false;

    receiveOscMessage();

    {
    	stringstream ss;
	    ss << "pixeGirls+ DungeonWitches  ver.0.0.1a - fps:" << ofGetFrameRate();
	    ofSetWindowTitle(ss.str());
    }
    
    ofSeedRandom(prmMap["RANDOMSEED"]->intVal);
    
}


void ofApp::_imgLoad(){

    imgPixels = img.getWidth() * img.getHeight();
    /*
    imgColorLst.clear();
    for(int i=0; i<imgPixels; i++) {
        imgColorLst.push_back( img.getColor( i%(int)img.getWidth(), i/(int)img.getWidth() ) );
    }
    */
    
    int particleImgNum = particleImg.getWidth() / particleImg.getHeight();
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    srand(time(NULL));
    ofSetColor(255);
    
    //screenFbo.begin();
    //cam.begin();
    
	//ofEnableAlphaBlending();
	//ofEnableAntiAliasing();
    //ofEnableSmoothing();
    //glEnable(GL_MULTISAMPLE);
    //glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    
    //ofSetupScreenOrtho();

    //ofClear(0,0,0,255);
	ofClear(ofColor::fromHsb(prmMap["BG_H"]->floatVal, prmMap["BG_S"]->floatVal, prmMap["BG_B"]->floatVal, prmMap["BG_A"]->floatVal));
    ofBackgroundGradient(ofColor::fromHsb(prmMap["BG_H"]->floatVal, prmMap["BG_S"]->floatVal, prmMap["BG_B"]->floatVal, prmMap["BG_A"]->floatVal), ofColor::fromHsb(prmMap["BG_H"]->floatVal, prmMap["BG_S"]->floatVal, prmMap["BG_B"]->floatVal/3, prmMap["BG_A"]->floatVal), OF_GRADIENT_CIRCULAR);
    
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
    
    
	//maskFbo.begin();
	//ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    
    
	//ofEnableAlphaBlending();
	//ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    
    
    int imgWidth = img.getWidth();
    int imgHeight = img.getHeight();
    float _size = prmMap["SIZE"]->floatVal;
    float dotSize = prmMap["DOT_SIZE"]->floatVal;
    float posX = prmMap["POS_X"]->floatVal;
    float posY = prmMap["POS_Y"]->floatVal;
    float rotateX = prmMap["ROTATE_X"]->floatVal;
    float rotateY = prmMap["ROTATE_Y"]->floatVal;
    float rotateZ = prmMap["ROTATE_Z"]->floatVal;
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
    pitch = _size;

	//ofBackground(192,128,160);
    //int bgColor = 224;
    //ofBackground(bgColor);
    


    for(int charCount=0; charCount<charList.size(); charCount++){

        Char tChar = charList[charCount];

        /* キャラごと色相変化
        int partsH = (int)ofRandom(0, 255);
        int partsS = (int)ofRandom(0, 255);
        int partsB = (int)ofRandom(0, 255);
        */
            // パーツごと色相変化
            int partsH = (int)ofRandom(0, 255);
            int partsS = (int)ofRandom(0, 255);
            int partsB = (int)ofRandom(0, 255);
        
        //cout << "char draw" << endl;
        //auto imgItr = tChar.imgMap.begin();

        //cout << "char parts num: " << tChar.imgMap.size() << endl;

        for(int categoryCount=0; categoryCount < charPartsDrawOrder.size(); categoryCount++) {

            string partsCategoryName = charPartsDrawOrder[categoryCount];
            //cout << "partsCategoryName: " << partsCategoryName << endl;
            
            img = *tChar.imgMap[partsCategoryName];
            //imgItr++;



            //cout << "parts load start: " <<partsCategoryName << " " << tChar.partsMap[partsCategoryName]<< endl;

            auto indexImg = tChar.indexImgMap.at(partsCategoryName );
            //cout << "access: " << indexImg << "partsCategoryName" << endl;
            
            auto palette = tChar.imgMapPalette.at(partsCategoryName );
            //cout << "access: " << palette << "partsCategoryName" << endl;
            
            //cout << "parts loaded: " << endl;

            ofPushMatrix();
    
            //ofTranslate(-img.getWidth()/2*size,-img.getHeight()/2*size);
	        //		ofRotate(180,rotateX,rotateY,rotateZ);
            /*
            ofRotateX(rotateX);
            ofRotateY(rotateY);
            ofRotateZ(rotateZ);
            */
            ofColor c;
            float h, s, bri;
	        for(int i=0; i<img.getHeight(); i++) {
		        for(int j=0; j<img.getWidth(); j++) {
            
                    //cout << "x:" << j << " y:" << i << " - "   << endl; 
			        //c = img.getColor(j, i);
                    vector<unsigned char> t = palette.at(indexImg[i][j]);
                    ofColor c = ofColor(t[0], t[1], t[2], t[3]);

                    
			        if (c.a == 0) {       // ピクセルが透過色の場合、描画処理をスキップする（高速化のため。（if文とどちらが重いのかは？
                        continue;
			        } 
                    ofPushMatrix();

                    c.getHsb(h, s, bri);
           
                    /*
                    c.r += addR;
                    c.g += addG;
                    c.b += addB;
                    if (c.r > 255) {
                        c.r = 255;
                    }*/

                    //c.setHue(c.getHue() + partsH); // 色相変換
                    //ofSetColor(c);


                /*
                    float b2 = (((int)b)/3)*3;
                    if (b2 > 255) {
                        b2 = 255;
                    }
                    if(h>=256) {
                        cout << h << endl;
                    }*/
                    if (indexImg[i][j] < 16) {
                        ofSetColor(ofColor::fromHsb((( int)h)%256 , s, bri));
                    } else {
                        ofSetColor(ofColor::fromHsb((( int)h+partsH)%256 , s, bri));
                    }

                    //cout << " " << c.r << " " << c.g << " " << c.b << endl;
                    //ofTranslate(j, i);
                                                        //ofTranslate(j*pitch+posX + ofRandomuf()*posRandomize, i*pitch+posY + ofRandomuf()*posRandomize);
                    ofTranslate(j*pitch+posX + ofRandomuf()*posRandomize - (pitch*img.getWidth()/2), i*pitch+posY + ofRandomuf()*posRandomize - (pitch * img.getHeight()/2));
                                                        //ofScale(size/256,size/256);
                   
                    ofRotateX(rotateX);
                    ofRotateY(rotateY);
                    ofRotateZ(rotateZ);
                                                            //ofRotate(45,0,0,1);
           
                        //particleImg.drawSubsection(0, 0, pSize, pSize, pSize*(int)ofRandom(pWidth/pSize), 0, pSize, pSize);
                        int tNum = (int )pWidth/(int)pSize;
           
                        //int penSlashNum = abs(((int)bgColor-(int)b))/tNum;
                        int penSlashNum = tNum/2 - ((int )bri / tNum) + 4;
                        if (penSlashNum > (tNum/2-1)) {
                            penSlashNum = tNum/2-1;
                        }
                        penSlashNum = 0;
           
                        //ofRect(0,0, pSize, pSize);
                        //ofRect(-particleImg.getWidth()/2,-particleImg.getHeight()/2, pSize, pSize);
                        particleImg.drawSubsection(tChar.x, tChar.y, pitch*dotSize, pitch*dotSize, (penSlashNum * 2 + (int)ofRandom(tNum)) * pSize, 0, pSize, pSize);
           
            ofPopMatrix();






                    //stringstream ss;
                    //ss << "x:" << j << "y:" << i << " - " << c.r << "," << c.g << "," << c.b << endl;
                    //cout << ss.str();

                
                        //cout << pSize;
		        }
	        }
            ofPopMatrix();
        }
    }
	
	//maskFbo.end();

	//ofSetColor( ofColor::fromHsb(compH, compS, compB) );

	
	//ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	//maskFbo.draw(0,0);

	//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    
    
    {
        stringstream ss;
        ss << "ofScr_" << ofGetFrameNum() << ".png";
        //ofSaveScreen(ss.str());
        //ofSaveFrame();
    }
	
	//particleImg.draw(0, 0);
     
    
    //cam.end();
    //screenFbo.end();

    //ofEnableAlphaBlending();
	//ofEnableAntiAliasing();

    //ofBackground(0);
    
    //_drawTransparentTile();

    //screenFbo.draw(0, 0);

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    return;

}

// 半透明を表す市松模様を描く
void ofApp::_drawTransparentTile() {

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

void ofApp::receiveOscMessage() {
    
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
        
        //dumpOsc(m);   // OSCメッセージをそのままコンソールに出力 (本番では重いのでコメントアウト
    }
    
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' ') {                   // space key で GUI のOn/Off
        
        if(gui->isEnabled()) {
            gui->disable();
        } else {
            gui->enable();
        }
        
    } else if (key == 's') {             // 's' key で screen shot
        _saveScreenShot();
    }
}

void ofApp::_saveScreenShot() {
    
    //ofSaveFrame();

    stringstream ss;

    // 保存するファイルパスを生成 --------------
    ss << "record/";
    ss << ofGetYear();
    ss << boost::format( "%02d" ) % ofGetMonth();
    ss << boost::format( "%02d" ) % ofGetDay();
    ss << "_";
    ss << boost::format( "%02d" ) % ofGetHours();
    ss << boost::format( "%02d" ) % ofGetMinutes();
    ss << boost::format( "%02d" ) % ofGetSeconds();
    
    // 撮影日時が前回と同じ場合はファイル名に連番を追加
    string dateTime = ss.str();
    if (dateTime == prevScreenShotDateTime) {
        screenShotCounter++;
    } else {
        screenShotCounter = 0;
    }
    ss << "_" << (screenShotCounter+1);
    ss << ".png";
    
    // スクリーンショットを保存 --------------
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
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
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
void ofApp::mousePressed(int x, int y, int button){

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
void ofApp::mouseReleased(int x, int y, int button){

    if (button == 2) {
        rightClick = true;
    }

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit(){

    gui->saveSettings("GUI/guiSettings.xml"); 
    delete gui; 

}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{

    string name = e.widget->getName();
    
    if (name == "LOAD_PIXEL_ART")
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

    else if (name == "SAVE_SCREENSHOT")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        
        if (!button->getValue()) {
            _saveScreenShot();
        }        
    }

    else if(name == "FULLSCREEN")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());   
    }

    else if(name == "BLEND_MODE")
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

    else if(name == "BLEND_MODE")
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
    
    else if(name == "P_IMG_PATTERN") {
        
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
    
    else if(name == "PARTICLE_IMAGE") {
        
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

    else if(name == "TILE_IMAGE") {
        
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        for(int i=0; i<selected.size(); i++) {
        
            stringstream ss;
            ss << "tileImg/" << selected[i]->getName();
            
            tileImg.loadImage(ss.str());
        }
    }

    else if(name == "PIXEL_ART") {
        
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        
        for(int i=0; i<selected.size(); i++) {
            stringstream ss;
            ss << "pixelArt/" << selected[i]->getName();
            
            img.loadImage(ss.str());
            _imgLoad();
        }
    }

    else if(name == "BG FILE OPEN")
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

    else {
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

// 2014/11/22 referenced:  http://gmoon.jp/png/
//http://yahirohumpty.blog2.fc2.com/blog-entry-280.html
vector<vector<unsigned char>> getPngIndexImage(string filePath) {
    
    vector<vector<unsigned char>> img;
    FILE            *fp;
	png_struct     *png_ptr;
	png_info       *info_ptr;
	unsigned int    width, height;
	int             bit_depth, color_type, interlace_type;
	unsigned char   **image;
	int             i;
    int fsize;
    unsigned char *filebuf;
  int bdepth,ctype;

	
	if ( (fp = fopen(filePath.c_str(), "rb"))  == NULL){                         // まずファイルを開きます
        cout << "png file load error" << endl;
        return img;
    }
    
  fseek(fp,0,SEEK_END);
  fsize=ftell(fp);
  fseek(fp,0,SEEK_SET);
  
  filebuf=(unsigned char *)malloc(fsize);
  
  fread(filebuf,fsize,1,fp);

  
  if(!png_check_sig(filebuf,fsize)){
    fprintf(stderr,"File is not PNG: %s\n","Yukkuri.png");
    fclose(fp);
    return img;
  }
  

	png_ptr = png_create_read_struct(                       // png_ptr構造体を確保・初期化します
	                PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	info_ptr = png_create_info_struct(png_ptr);             // info_ptr構造体を確保・初期化します


            if( setjmp(png_jmpbuf(png_ptr)) )
        {
                png_destroy_read_struct(&png_ptr,&info_ptr,(png_infopp)NULL);
                fclose(fp);
                return img;
        }


            
  png_set_read_fn(png_ptr, (png_voidp)&filebuf,(png_rw_ptr)pngreadfunction);

//            png_init_io(png_ptr, fp);                               // libpngにfpを知らせます
    
	png_read_info(png_ptr, info_ptr);                       // PNGファイルのヘッダを読み込みます       // ここからクラッシュ
    
	png_get_IHDR(png_ptr, info_ptr, &width, &height,        // IHDRチャンク情報を取得します
	                &bit_depth, &color_type, &interlace_type,
	                NULL, NULL);


    // ここからオリジナル ---- 

	image = (png_bytepp)malloc(height * sizeof(png_bytep)); // 以下３行は２次元配列を確保します
    for (i = 0; i < height; i++) {
	   image[i] = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    png_read_image(png_ptr, image);                         // 画像データを読み込みます

    //

    int count = 0;
    for(int i=0; i<height; i++) {

        vector<unsigned char> lineImg;

        for(int j=0; j<width; j++) {

            lineImg.push_back((unsigned char)image[i][j]);
            count++;
        }

        img.push_back(lineImg);
    }

    for (i = 0; i < height; i++) {
        free(image[i]);
    }            // ２次元配列を解放します
	free(image);

  	png_destroy_read_struct(                                // ２つの構造体のメモリを解放します
	        &png_ptr, &info_ptr, (png_infopp)NULL);
	fclose(fp);                                             // ファイルを閉じます


    return img;
}


// 2014/11/21 referenced:  http://d.hatena.ne.jp/shinji210/20061219
// http://d.hatena.ne.jp/yutopp/20110703/1309689369
vector<vector<unsigned char>> getPaletteFromPNG(string filePath)
{
	cout << "palette load start " << endl;
    
    vector<vector<unsigned char>> palette;
	// "IHDR"チャンク
	int offset = 8;
	int headerLength, chunkLength;
    int l;
    size_t fileSize;
    int imgWidth, imgHeight;

	cout << "palette png file  " << filePath  << endl;
    // ファイル読み込み
    std::vector<unsigned char> buf;
    std::ifstream fs( filePath, std::ios_base::in | std::ios_base::binary );
    if ( !fs )
	    return palette;
    
	cout << "palette png file opened  " << endl;
    fileSize = (size_t)fs.seekg(0, std::ios::end).tellg();
    fs.seekg(0, std::ios::beg);     // ストリームのポインタを一番前に戻して、これから先で使いやすいようにする
    
    unsigned char data;
    for(int i = 0; i < fileSize; i++){
        //fin >> x;
        fs.read((char*)&data, sizeof(char));
        buf.push_back((unsigned char)data);
        //cout << std::to_string(data) << ", ";    // 確認用
    }
    cout << endl;
    
    fs.close();


	// PNGシグネチャ
	if(!(buf[0] == 0x89) &&
			(buf[1] == 0x50) &&
			(buf[2] == 0x4E) &&
			(buf[3] == 0x47) &&
			(buf[4] == 0x0D) &&
			(buf[5] == 0x0A) &&
			(buf[6] == 0x1A) &&
			(buf[7] == 0x0A))
    {
		return palette;
	}
	cout << "palette png sign checked " << endl;
    
    offset += 8;

	// IHDRチャンクサイズ
	l = buf[offset + 0];
	l = (l << 8) + buf[offset + 1];
	l = (l << 8) + buf[offset + 2];
	l = (l << 8) + buf[offset + 3];
    headerLength = l;

    offset += 4;

	// IHDR:
    offset = 16;
    imgWidth = (buf[offset+0] << 24) + (buf[offset+1] << 16) + (buf[offset+2] << 8) + buf[offset+3];

    imgHeight = (buf[offset+4] << 24) + (buf[offset+5] << 16) + (buf[offset+6] << 8) +buf[offset+7];
    
    cout<<"size:" << "wh" << imgWidth<<" "<<imgHeight<<endl;
    // カラータイプがパレット(= 3)か
	if(buf[25] != 3) {
        cout << "indexImage png color type is "<< std::to_string(buf[offset + 1]) << endl;
		return palette;
	}
    cout << "palette png color type checked " << endl;

	//offset += headerLength + 4;

	offset = 16 + headerLength + 4;

	// "tRNS"チャンクを探す
	int transparentColorId = -1;
	while(offset < fileSize) {
		// チャンクサイズ
		l = buf[offset + 0];
		l = (l << 8) + buf[offset + 1];
		l = (l << 8) + buf[offset + 2];
		l = (l << 8) + buf[offset + 3];
        chunkLength = l;

		// チャンクID tRNS
		if((buf[offset + 4] == 0x74) &&
				(buf[offset + 5] == 0x52) &&
				(buf[offset + 6] == 0x4E) &&
				(buf[offset + 7] == 0x53))
		{
			// 透過色を探す
			int idx = offset + 8;
			for(int i = 0; i < chunkLength; i++) {
				if(buf[idx] == 0) {
					transparentColorId = i;
					break;
				}
				idx++;
			}
			break;
		} else {
			offset += 8 + chunkLength + 4;
		}
	}
    	cout << "palette  png tRNS chunk checked " << endl;
        
					transparentColorId = 0;

	// "PLTE"チャンク
	offset = 37;
	// チャンクサイズ
	l = buf[offset + 0];
	l = (l << 8) + buf[offset + 1];
	l = (l << 8) + buf[offset + 2];
	l = (l << 8) + buf[offset + 3];
    chunkLength = l;
	offset += 4;

	// チャンクID PLTE
//	if((buf[offset + 0] == 0x50) &&
//			(buf[offset + 1] == 0x4C) &&
//			(buf[offset + 2] == 0x54) &&
//			(buf[offset + 3] == 0x45))
//	{
        cout << "palette png PLTE chunk checked " << endl;
        offset = 41;
        for(int i=0; i<256; i++) {
            vector<unsigned char> color;
            color.clear();
            for (int j=0; j<3; j++) {        // RGB値を取得
                color.push_back( (unsigned char)buf[offset] );
                offset++;
            }

            // アルファチャンネル値を追加
            if (i == transparentColorId) {
                color.push_back(0);     // transparent alpha value
            } else {
                color.push_back(255);   // non transparent alpha value
            }

            palette.push_back(color);
        }

		return palette;
	//} 
    
	cout << "palette loaded" << endl;

	return palette;
}



// 2014/11/6 referenced:  http://qiita.com/episteme/items/0e3c2ee8a8c03780f01e
map<string, string> getDirectoryFileListRecursive(string targetDir) {

    map<string, string> pathList;

    namespace sys = std::tr2::sys;
    sys::path p(targetDir); // 列挙の起点
    //std::for_each(sys::directory_iterator(p), sys::directory_iterator(),
    //  再帰的に走査するならコチラ↓
    std::for_each(sys::recursive_directory_iterator(p), sys::recursive_directory_iterator(),
        [&](const sys::path& p) {
            if (sys::is_regular_file(p)) { // ファイルなら...
                cout << "file: " << p.string() << endl;     // "/" << p.filename()
                string parentDirName = p.parent_path().filename();

                pathList[ parentDirName + "/" + p.stem() ] = p.string();

            } else if (sys::is_directory(p)) { // ディレクトリなら...
                cout << "dir.: " << p.string() << endl;
            }
    });

    return pathList;
}


void pngreadfunction(png_struct *png,png_bytep buf,png_size_t size){
  unsigned char** p = (unsigned char**)png_get_io_ptr(png);
  memcpy(buf, *p, size);
  *p += (int)size;
}