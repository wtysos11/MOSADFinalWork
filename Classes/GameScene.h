#pragma once
#include <stdio.h>
#include "cocos2d.h"
#include "Line.h"
#include "Monster.h"
#include "Tower.h"
USING_NS_CC;
using std::string;



class GameScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	CREATE_FUNC(GameScene);
	void update(float f);
	void attackMenuCallback(Ref * pSender);
	void deadMenuCallback(Ref * pSender);
	void walkMenuCallback(Ref * pSender);
	void quitCallback(Ref * pSender);
	void onMouseMove(EventMouse* event);

	void addTower1(Object* pSender);
	void addTower2(Object* pSender);
	void addTower3(Object* pSender);
	bool onTouchBegan(Touch *touch, Event* event);
	void deleteTower(Object* pSender);
	void updateTower(Object* pSender);
	void bullet(float f);
	void hitByBullet();
private:
	MonsterManager monsterManager;
	TowerManager towerManager;
	Sprite * healthBar;
	Sprite * enemy1;
	Sprite * enemy2;
	Sprite * enemy3;
	int towerLandsNum;
	std::vector<Sprite*> towerLands;
	std::vector<Vec2> towerLandsPositions;

	//创建塔相关
	int clickItemtype;//-1为空，其余与塔号看齐
	Sprite* readyItem = NULL;

	//悬浮菜单
	Menu* towerMenu;
	Vec2 menuPos = Vec2::ZERO;
	//点击相关
	Vec2 prevPos;
	bool isQuit = false;

	//子弹
	std::vector<Sprite*> bullets;
	std::vector<int> bulletAttack;
	bool judgingBullets = false;
};