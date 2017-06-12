#pragma once
#ifndef ___NOTE__H___
#define ___NOTE__H___
using std::string;

#include "cocos2d.h"
#include "VisibleRect.h"
#include "Global.h"
USING_NS_CC;

class Note{

	public:
	int length;
	string type;
	int trackNum;
	Note(){
	}
	Note(string t, int l, int n) {
		type = t;
		length = l;
		trackNum = n;
	}
	Note(const Note& other) {
		this->length = other.length;
		this->type = other.type;
		this->trackNum = other.trackNum;
	}
	Sprite* drop(Node* parent);
	void operator=(const Note&);
private:
	VisibleRect SCREEN; // 用来调整各种位置
};


#endif // !___NOTE__H___
