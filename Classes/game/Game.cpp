//
//  Game.cpp
//  my_single
//
//  Created by awd on 10/7/14.
//
//

#include "extensions/cocos-ext.h"
#include "Game.h"
#include "../data/GameDataManager.h"
#include "../menu/ResultLayer.h"
#include "../menu/BackgroundLayer.h"
#include "../menu/MainMenu.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum {
    INDEX_BACKGROUND,
    INDEX_CONTAINER,
    INDEX_ALERT
};

//run the Game
Scene* Game::createScene(){
    Scene* scene = Scene::create();
    
    BackgroundLayer* bg = BackgroundLayer::create();
    scene->addChild(bg);
    
    Game* game = Game::create();
    scene->addChild(game);
    
    return scene;
}

//init game layer
bool Game::init(){
    if(!Layer::init()){
        return false;
    }
    
    //TODO: test result layer
    //this->setOpacity(0.5f);
    //ResultLayer* result = ResultLayer::create();
    //addChild(result, INDEX_ALERT);
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    
    _width = (_visibleSize.width < _visibleSize.height ? _visibleSize.width : _visibleSize.height)/7;
    _width = 86;
    
    _container = Node::create();
    _container->setPosition((_visibleSize.width-_width*7)/2, (_visibleSize.height-_width*7)/2);
    addChild(_container, INDEX_CONTAINER);
    
    add_menu();
    
    reload_last_game_or_reset();
    addTouchListener();
    
    enter_with_animation();
    return true;
}


/////////////////////////////menu
enum {
    MENU_ITEM_PREV,
    MENU_ITEM_NEXT,
    MENU_ITEM_BACK,
    MENU_ITEM_NEW,
};

void Game::add_menu(){
    Size winSize = Director::getInstance()->getWinSize();
    
    float w = 150;
    float h = 130;
    float x = (winSize.width - 4 * w)/2 + w/2;
    float y = winSize.height - ((winSize.height - _width*7)/2 - h)/2 - h/2;
    
    MenuItemImage* prev = MenuItemImage::create("btn_prev.png", "btn_prev_press.png", CC_CALLBACK_1(Game::menu_callback, this));
    prev->setTag(MENU_ITEM_PREV);
    prev->setPosition(x, y);
    
    x += w;
    MenuItemImage* next = MenuItemImage::create("btn_next.png", "btn_next_press.png", CC_CALLBACK_1(Game::menu_callback, this));
    next->setTag(MENU_ITEM_NEXT);
    next->setPosition(x, y);
    
    x += w;
    MenuItemImage* newgame = MenuItemImage::create("btn_new.png", "btn_new_press.png", CC_CALLBACK_1(Game::menu_callback, this));
    newgame->setTag(MENU_ITEM_NEW);
    newgame->setPosition(x, y);
    
    x += w;
    MenuItemImage* back = MenuItemImage::create("btn_back.png", "btn_back_press.png", CC_CALLBACK_1(Game::menu_callback, this));
    back->setTag(MENU_ITEM_BACK);
    back->setPosition(x, y);
    
    _menu = Menu::create(prev, next, newgame, back, nullptr);
    _menu->setPosition(0, 0);
    addChild(_menu);
}

void Game::menu_callback(Ref* sender){
    MenuItem* item = (MenuItem*)sender;
    int tag = item->getTag();
    switch (tag) {
        case MENU_ITEM_NEXT:{
            Move next = GameDataManager::getInstance()->next_move();
            if(next.x1 >= 0){
                jump(next);
            }
        }
            break;
        case MENU_ITEM_PREV:{
            Move prev = GameDataManager::getInstance()->prev_move();
            if(prev.x1 >= 0){
                withdraw(prev);
            }
        }
            break;
        case MENU_ITEM_BACK:{
            exit_with_animation();
            MainMenu* game = MainMenu::create();
            getParent()->addChild(game);
            runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([&]{
                removeFromParent();
            }), nullptr));
        }
            break;
        case MENU_ITEM_NEW:{
            reflash();
        }
            break;
        default:
            break;
    }
}
//////////////////menu end
void Game::reflash(){
    exit_with_animation();
    auto action = CallFunc::create([&]{
        _container->removeAllChildren();
        GameDataManager::getInstance()->new_game();
        this->reset();
        this->enter_with_animation();
    });
    
    this->runAction(Sequence::create(DelayTime::create(1.2f), action, nullptr));
}


