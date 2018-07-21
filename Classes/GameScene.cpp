#include "GameScene.h"
#include "MenuScene.h"
#include "Line.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
// ��ΪͼƬ�ز�������1920*1080�ֱ��ʴ�����ģ�������Ϸ���ڴ�СΪ1280*920����Ҫ����scale
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

	//�����ã���ʾ�������
	auto listener = EventListenerMouse::create();
	listener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

	// ��������
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("sounds/Jungle_Battle.mp3");
	audio->playBackgroundMusic("sounds/Jungle_Battle.mp3", true);

	// ����ͼƬ
	auto gameBg = Sprite::create("GameBg.png");
	gameBg->setScale(MY_SCALE_SIZE);
	gameBg->setPosition(Vec2(myXPosition, myYPosition));
	this->addChild(gameBg, 0);

	// Ѫ�������Ǹ����
	auto healthBarContaier = Sprite::create("HealthBar1.png");
	healthBarContaier->setScale(1, 0.7);
	healthBarContaier->setPosition(Vec2(myXPosition, myYPosition + 320));
	this->addChild(healthBarContaier, 10);

	// Ѫ����ͼ�ڵ�Ѫ����ͼ��Ѫ��Ϊ0����Ϸ������
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


	// ͼ��
	/*
	auto totem = Sprite::create("Totem.png");
	totem->setScale(MY_SCALE_SIZE);
	totem->setPosition(Vec2(myXPosition, myYPosition - 300));
	this->addChild(totem, 2);
	*/
	towerManager.createTower("Totem.png", TowerProperty(10, 300, 800,20), Vec2(myXPosition, myYPosition - 300),this);

	// ͼ���ӵ�
		/*
	auto totemBullet = Sprite::create("TotemBullet.png");
	totemBullet->setScale(MY_SCALE_SIZE);
	// ����ͼ���ӵ��������ʼλ��
	totemBullet->setPosition(Vec2(myXPosition + 16, myYPosition - 258));
	this->addChild(totemBullet, 3);*/

	//��ը����
	auto texture = Director::getInstance()->getTextureCache()->addImage("explosion.png");
	explore.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(190 * (i % 5), 200 * (i >= 5), 190, 200)));
		explore.pushBack(frame);
	}

	/*
	// ��λ����
	towerLandsNum = 12;

	// ������λ��λ��
	// ÿ����ɫ��Ϊһ�飬�������λλ�õ�˳���Ǵ�����
	// ���Ͽ�
	towerLandsPositions.push_back(Vec2(myXPosition - 530, myYPosition + 165));	// 0
	towerLandsPositions.push_back(Vec2(myXPosition - 210, myYPosition + 150));	// 1
	towerLandsPositions.push_back(Vec2(myXPosition - 10, myYPosition + 230));	// 2

	towerLandsPositions.push_back(Vec2(myXPosition + 55, myYPosition + 20));	// 3
	towerLandsPositions.push_back(Vec2(myXPosition + 180, myYPosition - 100));	// 4

																				// ���
	towerLandsPositions.push_back(Vec2(myXPosition - 260, myYPosition + 10));	// 5

																				// ���¿�
	towerLandsPositions.push_back(Vec2(myXPosition - 320, myYPosition - 120));	// 6
	towerLandsPositions.push_back(Vec2(myXPosition - 100, myYPosition - 100));	// 7
	towerLandsPositions.push_back(Vec2(myXPosition - 30, myYPosition - 150));	// 8

																				// ���Ͽ�
	towerLandsPositions.push_back(Vec2(myXPosition + 180, myYPosition + 180));	// 9
	towerLandsPositions.push_back(Vec2(myXPosition + 310, myYPosition + 60));	// 10

																				// ���¿�
	towerLandsPositions.push_back(Vec2(myXPosition + 390, myYPosition - 95));	// 11

																				// ������λ
	for (int i = 0; i < towerLandsNum; i++)
	{
		auto towerLand = Sprite::create("TowerLand.png");
		towerLand->setScale(MY_SCALE_SIZE);
		towerLand->setPosition(towerLandsPositions[i]);
		// �ŵ�towerLands��Ա���洦���ɸ�����Ҫ�޸�
		towerLands.push_back(towerLand);
		this->addChild(towerLand, 1);
	}
	// ���Ͷ�Ӧ���ӵ���ע���������scaleֵ��λ��ƫ�����������ã���Ҫ��������ֵ��ʾ����
	auto tower1 = Sprite::create("tower1.png");
	tower1->setScale(0.7);
	// ������λ��λ�õ������������ע�͡�����λ��Ҫ����һ��ƫ������Vec2(-2, 20)��
	tower1->setPosition(towerLandsPositions[4] + Vec2(-2, 16));
	this->addChild(tower1, 2);
	// �ӵ�
	auto tower1Bullet = Sprite::create("tower1Bullet.png");
	tower1Bullet->setScale(0.8);
	tower1Bullet->setPosition(towerLandsPositions[4] + Vec2(-2, 50));
	this->addChild(tower1Bullet, 3);

	auto tower2 = Sprite::create("tower2.png");
	tower2->setScale(0.8);
	tower2->setPosition(towerLandsPositions[8] + Vec2(-4, 20));
	this->addChild(tower2, 2);
	// �ӵ�
	auto tower2Bullet = Sprite::create("tower2Bullet.png");
	tower2Bullet->setScale(0.5);
	tower2Bullet->setPosition(towerLandsPositions[8] + Vec2(-4, 60));
	this->addChild(tower2Bullet, 3);

	auto tower3 = Sprite::create("tower3.png");
	tower3->setScale(0.8);
	tower3->setPosition(towerLandsPositions[3] + Vec2(0, 4));
	this->addChild(tower3, 2);
	// �ӵ�
	auto tower3Bullet = Sprite::create("tower3Bullet.png");
	tower3Bullet->setScale(0.8);
	tower3Bullet->setPosition(towerLandsPositions[3] + Vec2(-4, 20));
	this->addChild(tower3Bullet, 3);
	*/
	//��ѡ��˵�
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
	//���յ���Ϊ�����������������Ϊ�ڿյ��ڲ���ת��
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 530, myYPosition + 165), this);	// 0
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 210, myYPosition + 150), this);	// 1
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 10, myYPosition + 230), this);	// 2

	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 55, myYPosition + 20), this);	// 3
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 180, myYPosition - 100), this);	// 4

																				// ���
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 260, myYPosition + 10), this);	// 5

																				// ���¿�
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 320, myYPosition - 120), this);	// 6
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 100, myYPosition - 100), this);	// 7
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition - 30, myYPosition - 150), this);	// 8

																				// ���Ͽ�
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 180, myYPosition + 180), this);	// 9
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 310, myYPosition + 60), this);	// 10

																				// ���¿�
	towerManager.createTower("TowerLand.png", TowerProperty(), Vec2(myXPosition + 390, myYPosition - 95), this);	// 11
	// �������˵���ť
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

	//������һ������
	rate = 0;
	createMonster(rate);
	is_create = true;
	ct = 0;

	//��ʾ��Ҹ�����
	rateNum = Label::createWithTTF("Rate: 1/10", "fonts/arial.TTF", 40);
	rateNum->setColor(Color3B(255, 255, 255));
	rateNum->setPosition(visibleSize.width - 150, visibleSize.height - 50);
	this->addChild(rateNum, 3);
	moneyNum = Label::createWithTTF("Money: 400", "fonts/arial.TTF", 40);
	moneyNum->setColor(Color3B(255, 255, 255));
	moneyNum->setPosition(visibleSize.width - 165, visibleSize.height - 100);
	this->addChild(moneyNum, 3);

	//������
	schedule(schedule_selector(GameScene::update), 0.1f, kRepeatForever, 0);
	schedule(schedule_selector(GameScene::bullet), 0.1f, kRepeatForever, 0);
	//�¼�������
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
	else {	//�ж�������һ���¹��޻�����Ϸ����
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
		if (clickingTower.getType() == 4)//���ͼ��
		{
			if (towerMenu != NULL)
			{
				towerMenu->removeFromParentAndCleanup(true);
				towerMenu = NULL;
			}
			return false;

		}
		else if (clickingTower.isGround() && clickItemtype != -1)//������ǿյ��Ҵ�ѡ��ǿ�
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
		else if (!clickingTower.isGround() && towerMenu == NULL) //չ���˵�
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

	if (!clickingMenu && towerMenu != NULL)//������˲˵�����κεط����ᵼ�²˵�����
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

//�¼�
//�����ӵ�
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
			//�жϹ��������ľ��룬���С��������̣���ô�����ӵ�
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

				while (judgingBullets)//�����ж���ײ��ʱ���ӳٷ����ӵ�����������
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
//�ӵ����й���
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
			if (Smonster->getBoundingBox().containsPoint((*iter2)->getPosition()))//��ը��ɾ���ӵ�
			{
				//��ʱ���������Ѫ�����ٵĲ���
				Sprite* boomBullet = (*iter2);
				boomBullet->runAction(
					Sequence::create(
						Animate::create(
							Animation::createWithSpriteFrames(explore, 0.05f, 1)),
						CallFunc::create([boomBullet] {
							boomBullet->removeFromParentAndCleanup(true);
						}),
						nullptr));

				//����ɱ
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
		//��Ϸ����
	}
}

//�޸Ľ��
void GameScene::modifyMoney(int type)
{
	switch (type)
	{
		//��ɱ����
	case 0:
		money += 20;
		break;

		//����1������
	case 1:
		money -= 200;
		break;

		//����2������
	case 2:
		money -= 200;
		break;

		//����3������
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