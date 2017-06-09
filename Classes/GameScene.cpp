#include "GameScene.h"
#include "ResultScene.h" 
#include "Global.h" 
#include "SimpleAudioEngine.h"
#include <CCFileUtils.h>
USING_NS_CC;
using namespace CocosDenshion;
string MusicName = "End of the moonlight.mp3"; 
Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	CCLog("sharedFileUtils()->getWriteablePath():  %s", CCFileUtils::sharedFileUtils()->getWritablePath());
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	/*  drafting  */
    auto label = Label::createWithTTF("Game Scene", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    this->addChild(label, 1);

	Sprite* background = Sprite::create("gameSceneBg.jpg");
	background->setPosition(SCREEN.center());
	addChild(background, 0);

	///////////////////////
	/////// USEFUL CODE //////
	///////////////////////

	// initialize
	timeCounter = 0; // clear the time counter
	for (int i = 0; i < Global::TRACK_NUMBER; i++) {
		scoresOfTrack.push_back(map<int, Note>());
	}
	makeGear();   // INITIALIZE gear


	 // 读取谱面
	preLoadMusicScore();

	preLoadBackGroundMusic();

	// 毫秒计时器
	schedule(schedule_selector(GameScene::mSecCounter), 0.01f, kRepeatForever, 0.0f);
	schedule(schedule_selector(GameScene::updateScene), 0.01f, kRepeatForever, 0.0f);
	

    return true;
}
void GameScene::preLoadBackGroundMusic()
{

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MusicName.c_str());
	CCLog("		MUSIC BEGINS on   : %d", timeCounter);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(MusicName.c_str(), true);
}
void GameScene::mSecCounter(float dt) { 
	timeCounter++; 
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameScene::makeGear()
{
	gear = ui::Layout::create();
	gear->setLayoutType(ui::Layout::Type::HORIZONTAL);
	addChild(gear);
	setBoxes();
}
void GameScene::setBoxes() {
	// 设置判定线
	for (int i = 0; i < Global::TRACK_NUMBER; i++) {
		Sprite* box = Sprite::create("box.png");
		box->setScale(0.2);
		box->setPosition(SCREEN.judgePos());
		box->runAction(MoveBy::create(0.01f, Vec2(i*Global::TRACK_WIDTH, 0)));
		gear->addChild(box, 1);
	}

}
void GameScene::gameOver() {
	unschedule(schedule_selector(GameScene::updateScene));
	Director::getInstance()->replaceScene(ResultScene::createScene());
}
void GameScene::preLoadMusicScore() {
	string scoreString = FileUtils::getInstance()->getStringFromFile("eotm.json");
	rapidjson::Document d;
	d.Parse<0>(scoreString.c_str());

	if (d.HasParseError()) {
		CCLOG("GetParseError %s\n", d.GetParseError());
		return;
	}
	rapidjson::Value& scoreArray = d["array"];
	int stamp;
	for (int i = 0; i < scoreArray.Size(); i++) {
		// read in the "i-th" record, which contains notes info. when time="stamp"
		stamp = scoreArray[i]["time"].GetInt();
		rapidjson::Value& tracks = scoreArray[i]["tracks"];
		for (int j = 0; j < Global::TRACK_NUMBER; j++) {
			//  add a new note for the "j-th" track.
			string type = tracks[j]["type"].GetString();
			if (type != "none") {
				Note newNote(type, tracks[j]["length"].GetInt(), tracks[j]["num"].GetInt());
				std::pair<int, Note> newPair(stamp, newNote);
				scoresOfTrack.at(j).insert(newPair);
			}
				
		} 
	}

	for (int j = 0; j < stamp; j++) {
		timeElapsed.push_back(false);
	}
	 

}

void GameScene::updateScene(float dt)
{

	int timing = timeCounter;
	//CCLog("		timeCounter on 159  : %d", timing);
	if (timing >= timeElapsed.size() || timeElapsed[timing]) {
		// 若当前时间戳的note已被添加过，直接返回 
		return;
	} 
	//CCLog("Begin Update");
	timeElapsed[timing] = true;

	//CCLog("		timeCounter on 167  : %d", timeCounter);
	for (int i = 0; i < Global::TRACK_NUMBER; i++) {
		if (scoresOfTrack[i].count(timing)) {
			// if at this moment, there's a note on the track
			Note* newNode = &(scoresOfTrack[i][timing]);
			//CCLog("		node   trackNum:%d    timeStamp:%d", newNode->trackNum, timing); 
			newNode->drop(gear);
		}
	}
	//CCLog("End Update");
}
