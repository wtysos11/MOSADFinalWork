#include "GameScene.h"
#include "MenuScene.h"
#include "Line.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
// 因为图片素材是用在1920*1080分辨率窗口里的，而本游戏窗口大小为1280*920，故要设置scale
#define MY_SCALE_SIZE 0.54
Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
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

	isQuit = false;
	isGameOver = false;
	float myXPosition = visibleSize.width / 2 + origin.x;
	float myYPosition = visibleSize.height / 2 + origin.y;

	//调试用，显示鼠标坐标
	auto listener = EventListenerMouse::create();
	listener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

	// 背景音乐
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("sounds/Jungle_Battle.mp3");
	audio->playBackgroundMusic("sounds/Jungle_Battle.mp3", true);

	// 背景图片
	auto gameBg = Sprite::create("GameBg.png");
	gameBg->setScale(MY_SCALE_SIZE);
	gameBg->setPosition(Vec2(myXPosition, myYPosition));
	this->addChild(gameBg, 0);

	// 血条后面那个框框
	auto healthBarContaier = Sprite::create("HealthBar1.png");
	healthBarContaier->setScale(1, 0.7);
	healthBarContaier->setPosition(Vec2(myXPosition, myYPosition + 320));
	this->addChild(healthBarContaier, 10);

	// 血条（图腾的血量，图腾血量为0就游戏结束）
	auto sp = Sprite::create("HealthBar2.png");
	healthBar = ProgressTimer::create(sp);
	healthBar->setType(ProgressTimerType::BAR);
	healthBar->setBarChangeRate(Point(1, 0));
	healthBar->setMidpoint(Point(0, 0));
	healthBar->setPercentage(100);
	healthBar->setPosition(Vec2(myXPosition, myYPosition + 313));
	this->addChild(healthBar, 9);
	monsterManager.registerHealthBar(healthBar);
	/*
	healthBar = Sprite::create("HealthBar2.png");
	healthBar->setScale(1, 0.5);
	healthBar->setPosition(Vec2(myXPosition, myYPosition + 313));
	this->addChild(healthBar, 9);*/


	// 图腾
	/*
	auto totem = Sprite::create("Totem.png");
	totem->setScale(MY_SCALE_SIZE);
	totem->setPosition(Vec2(myXPosition, myYPosition - 300));
	this->addChild(totem, 2);
	*/
	towerManager.createTower("Totem.png", TowerProperty(10, 300, 800,20), Vec2(myXPosition, myYPosition - 300),this);

	// 图腾子弹
		/*
	auto totemBullet = Sprite::create("TotemBullet.png");
	totemBullet->setScale(MY_SCALE_SIZE);
	// 这是图腾子弹射击的起始位置
	totemBullet->setPosition(Vec2(myXPosition + 16, myYPosition - 258));
	this->addChild(totemBullet, 3);*/

	//爆炸动画
	auto texture = Director::getInstance()->getTextureCache()->addImage("explosion.png");
	explore.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(190 * (i % 5), 200 * (i >= 5), 190, 200)));
		explore.pushBack(frame);
	}

	/*
	// 塔位数量
	towerLandsNum = 12;

	// 各个塔位的位置
	// 每个绿色地为一块，下面的塔位位置的顺序是从左到右
	// 左上块
	towerLandsPositions.push_back(Vec2(myXPosition - 530, myYPosition + 165));	// 0
	towerLandsPositions.push_back(Vec2(myXPosition - 210, myYPosition + 150));	// 1
	towerLandsPositions.push_back(Vec2(myXPosition - 10, myYPosition + 230));	// 2

	towerLandsPositions.push_back(Vec2(myXPosition + 55, myYPosition + 20));	// 3
	towerLandsPositions.push_back(Vec2(myXPosition + 180, myYPosition - 100));	// 4

																				// 左块
	towerLandsPositions.push_back(Vec2(myXPosition - 260, myYPosition + 10));	// 5

																				// 左下块
	towerLandsPositions.push_back(Vec2(myXPosition - 320, myYPosition - 120));	// 6
	towerLandsPositions.push_back(Vec2(myXPosition - 100, myYPosition - 100));	// 7
	towerLandsPositions.push_back(Vec2(myXPosition - 30, myYPosition - 150));	// 8

																				// 右上块
	towerLandsPositions.push_back(Vec2(myXPosition + 180, myYPosition + 180));	// 9
	towerLandsPositions.push_back(Vec2(myXPosition + 310, myYPosition + 60));	// 10

																				// 右下块
	towerLandsPositions.push_back(Vec2(myXPosition + 390, myYPosition - 95));	// 11

																				// 放置塔位
	for (int i = 0; i < towerLandsNum; i++)
	{
		auto towerLand = Sprite::create("TowerLand.png");
		towerLand->setScale(MY_SCALE_SIZE);
		towerLand->setPosition(towerLandsPositions[i]);
		// 放到towerLands里，以便后面处理，可根据需要修改
		towerLands.push_back(towerLand);
		this->addChild(towerLand, 1);
	}
	// 塔和对应的子弹，注意各个塔的scale值和位置偏移量如下设置，不要设置其他值（示例）
	auto tower1 = Sprite::create("tower1.png");
	tower1->setScale(0.7);
	// 各个塔位的位置的索引见上面的注释。塔的位置要加上一个偏移量如Vec2(-2, 20)。
	tower1->setPosition(towerLandsPositions[4] + Vec2(-2, 16));
	this->addChild(tower1, 2);
	// 子弹
	auto tower1Bullet = Sprite::create("tower1Bullet.png");
	tower1Bullet->setScale(0.8);
	tower1Bullet->setPosition(towerLandsPositions[4] + Vec2(-2, 50));
	this->addChild(tower1Bullet, 3);

	auto tower2 = Sprite::create("tower2.png");
	tower2->setScale(0.8);
	tower2->setPosition(towerLandsPositions[8] + Vec2(-4, 20));
	this->addChild(tower2, 2);
	// 子弹
	auto tower2Bullet = Sprite::create("tower2Bullet.png");
	tower2Bullet->setScale(0.5);
	tower2Bullet->setPosition(towerLandsPositions[8] + Vec2(-4, 60));
	this->addChild(tower2Bullet, 3);

	auto tower3 = Sprite::create("tower3.png");
	tower3->setScale(0.8);
	tower3->setPosition(towerLandsPositions[3] + Vec2(0, 4));
	this->addChild(tower3, 2);
	// 子弹
	auto tower3Bullet = Sprite::create("tower3Bullet.png");
	tower3Bullet->setScale(0.8);
	tower3Bullet->setPosition(towerLandsPositions[3] + Vec2(-4, 20));
	this->addChild(tower3Bullet, 3);
	*/
	//塔选择菜单
	Sprite* chooseBackground = Sprite::create("label.jpg");
	chooseBackground->setAnchorPoint(Vec2(0, 0));
	chooseBackground->setPosition(Vec2(visibleSize.width-150, -200));
	this->addChild(chooseBackground, 2);

	readyItem = NULL;
	towerMenu = NULL;
	clickItemtype = -1;
	prevPos = Vec2(0, 0);

	auto tower1 = MenuItemImage::create(
		"tower1.png", 
		"tower1.png", 
		CC_CALLBACK_1(GameScene::addTower1, this));
	tower1->setScale(0.7);
	tower1->setPosition(Vec2(visibleSize.width - 100, 150));

	auto tower2 = MenuItemImage::create(
		"tower2.png",
		"tower2.png",
		CC_CALLBACK_1(GameScene::addTower2, this));
	tower2->setScale(0.8);
	tower2->setPosition(Vec2(visibleSize.width - 100, 250));

	auto tower3 = MenuItemImage::create(
		"tower3.png",
		"tower3.png",
		CC_CALLBACK_1(GameScene::addTower3, this));
	tower3->setScale(0.8);
	tower3->setPosition(Vec2(visibleSize.width - 100, 330));

	auto menu = Menu::create(tower1, tower2, tower3, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 5);
	//将空地视为特殊的塔，建塔操作为在空地内部的转换
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 530, myYPosition + 165), this);	// 0
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 210, myYPosition + 150), this);	// 1
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 10, myYPosition + 230), this);	// 2

	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 55, myYPosition + 20), this);	// 3
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 180, myYPosition - 100), this);	// 4

																				// 左块
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 260, myYPosition + 10), this);	// 5

																				// 左下块
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 320, myYPosition - 120), this);	// 6
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 100, myYPosition - 100), this);	// 7
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 30, myYPosition - 150), this);	// 8

																				// 右上块
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 180, myYPosition + 180), this);	// 9
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 310, myYPosition + 60), this);	// 10

																				// 右下块
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 390, myYPosition - 95), this);	// 11
	// 返回主菜单按钮
	auto quitButton = MenuItemImage::create(
		"QuitNormal.png",
		"QuitSelected.png",
		CC_CALLBACK_1(GameScene::quitCallback, this)
	);
	quitButton->setScale(1);
	quitButton->setPosition(Vec2(myXPosition + 500, myYPosition - 300));
	auto quitButtonMenu = Menu::create(quitButton, NULL);
	quitButtonMenu->setPosition(Vec2::ZERO);
	this->addChild(quitButtonMenu, 10);
	//line1
	line1.addPoint(0, 189);
	line1.addPoint(100, 212);
	line1.addPoint(200, 259);
	line1.addPoint(243, 277);
	line1.addPoint(300, 300);
	line1.addPoint(374, 308);
	line1.addPoint(400, 300);
	line1.addPoint(500, 339);
	line1.addPoint(600, 318);
	line1.addPoint(700, 245);
	line1.addPoint(735, 183);
	line1.addPoint(myXPosition, myYPosition - 300);
	//line2;
	line2.addPoint(0, 493);
	line2.addPoint(100, 441);
	line2.addPoint(200, 450);
	line2.addPoint(300, 460);
	line2.addPoint(400, 432);
	line2.addPoint(500, 339);
	line2.addPoint(600, 318);
	line2.addPoint(700, 245);
	line2.addPoint(735, 183);
	line2.addPoint(myXPosition, myYPosition - 300);
	//line3;
	line3.addPoint(757, 698);
	line3.addPoint(732, 619);
	line3.addPoint(737, 495);
	line3.addPoint(801, 409);
	line3.addPoint(916, 303);
	line3.addPoint(916, 199);
	line3.addPoint(735, 183);
	line3.addPoint(myXPosition, myYPosition - 300);
	//line4;
	line4.addPoint(1244, 443);
	line4.addPoint(1077, 436);
	line4.addPoint(916, 303);
	line4.addPoint(916, 199);
	line4.addPoint(735, 183);
	line4.addPoint(myXPosition, myYPosition - 300);

	//monsterManager.createMonster("enemy3_0.png",this, line3, monsterProperty(100,50));
	//monsterManager.createMonster("enemy3_0.png", this, line3, monsterProperty(100, 50));
	//monsterManager.createMonster("enemy3_0.png", this, line3, monsterProperty(100, 50));
	//monsterManager.createMonster("enemy1_0.png", this, line4, monsterProperty(100, 50));

	//创建第一波怪兽
	rate = 0;
	createMonster(rate);
	is_create = true;
	ct = 0;

	//显示金币跟波数
	rateNum = Label::createWithTTF("Rate: 1/10", "fonts/arial.TTF", 40);
	rateNum->setColor(Color3B(255, 255, 255));
	rateNum->setPosition(visibleSize.width - 150, visibleSize.height - 50);
	this->addChild(rateNum, 3);
	moneyNum = Label::createWithTTF("Money: 400", "fonts/arial.TTF", 40);
	moneyNum->setColor(Color3B(255, 255, 255));
	moneyNum->setPosition(visibleSize.width - 165, visibleSize.height - 100);
	this->addChild(moneyNum, 3);

	//调度器
	schedule(schedule_selector(GameScene::update), 0.1f, kRepeatForever, 0);
	schedule(schedule_selector(GameScene::bullet), 0.1f, kRepeatForever, 0);
	//事件处理器
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, gameBg);
	return true;
}

