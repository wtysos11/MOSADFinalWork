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
	float countDistance(Vec2 pos)
	{
		return pos.getDistance(tower->getPosition());
	}
	int getType()
	{
		return this->type;
	}
	void changeType(int type)
	{
		this->type = type;

		if (type == 1)
		{
			tower->setTexture("tower1.png");
		}
		else if (type == 2)
		{
			tower->setTexture("tower2.png");
		}
		else if (type == 3)
		{
			tower->setTexture("tower3.png");
		}
		else if (type == 0)
		{
			tower->setTexture("TowerLand.png");
		}
	}
	bool isGround()
	{
		if (type == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	Vec2 getPosition()
	{
		return tower->getPosition();
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

	//通过遍历所有塔的位置来判断是否点击到了塔，是的话返回true，不然返回false
	bool clickTower(Vec2 pos)
	{
		for (auto iter = tower.begin(); iter != tower.end(); iter++)
		{
			if ((*iter).countDistance(pos) < 30)
			{
				return true;
			}
		}
		return false;
	}
	Tower getTowerThroughPos(Vec2 pos)
	{
		for (auto iter = tower.begin(); iter != tower.end(); iter++)
		{
			if ((*iter).countDistance(pos) < 30)
			{
				return (*iter);
			}
		}
	}
	void changeTower(Vec2 pos, int changingType)
	{
		
		for (auto iter = tower.begin(); iter != tower.end(); iter++)
		{
			if ((*iter).countDistance(pos) < 30)
			{
				(*iter).changeType(changingType);
				break;
			}
		}
		//clicking.changeSetting();
	}
	void deleteTower(Vec2 pos)
	{
		for (auto iter = tower.begin(); iter != tower.end(); iter++)
		{
			if ((*iter).countDistance(pos) < 30)
			{
				(*iter).changeType(0);
				break;
			}
		}
	}
	void updateTower(Vec2 pos)
	{
		for (auto iter = tower.begin(); iter != tower.end(); iter++)
		{
			if ((*iter).countDistance(pos) < 30)
			{
				//(*iter).changeType(0);
				break;
			}
		}
	}
};

#endif