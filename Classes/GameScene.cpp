#include "GameScene.h"
#include "ResultScene.h" 
#include "Global.h" 
#include "SimpleAudioEngine.h" 
#include <sstream>
#include "PauseScene.h"
#define GLOBAL Global::getInstance()
using std::stringstream;

USING_NS_CC;
using namespace CocosDenshion;
string MusicName = "eotm.mp3";
char ScoreName[] = "eotm.json";


enum judge { GOOD, PERFECT, MISS, END };
enum key_4 { D, F, J ,K };


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

	//  暂停键  PAUSE BUTTON
	auto pauseItem = MenuItemImage::create("main_pause.png", "main_pause_2.png", [&](Ref* Sender) {
		Director::sharedDirector()->pushScene(PauseScene::createScene());
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}); 
	pauseItem->setPosition(SCREEN.leftTop() + Vec2(25, -30));
	pauseItem->setScale(0.4);
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
	menu->addChild(pauseItem,2);
    this->addChild(menu, 1);


	/* GAME SCENE LABEL,  FOR DEBUGGING  */
    auto label = Label::createWithTTF("Game Scene", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

   // this->addChild(label, 1);


	/// set background 
	Sprite* background = Sprite::create("main_background.jpg");
	background->setPosition(SCREEN.center());
	Size mywinsize = Director::getInstance()->getWinSize();
	float winw = mywinsize.width; //获取屏幕宽度
	float winh = mywinsize.height;//获取屏幕高度
	float spx = background->getTextureRect().getMaxX();
	float spy = background->getTextureRect().getMaxY();
	background->setScaleX(winw / spx); //设置精灵宽度缩放比例
	background->setScaleY(winh / spy);
	addChild(background, 0);


	///////////////////////
	/////// USEFUL CODE //////
	///////////////////////

	// initialize
	for (int i = 0; i < Global::getInstance().TRACK_NUMBER; i++) {
		scoresOfTrack.push_back(map<int, Note>());
		spritesOfTrack.push_back(vector<Sprite*>());
	}
	makeGear();   // INITIALIZE gear
	setBoxes();
	calculateTime();  //  计算特定时间戳，用于判定
	if (Global::getInstance().restartOrNot) {

		GLOBAL.reset();
		preLoadBackGroundMusic();


	}

	GLOBAL.restartOrNot = true;

	// 读取谱面
	preLoadMusicScore();

	//preLoadAnimation();
	addListeners();

	// 毫秒计时器
	schedule(schedule_selector(GameScene::mSecCounter), 0.01f, kRepeatForever, 0.0f);
	schedule(schedule_selector(GameScene::updateScene), 0.01f, kRepeatForever, 0.0f);
    return true;
}

/*
	PREPARATION
*/
void GameScene::makeGear()
{
	gear = ui::Layout::create();
	gear->setLayoutType(ui::Layout::Type::HORIZONTAL);
	addChild(gear);

	auto tracks = Sprite::create("main_separation.png");
	tracks->setPosition(SCREEN.center());
	tracks->setScale(0.4);
	gear->addChild(tracks);

	judgeResultBox = Sprite::create();
	judgeResultBox->setPosition(SCREEN.center());
	addChild(judgeResultBox);

	hitlabel = Label::create();
	hitlabel->setPosition(SCREEN.left());
	addChild(hitlabel, 4);

	auto scorePic = Sprite::create("main_score.png");
	scorePic->setPosition(SCREEN.right()-Vec2(80,30));
	scorePic->setScale(0.5);
	addChild(scorePic, 4);
	 
	scorelabel = Label::create();
	scorelabel->setPosition(SCREEN.right() - Vec2(80, 50));
    scorelabel->setTextColor(Color4B::GRAY);
	addChild(scorelabel, 4);

	auto comboPic = Sprite::create("combo.png");
	comboPic->setPosition(SCREEN.right() + Vec2(-80, 30));
	comboPic->setScale(0.5);
	addChild(comboPic, 4);

	combolabel = Label::create();
	combolabel->setPosition(SCREEN.right() + Vec2(-80, 10));
	combolabel->setTextColor(Color4B::GRAY);
	addChild(combolabel, 4);
}
void GameScene::preLoadBackGroundMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MusicName.c_str());
	CCLog("		MUSIC BEGINS on   : %d", GLOBAL.timeCounter);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(MusicName.c_str(), true);
}


