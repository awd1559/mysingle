//
//  GameDataManager.cpp
//  my_single
//
//  Created by awd on 10/12/14.
//
//

#include "GameDataManager.h"
#include "SQLManager.h"

USING_NS_CC;

static GameDataManager* _instance;

GameDataManager* GameDataManager::getInstance(){
    if(!_instance){
        _instance = new GameDataManager();
    }
    return _instance;
}

GameDataManager::GameDataManager(){
    load();
}

GameDataManager::~GameDataManager(){
    save();
}

void GameDataManager::save(){
    Data data = item_to_data(current_item);
    UserDefault::getInstance()->setDataForKey("last", data);
    UserDefault::getInstance()->setIntegerForKey("id", current_item.id);
    UserDefault::getInstance()->setIntegerForKey("last_index", _current_move_index);
    UserDefault::getInstance()->flush();
}
void GameDataManager::load(){
    Data current = UserDefault::getInstance()->getDataForKey("last");
    current_item = data_to_item(current);
    current_item.id = UserDefault::getInstance()->getIntegerForKey("id", -1);
    _current_move_index = UserDefault::getInstance()->getIntegerForKey("last_index", -1);
}

void GameDataManager::load_by_id(int id){
    current_item = SQLManager::getInstance()->get_item_by_index(id);
    _current_move_index = current_item.moves.size()-1;
}

void GameDataManager::new_game(){
    current_item.id = -1;
    current_item.layout[0] = 0x1c;
    current_item.layout[1] = 0x1c;
    
    current_item.layout[2] = 0x7f;
    current_item.layout[3] = 0x77;
    current_item.layout[4] = 0x7f;
    
    current_item.layout[5] = 0x1c;
    current_item.layout[6] = 0x1c;
    
    current_item.moves.clear();
    _current_move_index = -1;
    save();
}

void GameDataManager::finish_game(){
    SQLManager::getInstance()->insert_or_update_item(current_item);
    
    new_game();
}

Move GameDataManager::prev_move(){
    if(current_item.moves.size() > 0 && _current_move_index >= 0){
        Move m = current_item.moves[_current_move_index];
        _current_move_index--;
        save();
        return m;
    }
    Move move;
    move.x1 = -1;
    return move;
}

Move GameDataManager::next_move(){
    if(current_item.moves.size() > 0 && _current_move_index < current_item.moves.size()){
        _current_move_index++;
        save();
        return current_item.moves[_current_move_index];
    }
    Move move;
    move.x1 = -1;
    return move;
}

void GameDataManager::add_mvoe(Move move){
    std::size_t index = _current_move_index;
    current_item.moves.resize(index+1);
    current_item.moves.push_back(move);
    _current_move_index++;
    save();
}

Data GameDataManager::item_to_data(GameItem item){
    int len = 7 + item.moves.size() * 2;
    unsigned char* bytes = new unsigned char[len];
    
    int len_of_layout = 7;
    for(int i = 0; i < len_of_layout; i++){
        bytes[i] = (char)(item.layout[i] & 0xff);
    }
    
    int len_of_moves = item.moves.size();
    for(int j = 0; j < len_of_moves; j++){
        Move move = item.moves.at(j);
        bytes[j*2+7]   = (char)((move.x1 & 0xf << 4) | (move.y1 & 0xf));
        bytes[j*2+7+1] = (char)((move.x2 & 0xf << 4) | (move.y2 & 0xf));
    }
    
    Data data;
    data.copy(bytes, len);
    return data;
}

GameItem GameDataManager::data_to_item(Data data){
    GameItem item;
    unsigned char* bytes = data.getBytes();
    if(bytes == nullptr){
        return item;
    }
    
    int len_of_layout = 7;
    for(int i = 0; i < len_of_layout; i++){
        item.layout[i] = (int)(bytes[i]);
    }
    
    int len_of_moves = (data.getSize() - len_of_layout) / 2;
    for(int j = 0; j < len_of_moves; j++){
        Move move;
        move.x1 = bytes[j*2+7] >> 4;
        move.y1 = bytes[j*2+7] &  0xf;
        move.x2 = bytes[j*2+8] >> 4;
        move.y2 = bytes[j*2+8] &  0xf;
        move.xm = (move.x1 + move.x2) / 2;
        move.ym = (move.y1 + move.y2) / 2;
        
        item.moves.push_back(move);
    }
    return item;
}










