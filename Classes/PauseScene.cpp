#include "PauseScene.h"
#include "GameScene.h"
#include "Global.h" 

#define GLOBAL Global::getInstance()
#include "SimpleAudioEngine.h" 
USING_NS_CC;
using namespace CocosDenshion;

Scene* PauseScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = PauseScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PauseScene::init()
{
	auto timeCounter_backup = GLOBAL.timeCounter;
	auto maxCombo_backup = GLOBAL.maxCombo;
	auto perfect_backup = GLOBAL.perfect;
	auto score_backup = GLOBAL.score;
	auto HP_backup = GLOBAL.HP;
	auto NOTE_backup = GLOBAL.NOTE_HEIGHT;

	GLOBAL.combo = 0;
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	// create background..
	auto bg = Sprite::create("main_background.jpg");
	bg->setScale((visibleSize.height) / (bg->getContentSize().height));
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(bg, 0);

	// create restart button
	auto restartItem = MenuItemImage::create("restart.png", "restart_2.png", [=](Ref* Sender) {
		GLOBAL.restartOrNot = true;
		Director::sharedDirector()->pushScene(GameScene::createScene());
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	});
	restartItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100, origin.y + visibleSize.height/2));
	restartItem->setScale(visibleSize.height / bg->getContentSize().height);

	// create continue button
	auto continueItem = MenuItemImage::create("continue.png", "continue_2.png", [=](Ref* Sender) {
		GLOBAL.restartOrNot = false;
		Director::sharedDirector()->pushScene(GameScene::createScene());
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		GLOBAL.timeCounter = timeCounter_backup;
		GLOBAL.maxCombo = maxCombo_backup;
		GLOBAL.perfect = perfect_backup;
		GLOBAL.score = score_backup;
		GLOBAL.HP = HP_backup;
		GLOBAL.NOTE_HEIGHT = NOTE_backup;
	});
	continueItem->setPosition(Vec2(origin.x + visibleSize.width / 2 + 100, origin.y + visibleSize.height / 2));
	continueItem->setScale(visibleSize.height / bg->getContentSize().height);

	// create menu, it's an autorelease object
	auto menu = Menu::create(restartItem, continueItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("ResultScene", "fonts/Marker Felt.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
	 

    return true;
}


void PauseScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
