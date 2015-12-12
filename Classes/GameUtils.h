//
//  GameUtils.h
//  my_game_template
//
//  Created by awd on 9/23/14.
//
//

#ifndef _game_utils_h_
#define _game_utils_h_

#include "cocos2d.h"

#define IMG_PLAYER "player.png"

class IGameKeyDelegate{
public:
    virtual void first_key_pressed() = 0;
    virtual void second_key_pressed() = 0;
};


class GameUtils : public cocos2d::Node
{
public:
    static cocos2d::Label* debug();
    virtual bool init();
    void app_exit();
    void add_key_listener(IGameKeyDelegate* delegate);
    void remove_key_listener();
    CREATE_FUNC(GameUtils);
    
    cocos2d::Size visibleSize;
private:
    void reset_key(float t);
    
    bool _exit_key_pressed;
    cocos2d::EventListenerKeyboard* _key_listener;
    IGameKeyDelegate* _key_delegate;
};



#endif /* _game_utils_h_ */
