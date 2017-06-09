#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include <ui/UILayout.h>
#include <vector>
#include <string>
#include "cocos2d.h"
#include "Note.h"
#include "VisibleRect.h"

#include <map>
using std::vector;
using std::string;
using std::map;
using namespace cocos2d::ui;
 
class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);


	///// UI
	void makeGear();
	// �����ж���
	void setBoxes();
	// ����
	void preLoadMusicScore();
	void preLoadBackGroundMusic();

	///// GAME CONTROL
	void gameOver();
	void updateScene(float dt);
	void GameScene::mSecCounter(float dt);
private: 

	Layout* gear;
	vector<map<int, Note> > scoresOfTrack;

	int timeCounter; // 记录已流逝的毫秒数

	VisibleRect SCREEN; // 用来调整各种位置

	vector<bool> timeElapsed; // 记录已流逝的时间，用于判断某时间戳的note是否已被添加过
	 
};

#endif // __GAME_SCENE_H__