void GameScene::update(float f)
{
	if (isGameOver)
		return;

	if (monsterManager.gameLose())
	{
		gameLose();
	}



	monsterManager.updateAll();
	hitByBullet();

	if (is_create) {
		ct++;
		int speed = 10 + 10 * ((rate + 1) / 2);
		int dalaytime = 800 / speed;
		int temp = ct / dalaytime;
		if (temp >= (3 + rate / 2)) {
			is_create = false;
			ct = 0;
		}
		else if (ct % dalaytime == 0) createMonster(rate);
	}
	else {	//判断是生成一波新怪兽还是游戏结束
		if (monsterManager.isEmpty()) {
			if (rate > 9) {
				gameWin();
			}
			else {
				is_create = true;
				rate++;
			}
		}
	}
}

void GameScene::onMouseMove(EventMouse* event)
{
	Vec2 x = event->getLocationInView();
	//CCLOG("%f,%f", x.x,x.y);
	if (readyItem != NULL && !isQuit)
	{
		readyItem->setPosition(x.x, x.y);
	}
	x = event->getLocation();
	x = event->getLocationInView();
	x = event->getPreviousLocation();
	x = event->getPreviousLocationInView();
	x = event->getStartLocation();
	x = event->getStartLocationInView();
	x = event->getDelta();
}

