//
//  AboutLayer.cpp
//  my_single
//
//  Created by awd on 10/22/14.
//
//

#include "AboutLayer.h"

USING_NS_CC;

enum TAG{
    TAG_ABOUT,
    TAG_EXIT,
};

bool AboutLayer::init(){
    if(!Layer::init()){
        return false;
    }
    
    Size visiableSize = Director::getInstance()->getVisibleSize();
    
    _container = Node::create();
    _container->setPosition(0, 0);
    addChild(_container);
    
    bg = LayerColor::create(Color4B(17, 232, 245, 255));
    _container->addChild(bg);
    
    about = Sprite::create("about.png");
    about->setTag(TAG_ABOUT);
    about->setAnchorPoint(Vec2(0, 1));
    about->setPosition(0, visiableSize.height);
    _container->addChild(about);
    
    exit = Sprite::create("exit.png");
    exit->setAnchorPoint(Vec2(0, 1));
    exit->setPosition(0, visiableSize.height);
    exit->setScale(2);
    exit->setTag(TAG_EXIT);
    _container->addChild(exit);
    
    enter_with_animation();
    add_touch_listener();
    return true;
}

void AboutLayer::enter_with_animation() {
    Size visiableSize = Director::getInstance()->getVisibleSize();
    
    if(about){
        about->setPosition(0, visiableSize.height);
    }
    
    _container->setPosition(0, -visiableSize.height);
    _container->runAction(EaseBackInOut::create(MoveTo::create(0.8f, Vec2(0, 0))));
}

void AboutLayer::exit_with_animation(){
    Size visiableSize = Director::getInstance()->getVisibleSize();
    if(about){
        about->setPosition(0, visiableSize.height);
    }
   
    _container->setPosition(0, 0);
    _container->runAction(EaseBackInOut::create(MoveTo::create(0.8f, Vec2(0, -visiableSize.height))));
}

void AboutLayer::add_touch_listener(){
    _listener = EventListenerTouchOneByOne::create();
    _listener->setSwallowTouches(true);
    _listener->onTouchBegan = CC_CALLBACK_2(AboutLayer::touch_began, this);
    _listener->onTouchMoved = CC_CALLBACK_2(AboutLayer::touch_moved, this);
    _listener->onTouchEnded = CC_CALLBACK_2(AboutLayer::touch_ended, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

bool AboutLayer::touch_began(Touch* touch, Event* event){
    _position = touch->getLocation();
    return true;
}

void AboutLayer::touch_moved(Touch* touch, Event* event){
    auto touchLocation = touch->getLocation();
    float nMoveY = touchLocation.y - _position.y;
    
    auto curPos  = about->getPosition();
    auto nextPos = Vec2(curPos.x, curPos.y + nMoveY);
    
    if (nextPos.y < 0.0f)
    {
        about->setPosition(Vec2::ZERO);
        return;
    }
    
    about->setPosition(nextPos);
    _position = touchLocation;
}

void AboutLayer::touch_ended(Touch* touch, Event* event){
    auto touchLocation = touch->getLocation();
    if(exit && exit->getBoundingBox().containsPoint(touchLocation)){
        exit_with_animation();
        runAction(Sequence::create(DelayTime::create(1.1f), CallFunc::create([&]{
            removeFromParent();
        }),nullptr));
    }
    
}