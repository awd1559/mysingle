//
//  RecordList.h
//  my_single
//
//  Created by awd on 10/13/14.
//
//

#ifndef __my_single__RecordList__
#define __my_single__RecordList__

#include "cocos2d.h"

class RecordList : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    
    bool init();
    CREATE_FUNC(RecordList);
private:
    void enter_with_animation();
    void exit_with_animation();
    
    void touchevent(cocos2d::Ref *sender, cocos2d::extension::Control::EventType controlEvent);
    void menu_callback(cocos2d::Ref* sender);
    
    cocos2d::Node* _container;
};

#endif /* defined(__my_single__RecordList__) */
