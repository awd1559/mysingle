//
//  RecordList.cpp
//  my_single
//
//  Created by awd on 10/13/14.
//
//

#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "RecordList.h"
#include "MainMenu.h"
#include "../data/GameDataManager.h"
#include "../data/SQLManager.h"
#include "../game/Game.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

enum TAG{
    TAG_SCROLL_VIEW,
};

Scene* RecordList::createScene(){
    Scene* scene = Scene::create();
    
    RecordList* record_list = RecordList::create();
    scene->addChild(record_list);
    return scene;
}

bool RecordList::init(){
    if(!Layer::init()){
        return false;
    }
    
    _container = Node::create();
    _container->setPosition(0, 0);
    addChild(_container);
    
    auto bg = LayerColor::create(Color4B(17, 232, 245, 255));
    _container->addChild(bg);
    
    Size winSize = Director::getInstance()->getWinSize();
    MenuItemImage* exit = MenuItemImage::create("exit.png", "", CC_CALLBACK_1(RecordList::menu_callback, this));
    exit->setScale(2);
    exit->setAnchorPoint(Vec2(0, 1));
    exit->setPosition(0, winSize.height);
    Rect exit_bound = exit->getBoundingBox();
    Menu* menu = Menu::create(exit, nullptr);
    menu->setPosition(Vec2::ZERO);
    menu->setAnchorPoint(Vec2::ZERO);
    _container->addChild(menu);
    
    
    float h = 129;
    ui::ScrollView* scrollView = ui::ScrollView::create();
    scrollView->setTag(TAG_SCROLL_VIEW);
    scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollView->setBounceEnabled(true);
    scrollView->setPosition(Vec2(0,0));
    scrollView->setContentSize(winSize-Size(0, exit_bound.size.height));
    scrollView->setInnerContainerSize(Size(winSize.width, 20*h));
    scrollView->setClippingEnabled(true);
    
    std::vector<int> ids = SQLManager::getInstance()->get_first_sorted_ids();
    std::vector<int> moves = SQLManager::getInstance()->get_first_sorted_moves();
    int size = ids.size();
    for(int i = 0; i < size; i++){
        int id = ids.at(i);
        int count = moves.at(i);
        bool center = (count == 100);
        if(center) count = 1;
        
        std::ostringstream os;
        os << i + 1;
        
        ui::Layout* panel = ui::Layout::create();
        panel->setPosition(Vec2(winSize.width/2, (20-i - 1) * h));
        panel->setBackGroundImage("item_bg.png");
        scrollView->addChild(panel);
        Label *lab  = Label::createWithBMFont("fnt_count.fnt", os.str(), TextHAlignment::CENTER, 150);
        lab->setPosition(-230, 0);
        panel->addChild(lab);
        

        std::ostringstream os1;
        os1 << "剩 " << count << " 钻";
        Label* label = Label::createWithBMFont("fnt_text.fnt", os1.str(), TextHAlignment::CENTER, 150);
        
        auto normal = Scale9Sprite::create();
        ControlButton *button = ControlButton::create(label, normal);
        button->setTitleColorForState(Color3B::BLUE, Control::State::HIGH_LIGHTED);
        button->setTag(id);
        button->addTargetWithActionForControlEvents(this, cccontrol_selector(RecordList::touchevent), Control::EventType::TOUCH_UP_INSIDE);
        panel->addChild(button);
    }
    if(size == 0){
        auto banner = Sprite::create("bg_dialog.png");
        banner->setPosition(winSize.width/2, winSize.height/2);
        _container->addChild(banner);
        Label* label = Label::createWithBMFont("fnt_text.fnt", "无成绩", TextHAlignment::CENTER, 150);
        banner->addChild(label);
    }
    _container->addChild(scrollView);
    enter_with_animation();
    
    return true;
}

void RecordList::enter_with_animation(){
    Size visiableSize = Director::getInstance()->getVisibleSize();
    
    ui::ScrollView* scrollView = (ui::ScrollView*)getChildByTag(TAG_SCROLL_VIEW);
    if(scrollView){
        scrollView->setPosition(Vec2(0,0));
    }
    
    _container->setPosition(0, -visiableSize.height);
    _container->runAction(EaseBackInOut::create(MoveTo::create(0.5f, Vec2(0, 0))));
}

void RecordList::exit_with_animation(){
    Size visiableSize = Director::getInstance()->getVisibleSize();
    
    ui::ScrollView* scrollView = (ui::ScrollView*)getChildByTag(1000);
    if(scrollView){
        scrollView->setPosition(Vec2(0,0));
    }
    
    _container->setPosition(0, 0);
    _container->runAction(EaseBackInOut::create(MoveTo::create(0.8f, Vec2(0, -visiableSize.height))));
}

void RecordList::menu_callback(Ref* sender){
    exit_with_animation();
    
    runAction(Sequence::create(DelayTime::create(1.1f), CallFunc::create([&]{
        removeFromParent();
    }),nullptr));
}


void RecordList::touchevent(Ref *sender, Control::EventType controlEvent){
    int tag = ((Node*)sender)->getTag();
    
    GameDataManager::getInstance()->load_by_id(tag);
    
    exit_with_animation();
    
    runAction(Sequence::create(DelayTime::create(1.1f), CallFunc::create([&]{
        Game* game = Game::create();
        this->getParent()->addChild(game);
        removeFromParent();
    }),nullptr));
}



