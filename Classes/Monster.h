#ifndef MONSTER_H
#define MONSTER_H
#include "Line.h"
#include "cocos2d.h"
using std::string;
using namespace cocos2d;
struct monsterProperty {
	int health;
	float speed;
	monsterProperty(int health, float speed)
	{
		this->health = health;
		this->speed = speed;
	}
	monsterProperty()
	{
		health = 0;
		speed = 1.0f;
	}
};

class Monster {
private:
	//怪物
	monsterProperty setting;
	Sprite* monster;
	myLine line;
	int type;
public:
	bool isActive;
	Monster(Sprite* monster, myLine line, monsterProperty setting,int type)
	{
		this->setting = setting;
		this->monster = monster;
		this->line = line;
		this->type = type;
		isActive = true;
	}
	//当到达终点时返回false
	bool moveForward(float delta)
	{
		Vec2 origin = monster->getPosition();
		if (line.checkEnd(origin))
		{
			return false;
		}

		line.checkNext(origin);
		Vec2 point = line.getPoint();
		Vec2 aim(point.x - origin.x, point.y - origin.y);
		aim.normalize();

		float dist = setting.speed*delta;
		monster->runAction(MoveBy::create(delta, Vec2(dist*aim.x, dist*aim.y)));
		return true;
	}
	int getType()
	{
		return type;
	}
	Sprite* getSprite()
	{
		return monster;
	}
};

class MonsterManager {
private:
	vector<Monster> storage;
	int deltaTime;
public:
	void createMonster(string picture, Scene* scene, myLine line, monsterProperty setting)
	{
		Sprite* monster = Sprite::createWithSpriteFrameName(picture);

		Vec2 point = line.getPoint();
		line.nextPoint();//第一个点为起始点，第二个点开始为目标点
		monster->setPosition(point.x, point.y);
		scene->addChild(monster, 5);

		int type;
		if (picture == "enemy1_0.png")
		{
			monster->stopAllActions();
			Animate* enemy1Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1Animation"));
			monster->runAction(RepeatForever::create(enemy1Animation));
			type = 1;
		}
		else if (picture == "enemy2_0.png")
		{
			monster->stopAllActions();
			Animate* enemy2Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2Animation"));
			monster->runAction(RepeatForever::create(enemy2Animation));
			type = 2;
		}
		else if (picture == "enemy3_0.png")
		{
			monster->stopAllActions();
			Animate* enemy3Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3Animation"));
			monster->runAction(RepeatForever::create(enemy3Animation));
			type = 3;
		}

		storage.push_back(Monster(monster, line, setting,type));
	}
	void updateAll()
	{
		for (auto iter = storage.begin();iter!=storage.end();)
		{
			bool end = false;
			if ((*iter).isActive)
			{
				bool ending = (*iter).moveForward(0.1f);
				if (!ending)
				{
					//播放死亡动画，并在死亡后消灭它
					monsterAnimate(*iter, "dead");
					(*iter).isActive = false;
					iter = storage.erase(iter);
					end = true;
				}
			}
			if (!end)
			{
				iter++;
			}

		}
	}
	void monsterAnimate(Monster monster,string behaviour)
	{
		if (!monster.isActive)
			return;

		Sprite* enemy = monster.getSprite();
		if (behaviour == "dead")
		{
			monster.isActive = false;
			if (monster.getType() == 1)
			{
				enemy->stopAllActions();
				Animate* enemy1DeadAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1DeadAnimation"));
				enemy->runAction(Sequence::create(enemy1DeadAnimation, CCDelayTime::create(1), CallFunc::create([enemy]() {
					enemy->removeFromParentAndCleanup(false);
				}), nullptr));
			}
			else if (monster.getType() == 2)
			{
				enemy->stopAllActions();
				Animate* enemy2DeadAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2DeadAnimation"));
				enemy->runAction(Sequence::create(enemy2DeadAnimation, CCDelayTime::create(1), CallFunc::create([enemy]() {
					enemy->removeFromParentAndCleanup(false);
				}), nullptr));
			}
			else if (monster.getType() == 3)
			{
				enemy->stopAllActions();
				Animate* enemy3DeadAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3DeadAnimation")); 
				enemy->runAction(Sequence::create(enemy3DeadAnimation, CCDelayTime::create(1), CallFunc::create([enemy]() {
					enemy->removeFromParentAndCleanup(false);
				}), nullptr));
			}
		}
	}

};
#endif
