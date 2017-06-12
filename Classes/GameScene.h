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

	// PREPARATION
	void setBoxes();
	void preLoadMusicScore();
	void preLoadAnimation();
	void preLoadBackGroundMusic();
	void addListeners();

	///// GAME CONTROL
	void gameOver();
	void updateScene(float dt);
	void mSecCounter(float dt);
	void calculateTime();
	void judgeNoteByTrack(int trackNumber, int now);
	void onJudgeResultSettled(int result);

	// MISS NOTE
	void onNotePassingMissBox(EventCustom* event);
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);

private: 

	VisibleRect SCREEN; // 用来调整各种位置
	int LENGTH;  // 屏幕顶端到判定区顶部的距离
	int AVG_SPEED;  // LENGTH / DROP_TIME
	int BOX_TIME;  // 跨过一个判定区需要的时间 

	Layout* gear;
	Sprite* judgeResultBox;
	Label* hitlabel; // debug
	Label* scorelabel;
	vector<Sprite*> missBoxes;	//  boxes

	vector<map<int, Note> > scoresOfTrack; // 每个轨道上的音符信息
	vector<vector<Sprite*> > spritesOfTrack; // 每个轨道上已生成的note集合


	int timeCounter; // 记录已流逝的10毫秒数 
	vector<bool> timeElapsed; // 记录已流逝的时间，用于判断某时间戳的note是否已被添加过
	vector<bool> isNoodleDroping; // 记录每个track当前是否有noodle经过

};


#endif // __GAME_SCENE_H__
