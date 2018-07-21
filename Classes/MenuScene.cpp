#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "chineseDisplay.h"
#include "GameScene.h"
#include <sstream>
#define database UserDefault::getInstance()
using std::stringstream;
USING_NS_CC;

Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto firework = ParticleFlower::create();
	firework->setPosition(480, 460);
	this->addChild(firework,100);

	// 背景音乐
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("sounds/Jungle_Prep.mp3");
	audio->playBackgroundMusic("sounds/Jungle_Prep.mp3", true);
	
	// 开始场景的背景图片
	auto startBg = Sprite::create("StartBg.png");
	startBg->setScale(0.667);
	startBg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(startBg, 0);

	// 选择难度栏的木板
	auto optionsBoard = Sprite::create("OptionsBoard.png");
	optionsBoard->setScale(0.667);
	optionsBoard->setPosition(Vec2(visibleSize.width / 2 + origin.x - 400, visibleSize.height / 2 + origin.y - 80));
	this->addChild(optionsBoard, 1);

	auto options = Sprite::create("Options.png");
	options->setRotation(2);
	options->setScale(1.1);
	options->setPosition(Vec2(visibleSize.width / 2 + origin.x - 360, visibleSize.height / 2 + origin.y + 50));
	this->addChild(options, 2);
	
	// 难度1
	auto choice1 = MenuItemImage::create(
		"OneNormal.png",
		"OneSelected.png",
		CC_CALLBACK_1(MenuScene::startCallback, this)
	);
	choice1->setScale(1.2);
	choice1->setPosition(Vec2(visibleSize.width / 2 + origin.x - 360, visibleSize.height / 2 + origin.y - 42));

	auto menu1 = Menu::create(choice1, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 3);

	// 难度2
	auto choice2 = MenuItemImage::create(
		"TwoNormal.png",
		"TwoSelected.png",
		CC_CALLBACK_1(MenuScene::startCallback, this)
	);
	choice2->setScale(1.2);
	choice2->setPosition(Vec2(visibleSize.width / 2 + origin.x - 345, visibleSize.height / 2 + origin.y - 105));

	auto menu2 = Menu::create(choice2, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 3);

	// 难度3
	auto choice3 = MenuItemImage::create(
		"ThreeNormal.png",
		"ThreeSelected.png",
		CC_CALLBACK_1(MenuScene::startCallback, this)
	);
	choice3->setScale(1.2);
	choice3->setPosition(Vec2(visibleSize.width / 2 + origin.x - 365, visibleSize.height / 2 + origin.y - 162));

	auto menu3 = Menu::create(choice3, NULL);
	menu3->setPosition(Vec2::ZERO);
	this->addChild(menu3, 3);

	//创建本地存储来显示高分榜
	if (!database->getBoolForKey("isExist")) {
		database->setBoolForKey("isExist", true);
		database->setIntegerForKey("one", 0);
		database->setIntegerForKey("two", 0);
		database->setIntegerForKey("three", 0);
		database->setIntegerForKey("four", 0);
		database->setIntegerForKey("five", 0);
		database->setIntegerForKey("six", 0);
		database->setIntegerForKey("seven", 0);
		database->setIntegerForKey("eight", 0);
		database->setIntegerForKey("nine", 0);
		database->setIntegerForKey("ten", 0);
	}
	auto one = database->getIntegerForKey("one");
	auto two = database->getIntegerForKey("two");
	auto three = database->getIntegerForKey("three");
	auto four = database->getIntegerForKey("four");
	auto five = database->getIntegerForKey("five");
	auto six = database->getIntegerForKey("six");
	auto seven = database->getIntegerForKey("seven");
	auto eight = database->getIntegerForKey("eight");
	auto nine = database->getIntegerForKey("nine");
	auto ten = database->getIntegerForKey("ten");

	char font[10] = { "高分榜" };
	String str = chineseDisplay::a(font);
	Label* highScore = Label::createWithSystemFont(str.getCString(), "宋体", 50);
	highScore->setPosition(visibleSize.width - 200, visibleSize.height - 200);
	this->addChild(highScore, 10);

	int fontSize = 30;
	int ydelta = 40;
	int fixPriority = 10;
	int xdelta = 200;
	auto Label1 = Label::createWithSystemFont("1. "+int2str(one), "宋体", fontSize);
	Label1->setPosition(visibleSize.width - xdelta, visibleSize.height - 200 - ydelta*1);
	this->addChild(Label1, fixPriority);
	auto Label2 = Label::createWithSystemFont("2. " + int2str(two), "宋体", fontSize);
	Label2->setPosition(visibleSize.width - xdelta, visibleSize.height - 200 - ydelta * 2);
	this->addChild(Label2, fixPriority);
	auto Label3 = Label::createWithSystemFont("3. " + int2str(three), "宋体", fontSize);
	Label3->setPosition(visibleSize.width - xdelta, visibleSize.height - 200 - ydelta * 3);
	this->addChild(Label3, fixPriority);
	auto Label4 = Label::createWithSystemFont("4. " + int2str(four), "宋体", fontSize);
	Label4->setPosition(visibleSize.width - xdelta, visibleSize.height - 200 - ydelta * 4);
	this->addChild(Label4, fixPriority);
	auto Label5 = Label::createWithSystemFont("5. " + int2str(five), "宋体", fontSize);
	Label5->setPosition(visibleSize.width - xdelta, visibleSize.height - 200 - ydelta * 5);
	this->addChild(Label5, fixPriority);
	auto Label6 = Label::createWithSystemFont("6. " + int2str(six), "宋体", fontSize);
	Label6->setPosition(visibleSize.width - xdelta, visibleSize.height - 200 - ydelta * 6);
	this->addChild(Label6, fixPriority);
	auto Label7 = Label::createWithSystemFont("7. " + int2str(seven), "宋体", fontSize);
	Label7->setPosition(visibleSize.width - xdelta, visibleSize.height - 200 - ydelta * 7);
	this->addChild(Label7, fixPriority);
	auto Label8 = Label::createWithSystemFont("8. " + int2str(eight), "宋体", fontSize);
	Label8->setPosition(visibleSize.width - xdelta, visibleSize.height - 200 - ydelta * 8);
	this->addChild(Label8, fixPriority);
	auto Label9 = Label::createWithSystemFont("9. " + int2str(nine), "宋体", fontSize);
	Label9->setPosition(visibleSize.width - xdelta, visibleSize.height - 200 - ydelta * 9);
	this->addChild(Label9, fixPriority);
	auto Label10 = Label::createWithSystemFont("10. " + int2str(ten), "宋体", fontSize);
	Label10->setPosition(visibleSize.width - xdelta, visibleSize.height - 200 - ydelta * 10);
	this->addChild(Label10, fixPriority);
    return true;
}

// 选择难度后的回调函数，跳转到游戏场景GameScene
void MenuScene::startCallback(Ref* pSender) {
	auto newScene = GameScene::createScene();
	Director::getInstance()->replaceScene(CCTransitionFade::create(0.5, newScene));
}

string MenuScene::int2str(int x)
{
	stringstream ss;
	ss << x;
	return ss.str();
}

