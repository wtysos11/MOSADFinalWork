#include "GameScene.h"
#include "Line.h"
USING_NS_CC;

Scene* GameScene::createScene()
{
	return GameScene::create();
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto listener = EventListenerMouse::create();
	listener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);
	//draw image
	auto background = Sprite::create("go_stage07-1_01.png");
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);   //设置位置  
	background->setAnchorPoint(Point(0.5, 0.5));    //设置锚点  
													//根据缩放设置实际大小  
	Size visibleSize_back = background->getContentSize();  //获取精灵大小(设置图片的时候，精灵大小为图片大小)  
														   //根据实际宽度和总宽度，计算期望缩放比率  
	float scaleX = (float)visibleSize.width / (float)visibleSize_back.width;
	float scaleY = (float)visibleSize.height / (float)visibleSize_back.height;
	background->setScale(scaleX, scaleY);    //缩放  
	this->addChild(background, 1);

	//线路1，开头为左侧8点钟方向，主干
	auto checkpointa1 = Sprite::create("go_towers-2_2.png");
	checkpointa1->setPosition(0, 165);
	this->addChild(checkpointa1, 2);

	auto checkpointa2 = Sprite::create("go_towers-2_2.png");
	checkpointa2->setPosition(100, 194);
	this->addChild(checkpointa2, 2);

	auto checkpointa3 = Sprite::create("go_towers-2_2.png");
	checkpointa3->setPosition(200, 259);
	this->addChild(checkpointa3, 2);

	auto checkpointa3a = Sprite::create("go_towers-2_2.png");
	checkpointa3a->setPosition(243, 277);
	this->addChild(checkpointa3a, 2);

	auto checkpointa4 = Sprite::create("go_towers-2_2.png");
	checkpointa4->setPosition(300, 253);
	this->addChild(checkpointa4, 2);

	auto checkpointa4a = Sprite::create("go_towers-2_2.png");
	checkpointa4a->setPosition(374, 308);
	this->addChild(checkpointa4a, 2);

	auto checkpointa5 = Sprite::create("go_towers-2_2.png");
	checkpointa5->setPosition(400, 300);
	this->addChild(checkpointa5, 2);

	auto checkpointa6 = Sprite::create("go_towers-2_2.png");
	checkpointa6->setPosition(500, 270);
	this->addChild(checkpointa6, 2);

	auto checkpointa7 = Sprite::create("go_towers-2_2.png");
	checkpointa7->setPosition(548, 164);
	this->addChild(checkpointa7, 2);

	//线路2，左侧10点钟方向，与线路1合并后结束
	auto checkpointb1 = Sprite::create("go_towers-2_2.png");
	checkpointb1->setPosition(0, 463);
	this->addChild(checkpointb1, 2);

	auto checkpointb2 = Sprite::create("go_towers-2_2.png");
	checkpointb2->setPosition(100, 390);
	this->addChild(checkpointb2, 2);

	auto checkpointb3 = Sprite::create("go_towers-2_2.png");
	checkpointb3->setPosition(200, 410);
	this->addChild(checkpointb3, 2);

	auto checkpointb4 = Sprite::create("go_towers-2_2.png");
	checkpointb4->setPosition(300, 400);
	this->addChild(checkpointb4, 2);

	//线路3，12点钟
	auto checkpointc1 = Sprite::create("go_towers-2_2.png");
	checkpointc1->setPosition(567, 636);
	this->addChild(checkpointc1, 2);

	auto checkpointc2 = Sprite::create("go_towers-2_2.png");
	checkpointc2->setPosition(533, 503);
	this->addChild(checkpointc2, 2);

	auto checkpointc3 = Sprite::create("go_towers-2_2.png");
	checkpointc3->setPosition(567, 403);
	this->addChild(checkpointc3, 2);

	auto checkpointc4 = Sprite::create("go_towers-2_2.png");
	checkpointc4->setPosition(628, 329);
	this->addChild(checkpointc4, 2);

	auto checkpointc5 = Sprite::create("go_towers-2_2.png");
	checkpointc5->setPosition(691, 267);
	this->addChild(checkpointc5, 2);

	auto checkpointc6 = Sprite::create("go_towers-2_2.png");
	checkpointc6->setPosition(669, 176);
	this->addChild(checkpointc6, 2);

	//线路4,3点钟方向
	auto checkpointd1 = Sprite::create("go_towers-2_2.png");
	checkpointd1->setPosition(947, 372);
	this->addChild(checkpointd1, 2);

	auto checkpointd2 = Sprite::create("go_towers-2_2.png");
	checkpointd2->setPosition(813, 365);
	this->addChild(checkpointd2, 2);
	
	myLine line;
	line.addPoint(0, 463);
	line.addPoint(100, 390);
	line.addPoint(200, 410);
	line.addPoint(300, 400);
	manager.createMonster("go_towers-2_02.png",this,line,myProperty(100,50));

	schedule(schedule_selector(GameScene::update), 0.1f, kRepeatForever, 0);
	return true;
}

void GameScene::update(float f)
{
	manager.updateAll();
}

void GameScene::onMouseMove(EventMouse* event)
{
	Vec2 x = event->getLocationInView();
	CCLOG("%f,%f", x.x,x.y);

	x = event->getLocation();
	x = event->getLocationInView();
	x = event->getPreviousLocation();
	x = event->getPreviousLocationInView();
	x = event->getStartLocation();
	x = event->getStartLocationInView();
	x = event->getDelta();
}