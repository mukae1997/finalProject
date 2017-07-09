#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include "VisibleRect.h"


/*

记录游玩时的数据


*/
class Global {
public:
	// 不可设置参数
	static const int JUDGE_PERFECT_BOX_TAG = 1;
	static const int JUDGE_GOOD_BOX_TAG = 2;
	static const int JUDGE_MISS_BOX_TAG = 3;
	static const int JUDGE_BOX_HEIGHT = 15; // 判定框高度，不知道为什么要/2才能移到正确位置,好像不是按像素算的
	static const int TRACK_WIDTH = 55; // 判定框宽度 
	float NOTE_HEIGHT;  // 每个note的原始高度，单位是cocos2d神秘的单位长度

public:
	static Global& getInstance() {
		static Global global;
		return global;
	}


	int combo; // 连击数
	int maxCombo;
	int score; // 分数
	int HP; // 生命值
	int perfect, good, miss;

	int timeCounter;

	bool restartOrNot;



	// 可设置参数
	int TRACK_NUMBER = 4;
	int DROP_TIME = 100; // 音符从顶部掉落到完美判定框所需的10毫秒数， 例：若值为10，说明需要100毫秒

private:
	Global() {
		maxCombo = 0;
		combo = 0;
		perfect = good = miss = 0;
		score = 0;
		HP = 10000000;
		NOTE_HEIGHT = 48.0;
		
		timeCounter = 0;

		restartOrNot = true;
	}
public:
	void reset() {
		maxCombo = 0;
		combo = 0;
		perfect = good = miss = 0;
		score = 0;
		HP = 10000000;
		NOTE_HEIGHT = 48.0;

		timeCounter = 0;

		restartOrNot = true;
	}

};



#endif
