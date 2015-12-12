//
//  Player.h
//  my_single
//
//  Created by awd on 10/8/14.
//
//

#ifndef __my_single__Player__
#define __my_single__Player__

#include "cocos2d.h"

class Player : cocos2d::Layer{
public:
    cocos2d::Scene* createScene();
    bool init();
    CREATE_FUNC(Player);
private:
    void addTouchListener();
    bool TouchBegan(cocos2d::Touch* sender, cocos2d::Event* evnet);

    cocos2d::Sprite* _selected_star;
    float _width;
    cocos2d::Node* _container;
    cocos2d::EventListenerTouchOneByOne* _listener;
};

#endif /* defined(__my_single__Player__) */
