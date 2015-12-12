//
//  BackgroundLayer.cpp
//  my_single
//
//  Created by awd on 10/13/14.
//
//

#include "BackgroundLayer.h"

USING_NS_CC;

bool BackgroundLayer::init() {
    if(!Layer::init()) {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    LayerColor* color = LayerColor::create(Color4B(17, 232, 245, 175));
    addChild(color);
    
    return true;
}