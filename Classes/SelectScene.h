#ifndef __SELECT_SCENE_H__
#define __SELECT_SCENE_H__

#include "cocos2d.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

class SelectScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    void preLoadMusic();
    void playBgm();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void selectCallback(cocos2d::Ref* pSender, int i);
    void startCallback(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(SelectScene);
private:
    int SongNum = 2;
    bool SelectedOrNot = false;
    string SelectSongName;
};

#endif // __SELECT_SCENE_H__
