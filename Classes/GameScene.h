#pragma once
#include <stdio.h>
#include "cocos2d.h"
#include "Line.h"
#include "Monster.h"
USING_NS_CC;
using std::string;



class GameScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	CREATE_FUNC(GameScene);
	void update(float f);
	void onMouseMove(EventMouse* event);
private:
	MonsterManager manager;
};