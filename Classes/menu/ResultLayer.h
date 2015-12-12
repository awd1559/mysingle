//
//  ResultLayer.h
//  my_single
//
//  Created by awd on 10/8/14.
//
//

#ifndef __my_single__ResultLayer__
#define __my_single__ResultLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class ResultLayer : public cocos2d::Layer{
public:
    bool init();
    CREATE_FUNC(ResultLayer);
    void set_left_stars(int stars, bool center);
private:
    virtual ~ResultLayer();
    void addTouchListener();
    bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* evnet);
    void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void enter_with_animation();
    void exit_with_animation();
    
    cocos2d::Sprite* bg;
    int _left_stars;
    cocos2d::Label* _label;
    cocos2d::EventListenerTouchOneByOne* _listener;
    cocos2d::Node* _container;
};

#endif /* defined(__my_single__ResultLayer__) */
