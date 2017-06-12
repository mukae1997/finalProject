#include "Note.h"

Sprite* Note::drop(Node* parent) { 
	auto noteSprite = Sprite::create("note.png");
	noteSprite->setPosition(SCREEN.top());
	parent->addChild(noteSprite);
	noteSprite->runAction(MoveBy::create(0.0001f, Vec2(trackNum * Global::TRACK_WIDTH, 0))); // 移到正确的轨道上
	double xCoordinate = noteSprite->getPosition().x;
	int tn = trackNum;
	Sequence* seq = Sequence::create(MoveTo::create(Global::DROP_TIME / 100,Vec2(xCoordinate, SCREEN.judgePos().y)),
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
