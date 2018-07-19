#ifndef TOWER_H
#define TOWER_H

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
	//¹ÖÎï
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
};

#endif