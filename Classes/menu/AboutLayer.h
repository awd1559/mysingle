//
//  AboutLayer.h
//  my_single
//
//  Created by awd on 10/22/14.
//
//

#ifndef __my_single__AboutLayer__
#define __my_single__AboutLayer__

#include "cocos2d.h"

class AboutLayer : public cocos2d::Layer {
public:
    bool init();
    CREATE_FUNC(AboutLayer);
private:
    void enter_with_animation();
    void exit_with_animation();
    
    void add_touch_listener();
    bool touch_began(cocos2d::Touch* sender, cocos2d::Event* evnet);
    void touch_moved(cocos2d::Touch* sender, cocos2d::Event* event);
    void touch_ended(cocos2d::Touch* sender, cocos2d::Event* event);
    
    cocos2d::EventListenerTouchOneByOne* _listener;
    cocos2d::Vec2 _position;
    
    cocos2d::Node* _container;
    cocos2d::Sprite* exit;
    cocos2d::Sprite* about;
    cocos2d::LayerColor* bg;
};

#endif /* defined(__my_single__AboutLayer__) */
