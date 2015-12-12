//
//  Game.h
//  my_single
//
//  Created by awd on 10/7/14.
//
//

#ifndef __my_single__Game__
#define __my_single__Game__

#include "cocos2d.h"
#include "../data/GameDataManager.h"

enum class TYPE{
    INVALID,
    EMPTY,
    SET
};

class Game : public cocos2d::Layer{
public:
    static cocos2d::Scene* createScene();
    bool init();
    
    CREATE_FUNC(Game);
    void reflash();
    
private:
    void add_menu();
    void menu_callback(cocos2d::Ref* sender);
    
    
    void addTouchListener();
    bool TouchBegan(cocos2d::Touch* sender, cocos2d::Event* evnet);
    
    void jump(Move next);
    void withdraw(Move prev);
    void save_layout();
    
    
    //util
    void reload_last_game_or_reset();
    void reset();
    
    void create_sprite_at_coord(int x, int y);
    void move_sprite(int srcx, int srcy, int destx, int desty);
    void remove_sprite_at_coord(int x, int y);
    
    TYPE get_type(int x, int y);
    cocos2d::Vec2 getPostionForCoord(int x, int y);
    cocos2d::Vec2 getCoordForPosition(float x, float y);
    int getTagForCoord(int x, int y);
    int getTagForCoord(cocos2d::Vec2 coord);
    
    void die();
    bool check_die();
    void show_result();
    bool canJump(cocos2d::Vec2 dest);
    void jump(cocos2d::Vec2 dest);
    
    void enter_with_animation();
    void exit_with_animation();
    
    
    //not used
    void compose_bg();
    void add_cu_line(int x1, int y1, int x2, int y2);
    void add_xi_line(int x1, int y1, int x2, int y2);
    cocos2d::Size get_cu_size(cocos2d::Vec2 src, cocos2d::Vec2 dest);
    cocos2d::Size get_xi_size(cocos2d::Vec2 src, cocos2d::Vec2 dest);
    cocos2d::Vec2 mid(cocos2d::Vec2 v1, cocos2d::Vec2 v2);
    void initBG();
    
    
    TYPE _data[7][7];
    cocos2d::Sprite* _selected_star;
    float _width;
    cocos2d::Size _visibleSize;
    cocos2d::Node* _container;
    cocos2d::Menu* _menu;
};

#endif /* defined(__my_single__Game__) */