void GameScene::preLoadMusicScore() {
	string scoreString = FileUtils::getInstance()->getStringFromFile(ScoreName);
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
		for (int j = 0; j < Global::getInstance().TRACK_NUMBER; j++) {
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
	for (int j = 0; j < Global::getInstance().TRACK_NUMBER; j++) {
		isNoodleDroping.push_back(false);
	}

}
void GameScene::preLoadAnimation() {
	auto MissImage = Director::getInstance()->getTextureCache()->addImage("miss.png");

	Vector<SpriteFrame*> noteMiss;
		auto frame = SpriteFrame::createWithTexture(MissImage, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 355, 177)));
		noteMiss.pushBack(frame);

	Animation* noteMissAnimation = Animation::createWithSpriteFrames(noteMiss, 0.1f);
	
	AnimationCache::getInstance()->addAnimation(noteMissAnimation, "noteMiss");

}


void GameScene::mSecCounter(float dt) { 
	GLOBAL.timeCounter++;
}
void GameScene::calculateTime() {
	/*  算出来的都是时间偏移量 */
	BOX_TIME = this->AVG_SPEED * Global::JUDGE_BOX_HEIGHT; 
}


/*

EVENT DISPATCHER
AND HANDLERS SETTING

*/
void GameScene::addListeners()
{ 
	auto missListener = EventListenerCustom::create("onNotePassingMissBox", CC_CALLBACK_1(GameScene::onNotePassingMissBox, this));
	_eventDispatcher->addEventListenerWithFixedPriority(missListener, 1);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);


	
}


void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
		
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		judgeNoteByTrack(D, GLOBAL.timeCounter);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F:
		judgeNoteByTrack(F, GLOBAL.timeCounter);
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		judgeNoteByTrack(J, GLOBAL.timeCounter);
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		judgeNoteByTrack(K, GLOBAL.timeCounter);
		break;
	}
	log("keyRELEASED  COUNTER:  %d", GLOBAL.timeCounter);
	unschedule(schedule_selector(GameScene::holdingNoodle));
}
void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		judgeNoteByTrack(D, GLOBAL.timeCounter);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F:
		judgeNoteByTrack(F, GLOBAL.timeCounter);
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		judgeNoteByTrack(J, GLOBAL.timeCounter);
		break;
		 
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		judgeNoteByTrack(K, GLOBAL.timeCounter);
		break; 
	}

}

