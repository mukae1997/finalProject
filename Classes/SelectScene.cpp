#include "SelectScene.h"
std::string SongName[2] = {"SongName1", "SongName2"};
USING_NS_CC;

Scene* SelectScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = SelectScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SelectScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(SelectScene::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

     

    // add "SelectScene" splash screen"
    auto sprite = Sprite::create("songs_background.jpg");
    sprite->setScale(visibleSize.height/sprite->getContentSize().height);
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    auto sprite_1 = Sprite::create("songs_list.png");
        sprite_1->setScale(0.4);
     //   sprite_1->setScale(visibleSize.height/sprite_1->getContentSize().height);
    // position the sprite on the center of the screen
    sprite_1->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height + origin.y-100));
    this->addChild(sprite_1, 1);
    MenuItemLabel* songSprite[SongNum];
    
    for (int i = 0; i < SongNum; i++) {
        songSprite[i] = MenuItemLabel::create(Label::createWithTTF(SongName[i], "Marker Felt.ttf", 30),CC_CALLBACK_1(SelectScene::selectCallback, this, i));
        songSprite[i]->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height-(90+i*50)));
        menu->addChild(songSprite[i]);
    }
    auto start = MenuItemImage::create("start.png", "start.png", CC_CALLBACK_1(SelectScene::startCallback, this));
        start->setScale(0.5);
    start->setPosition(Vec2(origin.x+visibleSize.width-start->getContentSize().width+20, origin.y+start->getContentSize().height/2));
    menu->addChild(start);
    return true;
}


void SelectScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void SelectScene::selectCallback(cocos2d::Ref* pSender, int i) {
    SelectedOrNot = true;
    SelectSongName = SongName[i];
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic(SelectSongName.c_str(), true);
}
void SelectScene::startCallback(cocos2d::Ref* pSender) {
    if (!SelectedOrNot) return;
    /*auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopBackgroundMusic();*/
    auto GameScene =  GameScene::createScene(SelectSongName);
    Director::getInstance()->replaceScene(GameScene);
}

void SelectScene::preLoadMusic() {
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    for (int i = 0; i < 2;i++) {
        audio->preloadBackgroundMusic(SongName[i].c_str());
    }
}
