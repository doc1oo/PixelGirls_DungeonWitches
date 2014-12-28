#include "ofApp.h"


void ofApp::setup(){

    trace("setup() started.");

    // 標準入出力をファイルに変更
    //ofstream ofs("debug.log");
    //cout.rdbuf(ofs.rdbuf());

    srand(time(NULL));
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false); 

    // 変数の初期値設定 -----------------------------------------------

    bgImgPath = "";
    showBgImg = false;
    rightClick = false;
    pImgPattern = 0;
    size = 10;
    prevClickPoint = ofPoint(0,0);
    screenShotCounter = 0;
    key["up"] = 0;
    key["down"] = 0;
    key["left"] = 0;
    key["right"] = 0;
    key["enter"] = 0;
    key["cancel"] = 0;

    charPartsPathList.push_back("body");
    charPartsPathList.push_back("face");
    charPartsPathList.push_back("eye");
    charPartsPathList.push_back("hair");
    charPartsPathList.push_back("hairAcce");

    charPartsDrawOrder.push_back("backAcce");
    charPartsDrawOrder.push_back("weapon");
    charPartsDrawOrder.push_back("body");
    charPartsDrawOrder.push_back("face");
    charPartsDrawOrder.push_back("eye");
    charPartsDrawOrder.push_back("hair");
    charPartsDrawOrder.push_back("hairAcce");

    //ttFont.loadFont("font/07やさしさゴシック.ttf", 72);
    font.loadFont("font/NotoSansCJKjp-Light.otf", 32);
    boldFont.loadFont("font/NotoSansCJKjp-Bold.otf", 32);
    prettyFont.loadFont("font/07やさしさゴシック.ttf", 32);
    pxFont.loadFont("font/misaki_gothic.ttf", 28);
    pxFontBig.loadFont("font/misaki_gothic.ttf", 64);

    for(int i=0; i<BIG_MAP_SIZE_H; i++) {
        for(int j=0; j<BIG_MAP_SIZE_W; j++) {
            if (i == 0 || i == BIG_MAP_SIZE_W-1 || j == 0 || j == BIG_MAP_SIZE_W-1) {
                bigMap[i][j] = 1;       // ダンジョンの外壁
            } else {
                bigMap[i][j] = (int)ofRandom(10);
            }
        }
    }

    // GUI関係の情報設定 -----------------------------------------------
    {
        int w = 120;
        int h = 12;
        prmLst.push_back( AppParameter("SIZE", &typeid(float), "slider", 0, 50, 5, w, h, "bottom") );
        prmLst.push_back( AppParameter("DOT_SIZE", &typeid(float), "slider", 0, 5, 1, w, h, "right") );
        prmLst.push_back( AppParameter("POS_X", &typeid(float), "slider", -1000, 1000, 0, w, h, "bottom") );
        prmLst.push_back( AppParameter("POS_Y", &typeid(float), "slider", -1000, 1000, 0, w, h, "right") );
        prmLst.push_back( AppParameter("POS_Z", &typeid(float), "slider", -1000, 0, 0, w, h, "right") );
        prmLst.push_back( AppParameter("ROTATE_X", &typeid(float), "slider", -360, 360, 0, w, h, "bottom") );
        prmLst.push_back( AppParameter("ROTATE_Y", &typeid(float), "slider", -360, 360, 0, w, h, "right") );
        prmLst.push_back( AppParameter("ROTATE_Z", &typeid(float), "slider", -360, 360, 0, w, h, "right") );
        prmLst.push_back( AppParameter("CAM_ROT_X", &typeid(float), "slider", -360, 360, 0, w, h, "bottom") );
        prmLst.push_back( AppParameter("CAM_ROT_Y", &typeid(float), "slider", -360, 360, 0, w, h, "right") );
        prmLst.push_back( AppParameter("CAM_ROT_Z", &typeid(float), "slider", -360, 360, 0, w, h, "right") );
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
	bgParticleImg.loadImage("particleImg/posca.tif"); //circleAlpha.tif");
	bgImg.loadImage("img/mapchip.png");
	tileImg.loadImage("tileImg/default.png");
	imgTitleBanner.loadImage("img/pixelGirlsBannerMini.png");
    sndMap["se_screen_shot"].loadSound("se_screen_shot.wav");

    auto charPartsPathMap = getDirectoryFileListRecursive("./data/img/charParts/");

    for (auto path : charPartsPathMap) {

        ss << "vec-output: " << path.first << " " << path.second << endl;
        trace(&ss);

        auto p = std::tr2::sys::path(path.second);
        string parentDirName = p.parent_path().filename();
        string fileName = p.stem();

        ofImage tImg;
        auto res = tImg.loadImage("../" + path.second);
        ss << "loadImage: [" << parentDirName << "][" << fileName << "] " << "../" << path.second;
        trace(&ss);

        if (res) {
            ss << " - OK" << endl;
            trace(&ss);

            charPartsMap[parentDirName][fileName] = tImg;

            ss << parentDirName << fileName << endl;
            trace(&ss);

            indexImgMap[parentDirName + fileName] = getPngIndexImage(path.second);//getIndexImageFromPNG(path.second);

            imgMapPalette[parentDirName + fileName] = getPaletteFromPNG(path.second);

            ss << endl;
            trace(&ss);

        } else {
            ss << " - NG";
            trace(&ss);
        }

    }
    
    // random charMake -------------------------------------------------
    charList.clear();

    // プレイヤーキャラ生成
    {
        playerChar = Char();
        
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

            playerChar.partsMap[index] = imgFileName;
            playerChar.imgMap[index] = &charPartsMap[index][imgFileName];
            playerChar.indexImgMap[index] = indexImgMap[index+imgFileName];
            playerChar.imgMapPalette[index] = imgMapPalette[index+imgFileName];

            playerChar.x = ofRandom(-40, 40);
            playerChar.y = ofRandom(-40, 40)+2500;
            playerChar.z = 0;//ofRandom(1, 300);
            if (ofRandom(0, 100) >= 50) {
                playerChar.dir = LEFT;
            } else {
                playerChar.dir = RIGHT;
            }

            itr++;
        }
        charList.push_back(playerChar);
    }

    for (int charCount = 0; charCount < 16; charCount++) {

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

            tChar.x = (charCount%4)*280 + ofRandom(-40, 40);
            tChar.y = (charCount/4)*160 + ofRandom(-40, 40);
            tChar.z = 0;//ofRandom(1, 300);
            if (ofRandom(0, 100) >= 50) {
                tChar.dir = LEFT;
            } else {
                tChar.dir = RIGHT;
            }

            itr++;
        }

        charList.push_back(tChar);
    }

    /* ランダム選択処理
    for(int i=0; i<(int)ofRandom(charPartsPathMap.size()); i++) {
        sel = itr->first;
        ss << sel << endl;
        itr++;
    }*/
    /*
    ss << "random image: " << sel << endl;
    img = imgCharPartsMap[sel];
    */
    
    // システム初期化 -----------------------------------------------------------------
    
    screenFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	maskFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	tempFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

    gui = new ofxUICanvas(0,0,600,400);
    ofSetBackgroundAuto(false);
    ofDisableArbTex();

    oscReceiver.setup( PORT );          // OSC受信用ポート設定

    cam.setFov(80.0);
	cam.setPosition(0, 2000, 200);
    //cam.lookAt(ofVec3f(0,0,1));
    //cam.lookAt(ofVec3f(0,-1,0));//, ofVec3f(0,0,1));     // 見てほしい方向, 頭の向き
    cam.lookAt(ofVec3f(0,-1,0));
    cam.setVFlip(true);

    cam.setNearClip(10);
    cam.setFarClip(1000);
	cam.setNearClip(0.f);
	cam.setFarClip(-1000.f);
	//cam.enableOrtho();
    
    //cam.setupPerspective();

    easyCam.setFov(80);
    easyCam.setFarClip(30000);          // デフォルト設定より遠くまで見えるように
    easyCam.setNearClip(0);         // 手前のオブジェクトが消えるように

    
    ss << "cam:" << endl;
    ss << cam.getNearClip() << endl;
    ss << cam.getFarClip() << endl;
    ss << cam.getFov() << endl;
    ss << cam.getAspectRatio() << endl;
    ss << cam.getForceAspectRatio() << endl;
    trace(&ss);
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
        
        //ss << "dir: ";
        
        vector<ofFile> files = ofDir.getFiles();
        vector<ofFile>::iterator itr = files.begin();  // イテレータのインスタンス化
        while( itr != files.end() )  // 末尾要素まで
        {
            if (!itr->isDirectory()) {
                //ss << itr->getFileName() << endl;  // *演算子で間接参照
                
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
                //ss << itr->getFileName() << endl;  // *演算子で間接参照
                
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
                //ss << itr->getFileName() << endl;  // *演算子で間接参照
                
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

    trace("setup() finished.");
    
    glDisable(GL_CULL_FACE);        // カリングしない

    //easyCam.setDistance(500);
    //ofEnableDepthTest();            // 深度テストを有効にすると、z座標の値によって前後関係が正しく表現されるが、透過が効かずに、描画順が命令順どおりにならなくなる
}



// メイン更新処理 --------------------------------------------------------------
void ofApp::update(){
    
    trace("update() start");

    //cam.setFov(prmMap["CAM_FOV"]->floatVal);
    //cam.setNearClip(prmMap["CAM_NEAR"]->floatVal);
    //cam.setFarClip(prmMap["CAM_FAR"]->floatVal);

    rightClick = false;

    //receiveOscMessage();

    {
    	stringstream s;
	    s << "pixeGirls+ DungeonWitches  ver.0.0.1a - fps:" << ofGetFrameRate();
	    ofSetWindowTitle(s.str());
    }
    
    ofSeedRandom(time(NULL));
    
    for(auto &tChar : charList) {

        cout << tChar.x << endl;

        //Char *tChar = &charList[0];
        if (key["left"] == 1) {
            tChar.x -= 5;
            tChar.dir = LEFT;
        } else if (key["right"] == 1) {
            tChar.x += 5;
            tChar.dir = RIGHT;
        } else if (key["up"] == 1) {
            tChar.y -= 5;
            tChar.dir = LEFT;
        } else if (key["down"] == 1) {
            tChar.y += 5;
            tChar.dir = LEFT;
        }
        

        if (tChar.action == ACT_NONE) {
            if ((int)ofRandom(0, 20) == 0) {
                tChar.action = ACT_ATTACK;
                tChar.actCount = 0;
                tChar.actTime = 10;
            }
        }

        if (tChar.action != ACT_NONE) {

            if (tChar.actCount >= tChar.actTime) {
                tChar.action = ACT_NONE;
                tChar.actCount = 0;
            } else {
            }

            tChar.actCount++;

        }

    
        tChar.count++;
    }

}



void ofApp::_imgLoad(){
    
    //particleImgNum = particleImg.getWidth() / particleImg.getHeight();
    
}



// メイン描画処理 --------------------------------------------------------------
void ofApp::draw(){

    trace("draw() start");
    
    
    ofClear(0,0,0,255);
    //ofClear(255,255,255,255);
    ofBackgroundGradient(ofColor::fromHsb(prmMap["BG_H"]->floatVal, prmMap["BG_S"]->floatVal, prmMap["BG_B"]->floatVal, prmMap["BG_A"]->floatVal), ofColor::fromHsb(prmMap["BG_H"]->floatVal, prmMap["BG_S"]->floatVal, prmMap["BG_B"]->floatVal/3, prmMap["BG_A"]->floatVal), OF_GRADIENT_CIRCULAR);
    
    easyCam.begin();
    //cam.begin();

    ofEnableDepthTest();

    // ライティングを有効に
    light.enable();
    // スポットライトを配置
    light.setPointLight();
    // 照明の位置
    light.setPosition(-100, 100, 1000);
    // 環境反射光の色
    light.setAmbientColor(ofFloatColor(0.5, 0.5, 0.5));
    // 拡散反射光の色
    light.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
    // 鏡面反射光の色
    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0)); 

    /*
    std::mt19937 engine;
    std::vector< std::uint_least32_t > v;
    v.push_back( prmMap["randomize"]);
    std::seed_seq seed( v.begin(), v.end() ) ;
    std::mt19937 engine( seed ) ;
    */
    ofSeedRandom(prmMap["RANDOMSEED"]->intVal);

    ofSetColor(255);
    glReadBuffer(GL_FRONT);         // ofSaveScreenで画像が保存されない問題への対処
    //screenFbo.begin();
    //cam.begin();
    
	//ofEnableAlphaBlending();
	//ofEnableAntiAliasing();
    //ofEnableSmoothing();
    //glEnable(GL_MULTISAMPLE);
    //glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    
    //ofSetupScreenOrtho();
    

    if (showBgImg) {
        float imgAspect = img.getWidth() / img.getHeight();
        bgImg.draw(0,0,ofGetWidth(), ofGetWidth() / imgAspect);
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
    float posZ = prmMap["POS_Z"]->floatVal;
    float rotateX = prmMap["ROTATE_X"]->floatVal;
    float rotateY = prmMap["ROTATE_Y"]->floatVal;
    float rotateZ = prmMap["ROTATE_Z"]->floatVal;
    float camRotX = prmMap["CAM_ROT_X"]->floatVal;
    float camRotY = prmMap["CAM_ROT_Y"]->floatVal;
    float camRotZ = prmMap["CAM_ROT_Z"]->floatVal;
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
    
    

    int objSize = 128*5;
    for(int i=0; i<17; i++) {
        for(int j=0; j<17; j++) {
            if (bigMap[i][j] == 1) {
                ofBoxPrimitive box;
                ofTexture tex;

                box.set(objSize);
                ofColor(255,255,255);
                box.setPosition(j*objSize, i*objSize,objSize/2);
                box.draw();
            } else if (bigMap[i][j] == 2) {
                ofConePrimitive cone;
                cone.set(objSize/2, objSize, 12, 1);
                cone.rotate(-90,1.0,0,0);
                ofColor(255,255,255);
                cone.setPosition(j*objSize, i*objSize,objSize/2);
                cone.draw();

            }
        }
    }
    
    
    // 背景描画 ---------------------
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);

    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ofPushMatrix();
    
    ofRotateX(camRotX);
    ofRotateY(camRotY);
    ofRotateZ(camRotZ);

    for(int y=0; y<4; y++) {
        for(int x=0; x<6; x++) {

            for(int i=0; i<8; i++) {
                for(int j=0; j<8; j++) {

                    

                    int tSize = bgParticleImg.getHeight();
                    int tpitch = 28;
                    int tdotSize = 2;

                    ofPushMatrix();
                    
                    ofTranslate((x*8+j)*tpitch + ofRandom(0,8) + posX, (y*8+i)*tpitch + ofRandom(0,8)+ posY, ofRandom(0,0)+ posZ);

                    ofRotateZ(ofRandom(-10,10));
                    ofColor c = bgImg.getColor(j+8*8, i+8);
                    ofSetColor(c);

                    bgParticleImg.drawSubsection(0, 0, tpitch*tdotSize, tpitch*tdotSize, 0, 0, tSize, tSize);
                    ofPopMatrix();
                }
            }

        }
    }
    ofPopMatrix();
    glDepthMask(GL_TRUE);
    // --------------------------------

    //light.disable();
    //ofDisableDepthTest();
    

    // ----------------------------------------------------------


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

    // キャラクター描画 ---------------------
    for(int charCount=0; charCount<charList.size(); charCount++){

        Char *tChar = &charList[charCount];
        
        //ss << "char draw" << endl;
        //auto imgItr = tChar.imgMap.begin();

        //ss << "char parts num: " << tChar.imgMap.size() << endl;
        

        int faceAngle = ofRandom(-16,16);

        for(int categoryCount=0; categoryCount < charPartsDrawOrder.size(); categoryCount++) {

            string partsCategoryName = charPartsDrawOrder[categoryCount];
            //ss << "partsCategoryName: " << partsCategoryName << endl;
            

            ofImage *tImg = tChar->imgMap[partsCategoryName];
            //imgItr++;

            //ss << "parts load start: " <<partsCategoryName << " " << tChar.partsMap[partsCategoryName]<< endl;

            vector<vector<unsigned char>> *indexImg = &(tChar->indexImgMap.at(partsCategoryName ));
            //ss << "access: " << indexImg << "partsCategoryName" << endl;
            
            vector<vector<unsigned char>> *palette = &(tChar->imgMapPalette.at(partsCategoryName ));
            //ss << "access: " << palette << "partsCategoryName" << endl;
            
            //ss << "parts loaded: " << endl;

            // パーツごと色相変化
            int partsH = (int)ofRandom(0, 255);
            int partsS = (int)ofRandom(0, 255);
            int partsB = (int)ofRandom(0, 255);

            ofPushMatrix();
    
            ofRotateX(camRotX);
            ofRotateY(camRotY);
            ofRotateZ(camRotZ);

            //ofTranslate(-img.getWidth()/2*size,-img.getHeight()/2*size);
	        //		ofRotate(180,rotateX,rotateY,rotateZ);
            
            int dirFlag = 0;
            if (tChar->dir != RIGHT) {
                dirFlag = -1;
            } else {
                dirFlag = 1;
            }


            ofTranslate(tChar->x+posX , tChar->y+posY , tChar->z+posZ);

            if (partsCategoryName == "weapon") {
                if (tChar->action == ACT_NONE) {
                        ofTranslate(4*pitch*dirFlag, 0*pitch );
                } else{
                    //ofTranslate(4*pitch , 4*pitch );
                }
            }
            
            // キャラごとの回転
            //ofRotateX(rotateX);
            //ofRotateY(rotateY);
            ofRotateX(-90);

            if (partsCategoryName == "hair" || partsCategoryName == "hairAcce" || partsCategoryName == "face" || partsCategoryName == "eye") {
                ofRotateZ(faceAngle);
            } else if (partsCategoryName == "weapon") {

                if (tChar->action == ACT_NONE) {
                    ofRotateZ(rotateZ*dirFlag);     // ofRandom(-90,90)
                } else if (tChar->action == ACT_ATTACK) {
                    ofRotateZ(weaponAttackDeg(tChar->actCount));
                }

            }


            ofColor c;
            float h, s, bri;
	        for(int i=0; i<tImg->getHeight(); i++) {
		        for(int j=0; j<tImg->getWidth(); j++) {

                    int x, y, z;
                    if (tChar->dir != RIGHT) {
                        x = j;
                    } else {
                        x = (tImg->getWidth()-1) - j;
                    }
                    y = i;
            
                    //ss << "x:" << j << " y:" << i << " - "   << endl; 
			        //c = img.getColor(j, i);
                    unsigned char index = (*indexImg)[y][x];
                    vector<unsigned char> *t = &(*palette).at(index);
                    ofColor c = ofColor( (*t)[0], (*t)[1], (*t)[2], (*t)[3] );

			        if (c.a == 0) {       // ピクセルが透過色の場合、描画処理をスキップする（高速化のため。（if文とどちらが重いのかは？
                        continue;
			        } 
                    ofPushMatrix();
                    
                    ofRotateX(-rotateX);
                    ofRotateY(-rotateY);

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
                        ss << h << endl;
                    }*/
                    if ((*indexImg)[y][x] < 16) {
                        ofSetColor(ofColor::fromHsb((( int)h)%256 , s, bri));
                    } else {
                        // 色相の変更で色がどぎつい鮮やかさになる場合があるのを補正？
                        int ts = s;
                        //if (ts > 128) {
                       //     ts = 0;//(s*7)/10;//*2/3;
                       // }
                        if (ts > 80) {
                            ts = s/4+80;
                        }
                        ofSetColor(ofColor::fromHsb((( int)h+partsH)%256 , ts+100, bri));
                    }
                    //ss << " " << c.r << " " << c.g << " " << c.b << endl;
                    //ofTranslate(j, i);
                                                        //ofTranslate(j*pitch+posX + ofRandomuf()*posRandomize, i*pitch+posY + ofRandomuf()*posRandomize);
                    if (partsCategoryName == "weapon") {
//                        ofTranslate((j-tImg->getWidth()/2 + 4)*pitch + ofRandomuf()*posRandomize, (i-tImg->getHeight()/2 - 4)*pitch+ ofRandomuf()*posRandomize);

                        if (tChar->action == ACT_NONE) {
                            ofTranslate((j-tImg->getWidth()/2 - 4*dirFlag)*pitch + ofRandomuf()*posRandomize, (i-tImg->getHeight()/2-8 )*pitch+ ofRandomuf()*posRandomize);
                        } else {
                            ofTranslate((j-tImg->getWidth()/2 + 4*dirFlag)*pitch + ofRandomuf()*posRandomize, (i-tImg->getHeight()/2-8 - 4)*pitch+ ofRandomuf()*posRandomize);
                        }
                                                        //ofScale(size/256,size/256);
                    } else {
                        ofTranslate((j-tImg->getWidth()/2)*pitch + ofRandomuf()*posRandomize, (i-tImg->getHeight()/2-8)*pitch+ ofRandomuf()*posRandomize);
                                                        //ofScale(size/256,size/256);
                    }
                   
                    /*
                    ofRotateX(rotateX);
                    ofRotateY(rotateY);
                    ofRotateZ(rotateZ);
                    */
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
                    if (partsCategoryName == "weapon") {
                        float destSize = pitch * dotSize;
//                        particleImg.drawSubsection(4*pitch, 0*pitch, pitch*dotSize, pitch*dotSize, (penSlashNum * 2 + (int)ofRandom(tNum)) * pSize, 0, pSize, pSize);
                        if (tChar->action == ACT_ATTACK) {
                            particleImg.drawSubsection(-0*pitch*dirFlag, 0*pitch, -0.01*categoryCount, pitch*dotSize, pitch*dotSize, (penSlashNum * 2 + (int)ofRandom(tNum)) * pSize, 0, pSize, pSize);
                        } else {
                            particleImg.drawSubsection(0*pitch*dirFlag, 0*pitch, -0.01*categoryCount, pitch*dotSize, pitch*dotSize, (penSlashNum * 2 + (int)ofRandom(tNum)) * pSize, 0, pSize, pSize);
                        }
                    } else {
                        float destSize = pitch * dotSize;
                        particleImg.drawSubsection(0-destSize/2, 0-destSize/2, -0.01*categoryCount, destSize, destSize, (penSlashNum * 2 + (int)ofRandom(tNum)) * pSize, 0, pSize, pSize);
                    }
           
                    ofPopMatrix();

                    //stringstream ss;
                    //ss << "x:" << j << "y:" << i << " - " << c.r << "," << c.g << "," << c.b << endl;
                    //ss << ss.str();

                    //ss << pSize;
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
        stringstream s;
        s << "ofScr_" << ofGetFrameNum() << ".png";
        trace(&s);
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

    screenShotCounter++;
    stringstream s;
    s << screenShotCounter << ".png";
    //ofSaveScreen(s.str());

    //ofDrawBitmapString("0 1 2 3 4 5 6 7 8 9", 0, 0);
    
    /*
    //easyCam.end();
    ofSetColor(255, 255, 255);
    tempFbo.begin();
    ofBackground(0,0,0,0);
    prettyFont.drawString("0 1 2 3 4 5 6 7 8 9 かわいい子猫Mewmewにゃーにゃー", 40, 340);
    tempFbo.end();
    tempFbo.draw(0, 0);
    */

    prettyFont.drawString("X 0 1 2 3 4 5 6 7 8 9 かわいい子猫Mewmewにゃーにゃー", 0, 0);
    ofPushMatrix();
    ofRotateY(90);
    prettyFont.drawString("Y 0 1 2 3 4 5 6 7 8 9 かわいい子猫Mewmewにゃーにゃー", 0, 0);
    ofPopMatrix();
    ofPushMatrix();
    ofRotateZ(90);
    prettyFont.drawString("Z 0 1 2 3 4 5 6 7 8 9 かわいい子猫Mewmewにゃーにゃー", 0, 0);
    ofPopMatrix();


    //cam.lookAt();

    //cam.end();
    easyCam.end();

    // ステータス表示 ------------------------------------------------------

    //light.disable();
    ofSetColor(255,255,255);

    pxFontBig.drawString("B?+ 1F", 30, 100);
    
    pxFont.drawString("あなた", 30, 170);
    pxFont.drawString("HP 100", 30, 220);
    pxFont.drawString("MP  99", 30, 260);
    
    pxFont.drawString("ないこ", 30, 350);
    pxFont.drawString("HP  40", 30, 390);

    pxFont.drawString("ぴよか", 30, 450);
    pxFont.drawString("HP  30", 30, 490);

    pxFont.drawString("くぴま", 30, 550);
    pxFont.drawString("HP  70", 30, 590);

    imgTitleBanner.draw(ofGetWidth()-360, ofGetHeight()-160, imgTitleBanner.width*7,imgTitleBanner.height*7);

    /*
    font.drawString("0 1 2 3 4 5 6 7 8 9 かわいい子猫Mewmewにゃーにゃー", 40, 40);
    boldFont.drawString("0 1 2 3 4 5 6 7 8 9 かわいい子猫Mewmewにゃーにゃー", 40, 140);
    pxFont.drawString("0 1 2 3 4 5 6 7 8 9 かわいい子猫Mewmewにゃーにゃー", 40, 240);
    */

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
                
                ss << "OSC mes: " << m.getAddress() << endl;
                trace(&ss);

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
                    //ss << "OSC / " << prmLst[i] << ":" << n << endl;

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
                        ss << "t:" << toggles[i]->getName() << endl;
                        trace(&ss);
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
void ofApp::keyPressed(int pressedKey){
    
    if (pressedKey == ' ') {                   // space key で GUI のOn/Off
        
        if(gui->isEnabled()) {
            gui->disable();
        } else {
            gui->enable();
        }
        
    }
    if (pressedKey == 'a') {
        key["left"] = 1;
    } else if (pressedKey == 'd') {
        key["right"] = 1;
    }
    if (pressedKey == 'w') {             // 's' key で screen shot
        key["up"] = 1;
        //_saveScreenShot();
    } else if (pressedKey == 's') {             // 's' key で screen shot
        key["down"] = 1;
        //_saveScreenShot();
    }


    if (pressedKey == 'f') {
        //フルスクリーン on/off の切り替え
        ofToggleFullscreen();
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int pressedKey){
    if (pressedKey == 'a') {
        key["left"] = 0;
    } else if (pressedKey == 'd') {
        key["right"] = 0;
    }
    if (pressedKey == 'w') {             // 's' key で screen shot
        key["up"] = 0;
    } else if (pressedKey == 's') {             // 's' key で screen shot
        key["down"] = 0;
    }

}




void ofApp::_saveScreenShot() {
    
    //ofSaveFrame();

    stringstream s;

    // 保存するファイルパスを生成 --------------
    s << "record/";
    s << ofGetYear();
    s << boost::format( "%02d" ) % ofGetMonth();
    s << boost::format( "%02d" ) % ofGetDay();
    s << "_";
    s << boost::format( "%02d" ) % ofGetHours();
    s << boost::format( "%02d" ) % ofGetMinutes();
    s << boost::format( "%02d" ) % ofGetSeconds();

    // 撮影日時が前回と同じ場合はファイル名に連番を追加
    string dateTime = s.str();
    if (dateTime == prevScreenShotDateTime) {
        screenShotCounter++;
    } else {
        screenShotCounter = 0;
    }
    s << "_" << (screenShotCounter+1);
    s << ".png";
    
    // スクリーンショットを保存 --------------
    ofSaveScreen(s.str());

    /*
    ofPixels ofp;
    screenFbo.readToPixels(ofp);
    ofSaveImage(ofp, ss.str());
    */
    
    sndMap["se_screen_shot"].play();
    
    prevScreenShotDateTime = dateTime;

}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}



//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    /*
    if (!(x < 600 && y < 400) || gui->isEnabled() == false) {
        if (button == 0) { // left click
           
            // prmMap["ROTATE_X"]->floatVal = prevRotateX + (x - prevClickPoint.x);
            // prmMap["ROTATE_Y"]->floatVal = prevRotateY + (y - prevClickPoint.y);
            
            
            ofxUISlider* s = (ofxUISlider*)gui->getWidget("ROTATE_X");
            s->setValue(prevRotateX + (y - prevClickPoint.y));
            s->update();
            s->triggerSelf();
            
            s = (ofxUISlider*)gui->getWidget("ROTATE_Y");
            s->setValue(prevRotateY + (x - prevClickPoint.x));
            s->update();
            s->triggerSelf();
            
        } else if (button == 2) {
            
            // prmMap["ROTATE_X"] = prevRotateX + (x - prevClickPoint.x);
            // prmMap["ROTATE_Y"] = prevRotateY + (y - prevClickPoint.y);
            
            
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
    */
    
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
    
    
    ss << "pressed";
    trace(&ss);

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
            stringstream s;
            s << selected[i]->getName();
            if ( s.str() == "NORMAL") {
                blendMode = 0;
            } else if (s.str() == "ADD") {
                blendMode = 1;
            } else if (s.str() == "MULTIPLY") {
                blendMode = 2;
            } else if (s.str() == "SUB") {
                blendMode = 3;
            } else if (s.str() == "SCREEN") {
                blendMode = 4;
            } else if (s.str() == "ALPHA") {
                blendMode = 5;
            }
            
        }
        ss << blendMode << endl;
        trace(&ss);
        
    }

    else if(name == "BLEND_MODE")
    {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        
        for(int i=0; i<selected.size(); i++) {
            stringstream s;
            s << selected[i]->getName();
            if ( s.str() == "NORMAL") {
                blendMode = 0;
            } else if (s.str() == "ADD") {
                blendMode = 1;
            } else if (s.str() == "MULTIPLY") {
                blendMode = 2;
            } else if (s.str() == "SUB") {
                blendMode = 3;
            } else if (s.str() == "SCREEN") {
                blendMode = 4;
            } else if (s.str() == "ALPHA") {
                blendMode = 5;
            }            
        }
        ss << blendMode << endl;
        trace(&ss);

    }
    
    else if(name == "P_IMG_PATTERN") {
        
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        
        for(int i=0; i<selected.size(); i++) {
            stringstream s;
            s << selected[i]->getName();
            
            if ( s.str() == "RANDOM") {
                pImgPattern = 0;
            } else if (s.str() == "BRIGHTNESS/2") {
                pImgPattern = 1;
            }
            
        }
    }
    
    else if(name == "PARTICLE_IMAGE") {
        
        cout<< "PART_IMAGE!"<<endl;
        
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        
        for(int i=0; i<selected.size(); i++) {
            stringstream s;
            s << "particleImg/" << selected[i]->getName();
            
            ss << "prtImage: " << s.str() << endl;
            trace(&ss);
            
            //ofImage tImg;
            //tImg.loadImage(s.str());
            
            //particleImg.

            //trace("particle:" + s.str());
            
            particleImg.loadImage(s.str());
            
            //_imgLoad();
            
            
            //ofColor centerColor = particleImg.getColor(particleImg.getWidth()/2, particleImg.getWidth()/2);
            
            
        }
    }

    else if(name == "TILE_IMAGE") {
        
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        for(int i=0; i<selected.size(); i++) {
        
            stringstream s;
            s << "tileImg/" << selected[i]->getName();
            
            tileImg.loadImage(s.str());
        }
    }

    else if(name == "PIXEL_ART") {
        
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        
        for(int i=0; i<selected.size(); i++) {
            stringstream s;
            s << "pixelArt/" << selected[i]->getName();
            
            img.loadImage(s.str());
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
            //ss << slider->getName() << endl;
            //ss << typeid(*prmMap[name]).name() << endl;
            string name = (string)slider->getName();

            if (!rightClick) {
                
                if (*prmMap[name]->type == typeid(float)) {
                    prmMap[name]->floatVal = slider->getScaledValue();
                    
                    //ss << "float" << name << ":" << prmMap[name]->floatVal << endl;
                } else if (*prmMap[name]->type == typeid(int)) {
                    string label = slider->getLabel()->getLabel();      // PITCH: 0 のような文字列
                    
                    vector<string> strLst;
                    boost::algorithm::split( strLst, label, boost::algorithm::is_space() );
                    prmMap[name]->intVal = ofToInt( strLst[1] );
                    
                    //ss << "int " << name << ":" << prmMap[name]->intVal << endl;
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
// http://yahirohumpty.blog2.fc2.com/blog-entry-280.html
// 
vector<vector<unsigned char>> getPngIndexImage(string filePath) {
    
    vector<vector<unsigned char>> img;
    stringstream ss;
	unsigned char   **image;
    FILE            *fp;
    unsigned char   *filebuf;
    int             fsize;
	png_struct      *png_ptr;
	png_info        *info_ptr;
	unsigned int    width, height;
	int             bit_depth, color_type, interlace_type;
	int             i;
    int             bdepth,ctype;
	
	if ( (fp = fopen(filePath.c_str(), "rb"))  == NULL){                         // まずファイルを開きます
        ss << "png file load error" << endl;
        trace(&ss);
        return img;
    }
    
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
  
    filebuf = (unsigned char *)malloc(fsize);
  
    fread(filebuf, fsize, 1, fp);
  
    if(!png_check_sig(filebuf, fsize)){
        fprintf(stderr,"File is not PNG: %s\n","Yukkuri.png");
        fclose(fp);
        return img;
    }

	png_ptr = png_create_read_struct(                       // png_ptr構造体を確保・初期化します
	                PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	info_ptr = png_create_info_struct(png_ptr);             // info_ptr構造体を確保・初期化します


    if( setjmp(png_jmpbuf(png_ptr)) ) {
                png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
                fclose(fp);
                return img;
    }
            
    png_set_read_fn(png_ptr, (png_voidp)&filebuf, (png_rw_ptr)pngReadFunction);

    // png_init_io(png_ptr, fp);                               // libpngにfpを知らせます
    
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

    int count = 0;
    for(int i=0; i<height; i++) {

        vector<unsigned char> lineImg;

        for(int j=0; j<width; j++) {

            lineImg.push_back((unsigned char)image[i][j]);
            count++;
        }

        img.push_back(lineImg);
    }

    // ２次元配列を解放します
    for (i = 0; i < height; i++) {
        free(image[i]);
    }
	free(image);

  	png_destroy_read_struct(                                // ２つの構造体のメモリを解放します
	        &png_ptr, &info_ptr, (png_infopp)NULL);

	fclose(fp);                                             // ファイルを閉じます

    return img;
}



// 2014/11/21 referenced:  http://d.hatena.ne.jp/shinji210/20061219
// http://d.hatena.ne.jp/yutopp/20110703/1309689369
//
vector<vector<unsigned char>> getPaletteFromPNG(string filePath) {

    stringstream ss;

	ss << "palette load start " << endl;
    trace(&ss);

    vector<vector<unsigned char>> palette;

	// "IHDR"チャンク
	int offset = 8;
	int headerLength, chunkLength;
    int l;
    int imgWidth, imgHeight;
    size_t fileSize;

	ss << "palette png file  " << filePath  << endl;
    trace(&ss);

    // ファイル読み込み
    std::vector<unsigned char> buf;
    std::ifstream fs( filePath, std::ios_base::in | std::ios_base::binary );
    if ( !fs )
	    return palette;
    
	ss << "palette png file opened  " << endl;
    trace(&ss);

    fileSize = (size_t)fs.seekg(0, std::ios::end).tellg();
    fs.seekg(0, std::ios::beg);                                         // ストリームのポインタを一番前に戻して、これから先で使いやすいようにする
    
    unsigned char data;
    for(int i = 0; i < fileSize; i++){
        fs.read((char*)&data, sizeof(char));
        buf.push_back((unsigned char)data);
        //ss << std::to_string(data) << ", ";    // 確認用
    }
    ss << endl;
    trace(&ss);
    
    fs.close();

	// PNGシグネチャ
	if(!(buf[0] == 0x89) &&
			(buf[1] == 0x50) &&
			(buf[2] == 0x4E) &&
			(buf[3] == 0x47) &&
			(buf[4] == 0x0D) &&
			(buf[5] == 0x0A) &&
			(buf[6] == 0x1A) &&
			(buf[7] == 0x0A)) {
		return palette;
	}
	ss << "palette png sign checked " << endl;
    trace(&ss);
    
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
    
    ss << "size:" << "wh" << imgWidth<<" "<<imgHeight<<endl;
    trace(&ss);

    // カラータイプがパレット(= 3)か
	if(buf[25] != 3) {
        ss << "indexImage png color type is "<< std::to_string(buf[offset + 1]) << endl;
        trace(&ss);
		return palette;
	}

    ss << "palette png color type checked " << endl;
    trace(&ss);

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
				(buf[offset + 7] == 0x53)) {
	
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

    ss << "palette  png tRNS chunk checked " << endl;
    trace(&ss);
        
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
    ss << "palette png PLTE chunk checked " << endl;
    trace(&ss);

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
    
	ss << "palette loaded" << endl;
    trace(&ss);

	return palette;
}



// 2014/11/6 referenced:  http://qiita.com/episteme/items/0e3c2ee8a8c03780f01e
map<string, string> getDirectoryFileListRecursive(string targetDir) {

    map<string, string> pathList;
    stringstream ss;

    namespace sys = std::tr2::sys;
    sys::path p(targetDir);                  // 列挙の起点

    //std::for_each(sys::directory_iterator(p), sys::directory_iterator(),
    //  再帰的に走査するならコチラ↓
    std::for_each(sys::recursive_directory_iterator(p), sys::recursive_directory_iterator(),
        [&](const sys::path& p) {

            if (sys::is_regular_file(p)) { // ファイルなら...
                ss << "file: " << p.string() << endl;     // "/" << p.filename()
                trace(&ss);

                string parentDirName = p.parent_path().filename();

                pathList[ parentDirName + "/" + p.stem() ] = p.string();

            } else if (sys::is_directory(p)) { // ディレクトリなら...
                ss << "dir.: " << p.string() << endl;
                trace(&ss);
            }
    });

    return pathList;
}



void pngReadFunction(png_struct *png,png_bytep buf,png_size_t size){

    unsigned char** p = (unsigned char**)png_get_io_ptr(png);
    memcpy(buf, *p, size);
    *p += (int)size;

}


// デバッグ用のトレース文出力
void trace(stringstream *ss) {

    //cout << ss->str();

    ss->str("");
}



// デバッグ用のトレース文出力
void trace(string s) {

    cout << s << endl;

}


int weaponAttackDeg(int count) {
    
    //int deg[10] = {0,-120, 30, 90, -60, 30, 75, -90, 15, 75};
    int deg[10] = {0,-120, -115, 40, 85, 90, -90, 0, 75, 80};

    return deg[count];

}