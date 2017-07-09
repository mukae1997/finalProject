#include "Note.h"
#define GLOBAL Global::getInstance()
Sprite* Note::drop(Node* parent) {  
	Sprite* noteSprite;
	switch (trackNum+1) {
	case 1:
		noteSprite = Sprite::create("note1.png");
		break;
	case 2:
		noteSprite = Sprite::create("note2.png");
		break;
	case 3:
		noteSprite = Sprite::create("note3.png");
		break;
	case 4:
		noteSprite = Sprite::create("note4.png");
		break;

	}
	noteSprite->setScale(0.7);

	
	if (type == "noodle") noteSprite->setScaleY(length);
	GLOBAL.NOTE_HEIGHT = noteSprite->getContentSize().height;
	log("note 原始length:  %f", noteSprite->getContentSize().height);
	GLOBAL.NOTE_HEIGHT = noteSprite->getContentSize().height;
	noteSprite->setPosition(SCREEN.trackBegin());
	parent->addChild(noteSprite);
	noteSprite->runAction(MoveBy::create(0.0001f, Vec2((trackNum) * Global::TRACK_WIDTH, 0))); // 移到正确的轨道上
	double xCoordinate = noteSprite->getPosition().x;
	int tn = trackNum;
	Sequence* seq = Sequence::create(MoveTo::create(GLOBAL.DROP_TIME / 100,Vec2(xCoordinate, SCREEN.judgePos().y)),
		MoveTo::create(0.3f, Vec2(xCoordinate+trackNum * Global::TRACK_WIDTH, 0)), 
		FadeOut::create(0.3f), NULL);
	noteSprite->runAction(seq); 
	return noteSprite;
}

void Note::operator=(const Note & other)
{
	this->length = other.length;
	this->trackNum = other.trackNum;
	this->type = other.type;
	
}
