//
//  Player.cpp
//  my_single
//
//  Created by awd on 10/8/14.
//
//

#include "Player.h"

USING_NS_CC;

Scene* Player::createScene(){
    auto scene = Scene::create();
    Player* player = Player::create();
    scene->addChild(player);
    
    return scene;
}

bool Player::init(){
    if(!Layer::init()){
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _width = (visibleSize.width < visibleSize.height ? visibleSize.width : visibleSize.height)/7;
    
    _container = Node::create();
    _container->setPosition((visibleSize.width-_width*7)/2, (visibleSize.height-_width*7)/2);
    addChild(_container);
    
    addTouchListener();
    
    return true;
}

void Player::addTouchListener(){
    _listener = EventListenerTouchOneByOne::create();
    _listener->onTouchBegan = CC_CALLBACK_2(Player::TouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

bool Player::TouchBegan(Touch* sender, Event* evnet){
    return true;
}