//////////////////touch begin
void Game::addTouchListener(){
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Game::TouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool Game::TouchBegan(Touch* touch, Event* evnet){
    Vec2 position = touch->getLocation();
    
    position = position - _container->getPosition();
    Vec2 touched_coord = getCoordForPosition(position.x, position.y);
    int tag = getTagForCoord(touched_coord);
    if(tag < 0) tag = 0;
    Sprite* touched_sprite = (Sprite*)_container->getChildByTag(tag);
    
    if(touched_sprite){
        if(_selected_star){
            _selected_star->setTexture("commonstar.png");
        }
        _selected_star = touched_sprite;
        _selected_star->setTexture("selectedstar.png");
        return true;
    }
    
    if(canJump(touched_coord)){
        jump(touched_coord);
        if(check_die()){
            die();
        }
    }
    
    return true;
}
/////////////////////////////////touch end


void Game::jump(Move move){
    if(_selected_star) {
        _selected_star->setTexture("commonstar.png");
        _selected_star = nullptr;
    }
    
    int x1 = move.x1;
    int y1 = move.y1;
    int x2 = move.x2;
    int y2 = move.y2;
    int xm = move.xm;
    int ym = move.ym;
    
    remove_sprite_at_coord(xm, ym);
    move_sprite(x1, y1, x2, y2);
    
    save_layout();
}

void Game::withdraw(Move move){
    if(_selected_star) {
        _selected_star->setTexture("commonstar.png");
        _selected_star = nullptr;
    }
    
    int x1 = move.x1;
    int y1 = move.y1;
    int x2 = move.x2;
    int y2 = move.y2;
    int xm = move.xm;
    int ym = move.ym;
    
    create_sprite_at_coord(xm, ym);
    move_sprite(x2, y2, x1, y1);
    
    save_layout();
}


void Game::save_layout(){
    int layout[7];
    for(int y = 0; y < 7; y++){
        layout[y] = 0;
        for(int x = 0; x < 7; x++){
            layout[y] = layout[y] << 1;
            if(_data[x][y] == TYPE::SET)
                layout[y] += 1;
        }
        GameDataManager::getInstance()->current_item.layout[y] = layout[y];
    }
}

void Game::reload_last_game_or_reset(){
    if(GameDataManager::getInstance()->current_item.moves.size() <= 0){
        reset();
        return;
    }
    
    _selected_star = nullptr;
    initBG();
    
    int len_of_layout = 7;
    for(int y = 0; y < len_of_layout; y++){
        int layout = GameDataManager::getInstance()->current_item.layout[y];
        for(int x = 6; x >= 0; x--){
            int state = layout & 0x1;
            if(state == 1){
                create_sprite_at_coord(x, y);
            } else {
                _data[x][y] = TYPE::EMPTY;
            }
            layout = layout >> 1;
            
            if((x == 0 || x == 1 || x == 5 || x == 6) && (y == 0 || y == 1 || y == 5 || y == 6)){
                _data[x][y] = TYPE::INVALID;
            }
        }
    }
}

void Game::reset(){
    _selected_star = nullptr;
    initBG();
    
    for(int x = 0; x < 7; x++){
        for(int y = 0; y < 7; y++){
            if((x == 0 || x == 1 || x == 5 || x == 6) && (y == 0 || y == 1 || y == 5 || y == 6)){
                _data[x][y] = TYPE::INVALID;
                continue;
            }
            if(x == 3 && y == 3) {
                _data[3][3] = TYPE::EMPTY;
                continue;
            }
            
            create_sprite_at_coord(x, y);
        }
    }
    save_layout();
}

void Game::create_sprite_at_coord(int x, int y){
    float scale = _width / 100 * 0.8;
    
    Sprite* sprite = Sprite::create("commonstar.png");
    sprite->setScale(scale);
    _container->addChild(sprite);

    sprite->setPosition(getPostionForCoord(x, y));
    sprite->setTag(getTagForCoord(x, y));
    sprite->setOpacity(0.0f);
    
    auto action = FadeIn::create(0.8f);
    sprite->runAction(action);
    _data[x][y] = TYPE::SET;
}

void Game::move_sprite(int srcx, int srcy, int destx, int desty){
    Sprite* src = (Sprite*)_container->getChildByTag(getTagForCoord(srcx, srcy));
    
    src->setPosition(getPostionForCoord(destx, desty));
    src->setTag(getTagForCoord(destx, desty));
    _data[srcx][srcy] = TYPE::EMPTY;
    _data[destx][desty] = TYPE::SET;
}

void Game::remove_sprite_at_coord(int x,int y){
    int tag = getTagForCoord(x, y);
    _container->removeChildByTag(tag);
    _data[x][y] = TYPE::EMPTY;
}

TYPE Game::get_type(int x, int y) {
    if(x < 0 || x >= 7) return TYPE::INVALID;
    if(y < 0 || y >= 7) return TYPE::INVALID;
    
    return _data[x][y];
}

Vec2 Game::getPostionForCoord(int x, int y) {
    Vec2 p(x*_width + _width/2, y*_width + _width/2);
    return p;
}

Vec2 Game::getCoordForPosition(float x, float y){
    int _x = x/_width;
    int _y = y/_width;
    Vec2 c(_x, _y);
    return c;
}

int Game::getTagForCoord(int x, int y){
    int tag = y * 7 + x;
    return tag;
}

int Game::getTagForCoord(Vec2 coord){
    return getTagForCoord((int)coord.x, (int)coord.y);
}

void Game::die(){
    show_result();
}

bool Game::check_die(){
    for(int x = 0; x < 7; x++){
        for(int y = 0; y < 7; y++){
            if(_data[x][y] == TYPE::SET) {
                //up
                if(get_type(x, y+1) == TYPE::SET && get_type(x, y+2) == TYPE::EMPTY) return false;
                //down
                if(get_type(x, y-1) == TYPE::SET && get_type(x, y-2) == TYPE::EMPTY) return false;
                //left
                if(get_type(x-1, y) == TYPE::SET && get_type(x-2, y) == TYPE::EMPTY) return false;
                //right
                if(get_type(x+1, y) == TYPE::SET && get_type(x+2, y) == TYPE::EMPTY) return false;
            }
        }
    }
    
    return true;
}

void Game::show_result(){
    ResultLayer* result = ResultLayer::create();
    addChild(result, INDEX_ALERT);
    
    int count = 0;
    for(int x = 0; x < 7; x++){
        for(int y = 0; y < 7; y++){
            if(_data[x][y] == TYPE::SET) {
                count++;
            }
        }
    }
    
    bool center = (count == 1) && (_data[3][3] == TYPE::SET);
    result->set_left_stars(count, center);
}

bool Game::canJump(Vec2 dest){
    if(!_selected_star) return false;
    
    if(_data[(int)dest.x][(int)dest.y] != TYPE::EMPTY) return false;
    
    int tag = _selected_star->getTag();
    int x1 = tag % 7;
    int y1 = tag / 7;
    int x2 = dest.x;
    int y2 = dest.y;
    int abs_x = abs(x1 - x2);
    int abs_y = abs(y1 - y2);
    
    if(abs_x == 2 && abs_y == 0) //horizental
    {
        if(_data[(x1+x2)/2][y1] == TYPE::SET) return true;
    }
    if(abs_x == 0 && abs_y == 2) //vertical
    {
        if(_data[x1][(y1+y2)/2] == TYPE::SET) return true;
    }
    
    return false;
}

void Game::jump(Vec2 dest){
    if(!_selected_star) return ;
    
    int tag = _selected_star->getTag();
    int x1 = tag % 7;
    int y1 = tag / 7;
    int x2 = dest.x;
    int y2 = dest.y;
    int xm = (x1 + x2) / 2;
    int ym = (y1 + y2) / 2;
    
    remove_sprite_at_coord(xm, ym);
    
    move_sprite(x1, y1, x2, y2);
    
    Move move(x1, y1, x2, y2);
    GameDataManager::getInstance()->add_mvoe(move);
    save_layout();
}

void Game::enter_with_animation(){
    if(_menu){
        _menu->setScale(3);
        auto action = ScaleTo::create(0.8, 1, 1);
        _menu->runAction(action);
    }
    if(_container){
        Size visiableSize = Director::getInstance()->getVisibleSize();
        Vec2 dest((_visibleSize.width-_width*7)/2, (_visibleSize.height-_width*7)/2);
        Vec2 src(_container->getPosition().x, visiableSize.height + visiableSize.height/2);
        auto action = Spawn::create(FadeIn::create(0.8f), EaseElasticOut::create(MoveTo::create(0.8, dest)), nullptr);
        
        _container->setPosition(src);
        _container->runAction(action);
    }
}


void Game::exit_with_animation(){
    if(_menu){
        auto action = ScaleTo::create(0.8, 3, 3);
        _menu->runAction(action);
    }
    if(_container){
        Size visiableSize = Director::getInstance()->getVisibleSize();
        Vec2 dest(_container->getPosition().x, -visiableSize.height);
        
        auto action = Spawn::create(FadeOut::create(0.8f), EaseElasticIn::create(MoveTo::create(0.8, dest)), nullptr);
        _container->runAction(action);
    }
}










//not used
void Game::compose_bg(){
     auto bg = Scale9Sprite::create("bg.9.png");
     addChild(bg);
     bg->setContentSize(Director::getInstance()->getVisibleSize());
     bg->setPosition(Director::getInstance()->getVisibleSize().width/2, Director::getInstance()->getVisibleSize().height/2);
     
     add_xi_line(1, 2, 1, 4);
     add_xi_line(2, 2, 2, 4);
     add_xi_line(3, 0, 3, 6);
     add_xi_line(4, 0, 4, 6);
     add_xi_line(5, 2, 5, 4);
     add_xi_line(6, 2, 6, 4);
     
     add_xi_line(2, 1, 4, 1);
     add_xi_line(2, 2, 4, 2);
     add_xi_line(0, 3, 6, 3);
     add_xi_line(0, 4, 6, 4);
     add_xi_line(2, 5, 4, 5);
     add_xi_line(2, 6, 4, 6);
     
     add_cu_line(2, 0, 4, 0);
     add_cu_line(5, 0, 5, 1);
     add_cu_line(5, 2, 7, 2);
     add_cu_line(7, 2, 7, 4);
     add_cu_line(5, 5, 7, 5);
     add_cu_line(5, 5, 5, 6);
     add_cu_line(2, 7, 4, 7);
     add_cu_line(2, 5, 2, 6);
     add_cu_line(0, 5, 1, 5);
     add_cu_line(0, 2, 0, 4);
     add_cu_line(0, 2, 1, 2);
     add_cu_line(2, 0, 2, 1);
}

void Game::add_cu_line(int x1, int y1, int x2, int y2){
    Vec2 offset = _container->getPosition();
    Vec2 src(x1, y1);
    Vec2 dest(x2, y2);
    auto e1 = Scale9Sprite::create("cu.9.png");
    e1->setContentSize(get_cu_size(src, dest));
    e1->setPosition(mid(src, dest) + offset);
    addChild(e1);
}
void Game::add_xi_line(int x1, int y1, int x2, int y2){
    Vec2 offset = _container->getPosition();
    Vec2 src(x1, y1);
    Vec2 dest(x2, y2);
    auto e1 = Scale9Sprite::create("xi.9.png");
    e1->setContentSize(get_xi_size(src, dest));
    e1->setPosition(mid(src, dest) + offset);
    addChild(e1);
}
Size Game::get_cu_size(Vec2 src, Vec2 dest){
    if(src.x == dest.x){
        return Size(8, abs(src.y-dest.y) * _width + _width + 8);
    }else if(src.y == dest.y){
        return Size( abs(src.x-dest.x) * _width + _width + 8, 8);
    }
    return Size::ZERO;
}
Size Game::get_xi_size(Vec2 src, Vec2 dest){
    if(src.x == dest.x){
        return Size(4, abs(src.y-dest.y) * _width + _width + 4);
    }else if(src.y == dest.y){
        return Size( abs(src.x-dest.x) * _width + _width + 4, 4);
    }
    return Size::ZERO;
}
Vec2 Game::mid(Vec2 v1, Vec2 v2){
    if(v1.x == v2.x){
        return Vec2((v1.x+v2.x)*_width/2, (v1.y+v2.y+1)*_width/2);
    }else if(v1.y == v2.y){
        return Vec2((v1.x+v2.x+1)*_width/2, (v1.y+v2.y)*_width/2);
    }
    return Vec2::ZERO;
}

void Game::initBG(){
    auto ui = Sprite::create("ui.png");
    ui->setPosition(_width * 3.5, _width * 3.5);
    _container->addChild(ui);
}