void GameScene::attackMenuCallback(Ref* pSender) {
	enemy1->stopAllActions();
	Animate* enemy1AttackAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1AttackAnimation"));
	enemy1->runAction(enemy1AttackAnimation);

	enemy2->stopAllActions();
	Animate* enemy2AttackAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2AttackAnimation"));
	enemy2->runAction(enemy2AttackAnimation);

	enemy3->stopAllActions();
	Animate* enemy3AttackAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3AttackAnimation"));
	enemy3->runAction(enemy3AttackAnimation);
}

void GameScene::deadMenuCallback(Ref* pSender) {
	enemy1->stopAllActions();
	Animate* enemy1DeadAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1DeadAnimation"));
	enemy1->runAction(enemy1DeadAnimation);

	enemy2->stopAllActions();
	Animate* enemy2DeadAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2DeadAnimation"));
	enemy2->runAction(enemy2DeadAnimation);

	enemy3->stopAllActions();
	Animate* enemy3DeadAnimation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3DeadAnimation"));
	enemy3->runAction(enemy3DeadAnimation);
}

void GameScene::walkMenuCallback(Ref* pSender) {
	enemy1->stopAllActions();
	Animate* enemy1Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy1Animation"));
	enemy1->runAction(RepeatForever::create(enemy1Animation));

	enemy2->stopAllActions();
	Animate* enemy2Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy2Animation"));
	enemy2->runAction(RepeatForever::create(enemy2Animation));

	enemy3->stopAllActions();
	Animate* enemy3Animation = Animate::create(AnimationCache::getInstance()->getAnimation("enemy3Animation"));
	enemy3->runAction(RepeatForever::create(enemy3Animation));
}

