//
//  MainMenu.h
//  my_single
//
//  Created by awd on 10/13/14.
//
//

#ifndef __my_single__MainMenu__
#define __my_single__MainMenu__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "GameUtils.h"

class MainMenu : public cocos2d::Layer, public IGameKeyDelegate{
public:
    static cocos2d::Scene* createScene();
    static MainMenu* create(bool bootup);
    bool init();
    
    CREATE_FUNC(MainMenu);
public:
    virtual ~MainMenu();
    //game key delegate
    virtual void first_key_pressed();
    virtual void second_key_pressed();
private:
    GameUtils* _utils;
    
private:
    void menu_callback(cocos2d::Ref* sender);
    void enter_with_animation();
    void exit_with_animation();
    
private:
    cocos2d::extension::Scale9Sprite* green;
    cocos2d::extension::Scale9Sprite* blue;
    cocos2d::extension::Scale9Sprite* pink;
    cocos2d::extension::Scale9Sprite* yellow;
    cocos2d::Sprite* logo;
public:
    bool bootup;
};

#endif /* defined(__my_single__MainMenu__) */
