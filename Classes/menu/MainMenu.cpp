//
//  MainMenu.cpp
//  my_single
//
//  Created by awd on 10/13/14.
//
//

#include "MainMenu.h"
#include "RecordList.h"
#include "AboutLayer.h"
#include "../game/Game.h"
#include "../data/GameDataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum TAG{
    MENU_TAG,
    MENU_ITEM_RESUME,
    MENU_ITEM_NEW,
    MENU_ITEM_RECORDS,
    MENU_ITEM_ABOUT
};

Scene* MainMenu::createScene(){
    Scene* scene = Scene::create();
    
    MainMenu* main_menu = MainMenu::create();
    main_menu->bootup = true;
    scene->addChild(main_menu);
    
    return scene;
}

MainMenu* MainMenu::create(bool bootup){
    MainMenu *pRet = new MainMenu();
    pRet->bootup = bootup;
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool MainMenu::init(){
    if(!Layer::init()){
        return false;
    }
    Size visiableSize = Director::getInstance()->getVisibleSize();
    
    float w = visiableSize.width;
    float fix = 200;
    float left_dest  = (w + fix)/2;
    float left_src   = (fix - w)/2;
    float right_src  = (3*w - fix)/2;
    float right_dest = (w-fix)/2;
    float h = 158;
    float between = 5;
    float start_at = 630;
    
    //logo
    Size winSize = Director::getInstance()->getWinSize();
    logo = Sprite::create("logo.png");
    addChild(logo);
    logo->setOpacity(0);
    
    Vec2 logo_p(winSize.width/2, winSize.height/2);
    logo_p.y = start_at + h/2 + (winSize.height - start_at - h/2 - 200)/2 + 100;
    
    if(bootup){
        logo->setPosition(winSize.width/2, winSize.height/2);
        auto action = Spawn::create(FadeIn::create(1.5f), MoveTo::create(1.5f, logo_p), nullptr);
        logo->runAction(action);
    } else {
        logo->setPosition(logo_p);
        logo->runAction(FadeIn::create(1.5f));
    }
    
    MenuItemImage* resume = MenuItemImage::create("lbl_resume.png", "", CC_CALLBACK_1(MainMenu::menu_callback, this));
    resume->setTag(MENU_ITEM_RESUME);
    resume->setOpacity(0);
    MenuItemImage* newgame = MenuItemImage::create("lbl_newgame.png", "", CC_CALLBACK_1(MainMenu::menu_callback, this));
    newgame->setTag(MENU_ITEM_NEW);
    newgame->setOpacity(0);
    MenuItemImage* records = MenuItemImage::create("lbl_records.png", "", CC_CALLBACK_1(MainMenu::menu_callback, this));
    records->setTag(MENU_ITEM_RECORDS);
    records->setOpacity(0);
    MenuItemImage* about = MenuItemImage::create("lbl_about.png", "", CC_CALLBACK_1(MainMenu::menu_callback, this));
    about->setTag(MENU_ITEM_ABOUT);
    about->setOpacity(0);
    
    Vec2 dest(left_dest, start_at);
    if(GameDataManager::getInstance()->current_item.moves.size() > 0){
        green = Scale9Sprite::create("brush_green.9.png", Rect(0, 0, 263, 105), Rect(40, 60, 10, 10));
        green->setPosition(left_src, start_at);
        green->setContentSize(Size(visiableSize.width + fix, h));
        addChild(green);
        green->runAction(Sequence::create(MoveTo::create(0.8, dest), TargetedAction::create(resume, FadeIn::create(0.3f)),nullptr));
        resume->setPosition(dest);
        start_at = start_at - h - between;
    }
    
    dest = Vec2(right_dest, start_at);
    blue = Scale9Sprite::create("brush_blue.9.png", Rect(0, 0, 263, 105), Rect(40, 60, 10, 10));
    blue->setRotation(180);
    blue->setPosition(right_src, start_at);
    blue->setContentSize(Size(visiableSize.width + fix, h));
    addChild(blue);
    blue->runAction(Sequence::create(MoveTo::create(0.8, dest), TargetedAction::create(newgame, FadeIn::create(0.3f)),nullptr));
    newgame->setPosition(dest);
    
    start_at = start_at - h - between;
    dest = Vec2(left_dest, start_at);
    pink = Scale9Sprite::create("brush_pink.9.png", Rect(0, 0, 263, 105), Rect(40, 60, 10, 10));
    pink->setPosition(left_src, start_at);
    pink->setContentSize(Size(visiableSize.width + fix, h));
    addChild(pink);
    pink->runAction(Sequence::create(MoveTo::create(0.8, dest), TargetedAction::create(records, FadeIn::create(0.3f)),nullptr));
    records->setPosition(dest);
    
    start_at = start_at - h - between;
    dest = Vec2(right_dest, start_at);
    yellow = Scale9Sprite::create("brush_yellow.9.png", Rect(0, 0, 263, 105), Rect(40, 60, 10, 10));
    yellow->setRotation(180);
    yellow->setPosition(right_src, start_at);
    yellow->setContentSize(Size(visiableSize.width + fix, h));
    addChild(yellow);
    yellow->runAction(Sequence::create(MoveTo::create(0.8, dest), TargetedAction::create(about, FadeIn::create(0.3f)),nullptr));
    about->setPosition(dest);
    
    Menu* menu;
    if(GameDataManager::getInstance()->current_item.moves.size() > 0){
        menu = Menu::create(resume, newgame, records, about, nullptr);
    }else {
        menu = Menu::create(newgame, records, about, nullptr);
    }
    menu->setPosition(Vec2::ZERO);
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setTag(MENU_TAG);
    addChild(menu);
    
    //add key listener
    _utils = GameUtils::create();
    addChild(_utils);
    _utils->add_key_listener(this);
    
    return true;
}

MainMenu::~MainMenu(){
    if(_utils){
        _utils->remove_key_listener();
    }
}

void MainMenu::first_key_pressed(){
    auto fade = FadeOut::create(2);
    auto label = Label::createWithSystemFont("press agine to exit", "", 20);
    label->setColor(Color3B::BLACK);
    this->addChild(label);
    label->setPosition(_utils->visibleSize.width/2, _utils->visibleSize.height/5);
    label->runAction(fade);
}

void MainMenu::second_key_pressed() {
    _utils->app_exit();
}

void MainMenu::menu_callback(Ref* sender){
    Menu* menu = (Menu*)sender;
    int tag = menu->getTag();
    switch (tag) {
        case MENU_ITEM_RESUME:{
            exit_with_animation();
            
            runAction(Sequence::create(DelayTime::create(1.1f), CallFunc::create([&]{
                Game* game = Game::create();
                getParent()->addChild(game);
                removeFromParent();
            }),nullptr));
        }
            break;
        case MENU_ITEM_NEW:{
            exit_with_animation();
            GameDataManager::getInstance()->new_game();
            
            runAction(Sequence::create(DelayTime::create(1.1f), CallFunc::create([&]{
                
                Game* game = Game::create();
                getParent()->addChild(game);
                removeFromParent();
            }),nullptr));
        }
            break;
        case MENU_ITEM_RECORDS:{
            RecordList* list = RecordList::create();
            getParent()->addChild(list);
        }
            break;
        case MENU_ITEM_ABOUT:{
            AboutLayer* about = AboutLayer::create();
            getParent()->addChild(about);
        }
            break;
        default:
            break;
    }
}

void MainMenu::enter_with_animation(){
    
}

void MainMenu::exit_with_animation(){
    Size visiableSize = Director::getInstance()->getVisibleSize();
    float fix = 500;
    float left = -(visiableSize.width + fix)/2;
    float right = visiableSize.width + (visiableSize.width + fix)/2;
    
    Node* menu = getChildByTag(MENU_TAG);
    menu->runAction(FadeOut::create(0.3f));
    
    if(logo){
        logo->runAction(FadeOut::create(0.8f));
    }
    
    if(green){
        green->runAction(EaseElasticOut::create(MoveTo::create(0.8f, Vec2(left, green->getPosition().y))));
    }
    if(blue){
        blue->runAction(EaseElasticOut::create(MoveTo::create(0.8f, Vec2(right, blue->getPosition().y))));
    }
    if(pink){
        pink->runAction(EaseElasticOut::create(MoveTo::create(0.8f, Vec2(left, pink->getPosition().y))));
    }
    if(yellow){
        yellow->runAction(EaseElasticOut::create(MoveTo::create(0.8f, Vec2(right, yellow->getPosition().y))));
    }
}