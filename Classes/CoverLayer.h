#ifndef __COVER_LAYER_H__
#define __COVER_LAYER_H__

#include "cocos2d.h"
#include "GameUtils.h"

class CoverLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(CoverLayer);
private:
    virtual ~CoverLayer();
    void gotoMain(float dt);
    void add_touch_lisener();
    bool touch_began(cocos2d::Touch* sender, cocos2d::Event* evnet);
    
    cocos2d::EventListenerTouchOneByOne* _listener;
};

#endif // __COVER_H__
