//
//  ResultLayer.cpp
//  my_single
//
//  Created by awd on 10/8/14.
//
//

#include "ResultLayer.h"
#include "BackgroundLayer.h"
#include "../data/GameDataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool ResultLayer::init(){
    if(!Layer::init()){
        return false;
    }
    
    BackgroundLayer* background = BackgroundLayer::create();
    addChild(background);
    
    Size winSize = Director::getInstance()->getWinSize();
    
    _container = Node::create();
    _container->setPosition(winSize.width/2, winSize.height/2);
    addChild(_container);

    bg = Sprite::create("bg_dialog.png");
    bg->setPosition(0, 0);
    _container->addChild(bg);
    
    _label = Label::createWithBMFont("fnt_text.fnt", "剩4钻");
    _label->setPosition(0, 0);
    _container->addChild(_label);
    
    enter_with_animation();
    addTouchListener();
    
    return true;
}
ResultLayer::~ResultLayer(){
    if(_listener){
        _eventDispatcher->removeEventListener(_listener);
    }
}

void ResultLayer::addTouchListener(){
    _listener = EventListenerTouchOneByOne::create();
    _listener->onTouchBegan = CC_CALLBACK_2(ResultLayer::TouchBegan, this);
    _listener->onTouchEnded = CC_CALLBACK_2(ResultLayer::TouchEnded, this);
    _listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, bg);
}


bool ResultLayer::TouchBegan(Touch* touch, Event* evnet){
    return true;
}
void ResultLayer::TouchEnded(Touch* touch, Event* event){
    Vec2 location = touch->getLocation();
    location = _container->convertToNodeSpace(location);
    if(bg && bg->getBoundingBox().containsPoint(location)){
        if(_listener) {
            _eventDispatcher->removeEventListener(_listener);
        }
        
        exit_with_animation();
        runAction(Sequence::create(DelayTime::create(1.1f), CallFunc::create([&]{
            removeFromParent();
        }),nullptr));
    }
}

void ResultLayer::enter_with_animation(){
    if(_container){
        _container->setScale(0);
        auto action = ScaleTo::create(0.8f, 1);
        _container->runAction(EaseElasticOut::create(action));
    }
}

void ResultLayer::exit_with_animation(){
    if(_container){
        auto action = ScaleTo::create(0.8f, 0);
        _container->runAction(EaseElasticIn::create(action));
    }
}

void ResultLayer::set_left_stars(int stars, bool center){
    _left_stars = stars;
    GameDataManager::getInstance()->finish_game();
    
    std::string state = "";
    switch (_left_stars) {
        case 5:
            state = "行脚僧";
            break;
        case 4:
            state = "比丘";
            break;
        case 3:
            state = "沙祢";
            break;
        case 2:
            state = "长老";
            break;
        case 1:
            state = "大师";
            if(center)state = "方丈";
            break;
            
        default:
            break;
    }
    if(_left_stars > 5) state = "小和尚";
    
    std::ostringstream oss;
    if(stars == 1 && center){
        oss << "天才";
    } else {
        oss << "剩" << _left_stars << "钻";
    }
    _label->setString(oss.str());
}