void GameScene::quitCallback(Ref* pSender) {
	isQuit = true;
	auto newScene = MenuScene::createScene();
	Director::getInstance()->replaceScene(CCTransitionFade::create(0.5, newScene));
}


void GameScene::addTower1(Object* pSender)
{
	if (isGameOver)
		return;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (readyItem != NULL)
	{
		readyItem->removeFromParentAndCleanup(true);
		readyItem = NULL;
	}
	clickItemtype = 1;
	readyItem = Sprite::create("tower1.png");
	readyItem->setScale(0.7);
	readyItem->setPosition(visibleSize.width - 100, 150);
	this->addChild(readyItem,15);
}
void GameScene::addTower2(Object* pSender)
{
	if (isGameOver)
		return;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (readyItem != NULL)
	{
		readyItem->removeFromParentAndCleanup(true);
		readyItem = NULL;
	}
	clickItemtype = 2;
	readyItem = Sprite::create("tower2.png");
	readyItem->setScale(0.8);
	readyItem->setPosition(visibleSize.width - 100, 250);
	this->addChild(readyItem, 15);
}
void GameScene::addTower3(Object* pSender)
{
	if (isGameOver)
		return;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (readyItem != NULL)
	{
		readyItem->removeFromParentAndCleanup(true);
		readyItem = NULL;
	}
	clickItemtype = 3;
	readyItem = Sprite::create("tower3.png");
	readyItem->setScale(0.8);
	readyItem->setPosition(visibleSize.width - 100, 330);
	this->addChild(readyItem, 15);
}

