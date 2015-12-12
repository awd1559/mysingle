//
//  GameDataManager.h
//  my_single
//
//  Created by awd on 10/12/14.
//
//

#ifndef __my_single__GameDataManager__
#define __my_single__GameDataManager__

#include "cocos2d.h"

struct Move {
    Move(){
        x1 = x2 = xm = y1 = y2 = ym = 0;
    }
    Move(int _x1, int _y1, int _x2, int _y2){
        x1 = _x1, y1 = _y1, x2 = _x2, y2 = _y2;
        xm = (x1 + x2) / 2;
        ym = (y1 + y2) / 2;
    }
    int x1, y1, x2, y2;
    int xm, ym;
};
struct GameItem {
    int layout[7];
    int id;
    std::vector<Move> moves;
};

class GameDataManager {
public:
    static GameDataManager* getInstance();
    void save();
    void load();
    void load_by_id(int id);
    
    Move prev_move();
    Move next_move();
    void add_mvoe(Move m);
    void new_game();
    void finish_game();
    
    //GameItem _items[20];
    GameItem current_item;
private:
    GameDataManager();
    virtual ~GameDataManager();
    cocos2d::Data item_to_data(GameItem item);
    GameItem data_to_item(cocos2d::Data data);
    
    int _current_move_index;
};

#endif /* defined(__my_single__GameDataManager__) */