void GameScene::onNotePassingMissBox(EventCustom * event)
{
	for (int i = 0; i < Global::getInstance().TRACK_NUMBER; i++) {
		for (int j = 0; j < spritesOfTrack[i].size(); j++) {
			Sprite* s = spritesOfTrack[i][j]; 
			
			if (s != NULL) { 
				if (s->getTag() != END && s->getTag() - GLOBAL.timeCounter < -1.5 * BOX_TIME) {
						// miss则播放miss动画，还没想好在哪里播放
					//  这里是为了在玩家没有按下任何按键的情况下依然能够监听到miss的音符而设置的
					onJudgeResultSettled(MISS);
					s->removeFromParent();
					spritesOfTrack[i][j] = NULL;
				}

			}
				
		}
	}
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void GameScene::setBoxes() {
	// 设置判定线
	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	for (int i = 0; i < Global::getInstance().TRACK_NUMBER; i++) {
		Sprite* good_box_up = Sprite::create("box.png");
		good_box_up->setPosition(SCREEN.judgePos());
		log("good box up y %f", good_box_up->getPosition().y);
		log("good box up y %f", good_box_up->getContentSize().height);
		log("good box up getminX %f", good_box_up->getBoundingBox().getMinX());
		log("good box up getminY %f", good_box_up->getBoundingBox().getMinY());
		log("box height		%f", good_box_up->getContentSize().height);
		good_box_up->runAction(MoveBy::create(0.01f, Vec2(i*Global::TRACK_WIDTH, Global::JUDGE_BOX_HEIGHT)));
		good_box_up->setTag(Global::JUDGE_GOOD_BOX_TAG);
		//addChild(good_box_up, 1);

		LENGTH = visibleSize.height - good_box_up->getPosition().y + Global::JUDGE_BOX_HEIGHT;
		AVG_SPEED = (float)LENGTH / (float)Global::getInstance().DROP_TIME;

		Sprite* perfect_box = Sprite::create("box.png");
		perfect_box->setPosition(SCREEN.judgePos());
		perfect_box->runAction(MoveBy::create(0.01f, Vec2(i*Global::TRACK_WIDTH, 0)));
		perfect_box->setTag(Global::JUDGE_PERFECT_BOX_TAG);
		//addChild(perfect_box, 1);
		Sprite* good_box_down = Sprite::create("box.png");
		good_box_down->setPosition(SCREEN.judgePos());
		good_box_down->runAction(MoveBy::create(0.01f, Vec2(i*Global::TRACK_WIDTH, 0-Global::JUDGE_BOX_HEIGHT)));
		good_box_down->setTag(Global::JUDGE_GOOD_BOX_TAG);
		//addChild(good_box_down, 1);
		Sprite* miss_box = Sprite::create("box.png");
		miss_box->setPosition(SCREEN.judgePos());
		miss_box->runAction(MoveBy::create(0.01f, Vec2(i*Global::TRACK_WIDTH, 0-2*Global::JUDGE_BOX_HEIGHT)));
		miss_box->setTag(Global::JUDGE_MISS_BOX_TAG);
		//addChild(miss_box, 1);
		missBoxes.push_back(miss_box);
	}
	
	auto line = Sprite::create("line2.png");
	line->setPosition(SCREEN.judgePos());
	addChild(line,4);
}

/*

	GAME CONTROL		

*/


void GameScene::gameOver() {
	unschedule(schedule_selector(GameScene::updateScene));
	unschedule(schedule_selector(GameScene::mSecCounter));
	Director::getInstance()->replaceScene(ResultScene::createScene());
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}


void GameScene::updateScene(float dt)
{ 
	// 分发事件
	EventCustom e("onNotePassingMissBox");
	_eventDispatcher->dispatchEvent(&e);

	int timing = GLOBAL.timeCounter + Global::getInstance().DROP_TIME;
	//  log("timing %d", timing);
	if (timing >= timeElapsed.size() || timeElapsed[timing]) {
		// 若当前时间戳的note已被添加过，直接返回 
		return;
	}  
	timeElapsed[timing] = true;
	 
	for (int i = 0; i < Global::getInstance().TRACK_NUMBER; i++) {
		if (scoresOfTrack[i].count(timing)) {
			// if at this moment, there's a note on the track, then drop it.
			Note* newNode = &(scoresOfTrack[i][timing]);
			CCLog("		node   trackNum:%d    timeStamp:%d", newNode->trackNum, timing); 
			auto s = newNode->drop(gear);
			s->setTag(timing);  // 这一步应该是封装在note里的，不过我懒得改了 ·U·
			// 将生成的sprite添加到对应的轨道中
			spritesOfTrack[i].push_back(s);
		}
	}
	//CCLog("End Update");
}

void GameScene::judgeNoteByTrack(int trackNumber, int now)
{ 
	vector<Sprite*>& track = spritesOfTrack[trackNumber];
	//  遍历当前track上的sprite
	for (int i = 0; i < track.size(); i++) {
		if (track[i] == NULL) continue;
		int perfectTime = track[i]->getTag();
		if (perfectTime == END) continue;

		auto nt = scoresOfTrack[trackNumber][perfectTime];
		bool noodleHeadHit = isNoodleDroping[trackNumber];
		if (noodleHeadHit) {
			perfectTime += nt.length * GLOBAL.NOTE_HEIGHT / AVG_SPEED;   //  06140005: perfectTime提早了，还要往后
			isNoodleDroping[trackNumber] = false;
		}

		int offset = perfectTime - now;

		if (abs(offset) > 0.5 * BOX_TIME && abs(offset) < 1.5 * BOX_TIME) {
			onJudgeResultSettled(GOOD); 
			
			if (!noodleHeadHit && nt.type == "noodle") {
				//  击中头部
			schedule(schedule_selector(GameScene::holdingNoodle), 0.5f, kRepeatForever, 0.0f); 
				isNoodleDroping[trackNumber] = true;
			} else 
			track[i]->setTag(END);

			if (noodleHeadHit) {
				//  如果尾部hit了，就unschedule
				unschedule(schedule_selector(GameScene::holdingNoodle));
			}
		}
		else if (abs(offset) < 0.5 * BOX_TIME) {
			onJudgeResultSettled(PERFECT);
			if (!noodleHeadHit && nt.type == "noodle") {
				//  击中头部
				schedule(schedule_selector(GameScene::holdingNoodle), 0.5f, kRepeatForever, 0.0f);
				isNoodleDroping[trackNumber] = true;
			}
			else {
				// 不是noodle类型的note，或者现在正在判断noodle的尾部
				track[i]->setTag(END);
			}

			if (noodleHeadHit) {
				//  如果尾部hit了，就unschedule
				unschedule(schedule_selector(GameScene::holdingNoodle));
			}
		}
		else if (offset < -1.5 * BOX_TIME) {

			if (nt.type == "noodle") { 
				//  如果noodle的头就miss了，尾部就不用判断了
				if (noodleHeadHit) {
					//  如果尾部miss了，就unschedule
					unschedule(schedule_selector(GameScene::holdingNoodle));
				}
			}
			onJudgeResultSettled(MISS);
			track[i]->setTag(END);
			track[i]->removeFromParent();
			track[i] = NULL;
		}
	}
}

void GameScene::holdingNoodle(float dt) {
	//  实现按noodle时combo不断增加
	GLOBAL.combo++;
	{
		stringstream ss;
		ss << GLOBAL.combo;
		string str;
		ss >> str;
		combolabel->setString(str.c_str());
	}
}

void GameScene::onJudgeResultSettled(int result) {
	if (g != nullptr) {
		g->removeFromParent();
		g = NULL;
	}
	//  play animation
	//  add combo
	//  add score
	//  modify lifetime
	{ 
		stringstream ss;
		ss << result;
		string str;
		ss >> str;
		hitlabel->setString(str.c_str());
	}
	switch (result) {
	case GOOD:
		Global::getInstance().combo++;
		Global::getInstance().HP += 5;
		Global::getInstance().HP % 101;
		Global::getInstance().score += 70;
		Global::getInstance().good++; 
		playAni(GOOD);
		break;
	case PERFECT:
		Global::getInstance().combo++;
		Global::getInstance().HP += 10;
		Global::getInstance().HP % 101;
		Global::getInstance().score += 100;
		Global::getInstance().perfect++;

		playAni(PERFECT);
		break;
	case MISS:
		Global::getInstance().maxCombo = Global::getInstance().maxCombo < Global::getInstance().combo ? Global::getInstance().combo : Global::getInstance().maxCombo;
		Global::getInstance().combo = 0;
		Global::getInstance().HP -= 10;
		Global::getInstance().miss++;

		playAni(MISS);
		if (Global::getInstance().HP < 0) {
			gameOver();
			return;
		} 

		break;
	}
	{
		stringstream ss;
		ss << Global::getInstance().score;
		string str;
		ss >> str;
		scorelabel->setString(str.c_str());
	}
	{
		stringstream ss;
		ss << GLOBAL.combo;
		string str;
		ss >> str;
		combolabel->setString(str.c_str());
	}
}

void GameScene::playAni(int result) {
	if (result == GOOD)
		g = Sprite::create("good.png");
	else if (result == PERFECT)
		g = Sprite::create("perfect.png");
	else  
		g = Sprite::create("miss.png");

	this->addChild(g, 5);
	g->setPosition(SCREEN.judgePos());
	g->runAction(Sequence::create(FadeOut::create(0.3), /*CallFunc::create([&, g]() {g->removeFromParent(); }),*/ NULL));

}
