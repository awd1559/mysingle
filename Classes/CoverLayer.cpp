#include "CoverLayer.h"
#include "menu/BackgroundLayer.h"
#include "menu/MainMenu.h"

//#include "AgentManager.h"
//#include "any/Ads.h"

//using namespace anysdk::framework;
USING_NS_CC;

Scene* CoverLayer::createScene()
{
	//Initialize AnySDK
	std::string oauthLoginServer = "http://oauth.anysdk.com/api/OauthLoginDemo/Login.php";
	std::string appKey = "CA0700A7-2F58-22C9-DB9A-4B7BCE17E1BD";
	std::string appSecret = "89c39d46a721896646d416597d5b716e";
	std::string privateKey = "6AC8397C6BC11EFC9C8517F13D4D5498";

	//AgentManager::getInstance()->init(appKey,appSecret,privateKey,oauthLoginServer);
	//AgentManager::getInstance()->loadALLPlugin();

    auto scene = Scene::create();
    
    BackgroundLayer* background = BackgroundLayer::create();
    scene->addChild(background);
    
    auto layer = CoverLayer::create();
    scene->addChild(layer);

    return scene;
}

bool CoverLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    auto logo = Sprite::create("logo.png");
    logo->setPosition(winSize.width/2, winSize.height/2);
    addChild(logo);
    
    auto lbl_logo = Sprite::create("lbl_logo.png");
    lbl_logo->setPosition(winSize.width/2, winSize.height/3);
    addChild(lbl_logo);
    
    add_touch_lisener();
    
    //after 2 sec goto MainMenu.scene
    this->scheduleOnce(schedule_selector(CoverLayer::gotoMain), 2.0f);
    
    //Ads::getInstance()->showBanner();

    return true;
}

void CoverLayer::add_touch_lisener() {
    _listener = EventListenerTouchOneByOne::create();
    _listener->onTouchBegan = CC_CALLBACK_2(CoverLayer::touch_began, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

CoverLayer::~CoverLayer(){
    if(_listener){
        _eventDispatcher->removeEventListener(_listener);
    }
}

bool CoverLayer::touch_began(Touch* sender, Event* evnet){
    gotoMain(1);
    return true;
}

void CoverLayer::gotoMain(float dt){
    if(_listener){
        _eventDispatcher->removeEventListener(_listener);
        _listener = nullptr;
    }
    this->unschedule(schedule_selector(CoverLayer::gotoMain));
    
    MainMenu* main_menu = MainMenu::create(true);
    this->getParent()->addChild(main_menu);
    removeFromParent();
}

