//
//  Constants.h
//  my_game_template
//
//  Created by awd on 9/23/14.
//
//

#include "GameUtils.h"
USING_NS_CC;

bool GameUtils::init(){
    _exit_key_pressed = false;
    visibleSize = Director::getInstance()->getVisibleSize();
    _key_listener = EventListenerKeyboard::create();
    return true;
}

void GameUtils::app_exit(){
    cocos2d::Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void GameUtils::add_key_listener(IGameKeyDelegate* delegate){
    _exit_key_pressed = false;
    _key_delegate = delegate;
    
    _key_listener->onKeyReleased = [&](EventKeyboard::KeyCode keycode, Event* event){
        if(keycode != EventKeyboard::KeyCode::KEY_BACK)
            return;
        
        if(_exit_key_pressed){
            _exit_key_pressed = false;
            _key_delegate->second_key_pressed();
        }else{
            _exit_key_pressed = true;
            _key_delegate->first_key_pressed();

            this->scheduleOnce(schedule_selector(GameUtils::reset_key), 2.0f);
        }
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_key_listener, 10);
}

void GameUtils::remove_key_listener(){
    if(_key_listener){
        Director::getInstance()->getEventDispatcher()->removeEventListener(_key_listener);
    }
}

void GameUtils::reset_key(float t){
    _exit_key_pressed = false;
}



Label* debug(std::string message){
    Size _visibleSize = Director::getInstance()->getVisibleSize();
    Label* label = Label::createWithSystemFont("", "", 36);
    label->setPosition(_visibleSize.width/2, _visibleSize.height/2);
    label->setString(message);
    
    return label;
}