bool GameScene::onTouchBegan(Touch *touch, Event* event)
{
	if (isGameOver)
		return false;

	CCLOG("%f,%f", touch->getLocation().x, touch->getLocation().y);
	if (prevPos.getDistance(touch->getLocation()) < 1)
	{
		return false;
	}
	bool isClick = false;
	bool clickingMenu = false;
	if (towerManager.clickTower(touch->getLocation()))
	{
		isClick = true;
		Tower clickingTower = towerManager.getTowerThroughPos(touch->getLocation());
		if (clickingTower.getType() == 4)//点击图腾
		{
			if (towerMenu != NULL)
			{
				towerMenu->removeFromParentAndCleanup(true);
				towerMenu = NULL;
			}
			return false;

		}
		else if (clickingTower.isGround() && clickItemtype != -1)//点击的是空地且待选项非空
		{
			if (money < 200)
				return false;
			else
				modifyMoney(1);
			towerManager.changeTower(touch->getLocation(), clickItemtype);

			clickItemtype = -1;
			readyItem->removeFromParentAndCleanup(true);
			readyItem = NULL;
		}
		else if (!clickingTower.isGround() && towerMenu == NULL) //展开菜单
		{
			auto update = MenuItemImage::create("update.png", "update.png", CC_CALLBACK_1(GameScene::updateTower, this));
			update->setScale(0.4);
			update->setPosition(Vec2(-30, 50));
			auto deleting = MenuItemImage::create("delete.png", "delete.png", CC_CALLBACK_1(GameScene::deleteTower, this));
			deleting->setScale(0.4);
			deleting->setPosition(Vec2(30, 50));

			towerMenu = Menu::create(update, deleting, NULL);
			towerMenu->setPosition(clickingTower.getPosition());
			this->addChild(towerMenu, 20);
			clickingMenu = true;
			menuPos = touch->getLocation();
		}
		else if (!clickingTower.isGround() && towerMenu != NULL)
		{
			towerMenu->removeFromParentAndCleanup(true);
			auto update = MenuItemImage::create("update.png", "update.png", CC_CALLBACK_1(GameScene::updateTower, this));
			update->setScale(0.4);
			update->setPosition(Vec2(-30, 50));
			auto deleting = MenuItemImage::create("delete.png", "delete.png", CC_CALLBACK_1(GameScene::deleteTower, this));
			deleting->setScale(0.4);
			deleting->setPosition(Vec2(30, 50));

			towerMenu = Menu::create(update, deleting, NULL);
			towerMenu->setPosition(clickingTower.getPosition());
			this->addChild(towerMenu, 20);
			clickingMenu = true;
			menuPos = touch->getLocation();
		}
	}

	if (!clickingMenu && towerMenu != NULL)//点击除了菜单外的任何地方都会导致菜单回缩
	{
		towerMenu->removeFromParentAndCleanup(true);
		towerMenu = NULL;
	}
	prevPos = touch->getLocation();
	return isClick;
}

void GameScene::deleteTower(Object* pSender)
{
	towerManager.deleteTower(menuPos);
	towerMenu->removeFromParentAndCleanup(true);
	towerMenu = NULL;
}
void GameScene::updateTower(Object* pSender)
{
	if (money >= 200) {
		modifyMoney(1);
		towerManager.updateTower(menuPos);
	}
	towerMenu->removeFromParentAndCleanup(true);
	towerMenu = NULL;
}

