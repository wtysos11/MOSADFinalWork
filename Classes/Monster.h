#ifndef MONSTER_H
#define MONSTER_H
#include "Line.h"
#include "cocos2d.h"
using std::string;
using namespace cocos2d;
struct myProperty {
	int health;
	float speed;
	myProperty(int health, float speed)
	{
		this->health = health;
		this->speed = speed;
	}
	myProperty()
	{
		health = 0;
		speed = 1.0f;
	}
};

class Monster {
private:
	//����
	myProperty setting;
	Sprite* monster;
	myLine line;
public:
	Monster(Sprite* monster, myLine line, myProperty setting)
	{
		this->setting = setting;
		this->monster = monster;
		this->line = line;
	}
	void moveForward(float delta)
	{
		Vec2 origin = monster->getPosition();
		line.checkNext(origin);
		Vec2 point = line.getPoint();
		Vec2 aim(point.x - origin.x, point.y - origin.y);
		aim.normalize();

		float dist = setting.speed*delta;
		monster->runAction(MoveBy::create(delta, Vec2(dist*aim.x, dist*aim.y)));
	}
};

class MonsterManager {
private:
	vector<Monster> storage;
	int deltaTime;
public:
	void createMonster(string picture, Scene* scene, myLine line, myProperty setting)
	{
		Sprite* monster = Sprite::create(picture);

		Vec2 point = line.getPoint();
		line.nextPoint();//��һ����Ϊ��ʼ�㣬�ڶ����㿪ʼΪĿ���
		monster->setPosition(point.x, point.y);
		scene->addChild(monster, 5);

		storage.push_back(Monster(monster, line, setting));
	}
	void updateAll()
	{
		for (unsigned int i = 0; i < storage.size(); i++)
		{
			storage[i].moveForward(0.1f);
		}
	}
};
#endif
