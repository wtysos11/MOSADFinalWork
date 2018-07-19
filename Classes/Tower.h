#ifndef TOWER_H
#define TOWER_H

#define MY_SCALE_SIZE 0.54
using std::vector;
struct TowerProperty {
	int attack;
	float speed;
	float range;
	TowerProperty(int attack, float speed,int range)
	{
		this->attack = attack;
		this->speed = speed;
		this->range = range;
	}
	TowerProperty()
	{
		attack = 0;
		speed = 1.0f;
		range = 100.0f;
	}
};

class Tower {
private:
	//怪物
	TowerProperty setting;
	Sprite* tower;
	int type;
public:
	Tower(TowerProperty setting, Sprite* t, int type)
	{
		this->setting = setting;
		tower = t;
		this->type = type;
	}
	bool isTower(Sprite* t)
	{
		if (t == tower)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

class TowerManager {
private:
	vector<Tower> tower;
public:
	void createTower(string picture,TowerProperty setting,Vec2 pos,Scene* scene)
	{
		int type = -1;
		if (picture == "TowerLand.png")
		{
			type = 0;
		}
		else if (picture == "tower1.png")
		{
			type = 1;
		}
		else if (picture == "tower2.png")
		{
			type = 2;
		}
		else if (picture == "tower3.png")
		{
			type = 3;
		}
		Sprite* sp = Sprite::create(picture);
		sp->setPosition(pos);
		sp->setScale(MY_SCALE_SIZE);
		scene->addChild(sp, 10);
		tower.push_back(Tower(setting, sp, type));
	}
	//根据精灵指针寻找对应的对象
	Tower getTower(Sprite* t)
	{
		for (auto iter = tower.begin(); iter != tower.end(); iter++)
		{
			if ((*iter).isTower(t))
			{
				return (*iter);
			}
		}
	}
};

#endif