//新加
//发射子弹
void GameScene::bullet(float f)
{
	if (isGameOver)
		return;
	Sprite* Smonster;
	Sprite* Stower;
	for (auto iter = towerManager.tower.begin(); iter != towerManager.tower.end(); iter++)
	{
		Stower = (Sprite*)(*iter).getTower();
		if ((*iter).getType() == 0)
			continue;

		(*iter).counting();
		if (!(*iter).isFire())
		{
			continue;
		}

		for (auto iter1 = monsterManager.storage.begin(); iter1 != monsterManager.storage.end(); iter1++)
		{
			Smonster = (Sprite*)(*iter1).getSprite();
			//判断怪物与塔的距离，如果小于塔的射程，那么发射子弹
			//if (sqrt(pow(Smonster->getPosition().x - Stower->getPosition().x, 2) + pow(Smonster->getPosition().y - Stower->getPosition().y, 2))<(*iter).getTowerProperty().range)
			if(Smonster->getPosition().getDistance(Stower->getPosition())<(*iter).getTowerProperty().range)
			{
				string picture = "";
				float offsetx, offsety, scale;
				switch ((*iter).getType())
				{
				case 1:
					picture = "tower1Bullet.png";
					offsetx = -2;
					offsety = 40;
					scale = 0.8;
					break;
				case 2:
					picture = "tower2Bullet.png";
					offsetx = -4;
					offsety = 40;
					scale = 0.5;
					break;
				case 3:
					picture = "tower3Bullet.png";
					offsetx = -4;
					offsety = 13;
					scale = 0.8;
					break;
				case 4:
					picture = "TotemBullet.png";
					offsetx = 16;
					offsety = 50;
					scale = 0.8;
					break;
				}
				auto towerBullet = Sprite::create(picture);
				towerBullet->setScale(scale);
				towerBullet->setPosition(Stower->getPosition() + Vec2(offsetx, offsety));
				auto time = Smonster->getPosition().getDistance(Stower->getPosition()) / (*iter).getTowerProperty().speed;

				towerBullet->runAction(Sequence::create(MoveTo::create(time, Vec2(Smonster->getPosition())), FadeOut::create(time), nullptr));

				while (judgingBullets)//正在判断碰撞的时候延迟发射子弹（互斥锁）
				{
				}
				bullets.push_back(towerBullet);
				bulletAttack.push_back((*iter).getAttack());
				this->addChild(towerBullet, 11);
				break;
			}
		}

	}

}
//子弹打中怪物
void GameScene::hitByBullet()
{
	Sprite* Smonster;
	judgingBullets = true;
	for (auto iter1 = monsterManager.storage.begin(); iter1 != monsterManager.storage.end(); iter1++)
	{
		Smonster = (Sprite*)(*iter1).getSprite();

		auto iter3 = bulletAttack.begin();
		for (auto iter2 = bullets.begin(); iter2 != bullets.end(); )
		{
			if (Smonster->getBoundingBox().containsPoint((*iter2)->getPosition()))//爆炸，删除子弹
			{
				//到时候这里添加血条减少的操作
				Sprite* boomBullet = (*iter2);
				boomBullet->runAction(
					Sequence::create(
						Animate::create(
							Animation::createWithSpriteFrames(explore, 0.05f, 1)),
						CallFunc::create([boomBullet] {
							boomBullet->removeFromParentAndCleanup(true);
						}),
						nullptr));

				//被击杀
				if ((*iter1).beingAttacked(*iter3))
				{
					monsterManager.monsterAnimate((*iter1), "dead");
					modifyMoney(0);
				}
				;

				iter2 = bullets.erase(iter2);
				iter3 = bulletAttack.erase(iter3);
			}
			else
			{
				iter2++;
				iter3++;
			}
		}
	}
	judgingBullets = false;

}

void GameScene::createMonster(int rate)
{
	//int num = 3 + rate/2;
	int speed = 10 + 10 * ((rate + 1) / 2);

	monsterManager.createMonster("enemy1_0.png", this, line1, monsterProperty(100, speed));
	monsterManager.createMonster("enemy2_0.png", this, line2, monsterProperty(100, speed));
	monsterManager.createMonster("enemy3_0.png", this, line3, monsterProperty(100, speed));
}

void GameScene::gameWin()
{
}

void GameScene::gameLose()
{
	if (!isGameOver)
	{
		CCLOG("game lose");
		isGameOver = true;
		for (auto iter = bullets.begin(); iter != bullets.end(); iter++)
		{
			(*iter)->stopAllActions();
		}
		monsterManager.stopAllActions();
		//游戏结束
	}
}

//修改金币
void GameScene::modifyMoney(int type)
{
	switch (type)
	{
		//击杀怪兽
	case 0:
		money += 20;
		break;

		//升级1类型塔
	case 1:
		money -= 200;
		break;

		//升级2类型塔
	case 2:
		money -= 200;
		break;

		//升级3类型塔
	case 3:
		money -= 200;
		break;
	default:
		break;
	}

	char str[15];
	sprintf(str, "Money: %d", money);
	moneyNum->setString(str);
}