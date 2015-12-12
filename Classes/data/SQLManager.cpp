//
//  SQLManager.cpp
//  my_single
//
//  Created by awd on 10/14/14.
//
//

#include "cocos2d.h"
#include "SQLManager.h"
#include "GameDataManager.h"

USING_NS_CC;

static SQLManager* _instance;

SQLManager* SQLManager::getInstance(){
    if(!_instance){
        _instance = new SQLManager();
        _instance->open();
    }
    
    return _instance;
}

void SQLManager::init_tables(){
    sqlite3 *pdb = NULL;
    std::string path = FileUtils::getInstance()->getWritablePath() + "save.db";
    
    
    int result;
    result = sqlite3_open(path.c_str(), &pdb);
    if(result != SQLITE_OK){
        CCLOG("open db failed, errno:%s", sqlite3_errmsg(pdb));
    }
    
    std::string sql = "create table if not exists records(ID integer primary key autoincrement DEFAULT NULL, layout Varchar(7), moves Varchar(1000), move_count integer)";
    result = sqlite3_exec(pdb, sql.c_str(), NULL, NULL, NULL);
    if(result != SQLITE_OK){
        CCLOG("create table failed, errno:%s", sqlite3_errmsg(pdb));
    }
    
    sqlite3_close(pdb);
}

void SQLManager::open(){
    std::string path= FileUtils::getInstance()->getWritablePath() + "save.db";
    int result;
    result = sqlite3_open(path.c_str(), &_db);
    if(result != SQLITE_OK)
    {
        CCLOG("open database failed, errno:%s", sqlite3_errmsg(_db));
    }
}

void SQLManager::close(){
    if(_db){
        sqlite3_close(_db);
        _db = nullptr;
    }
}


int score(unsigned char* layout){
    int count = 0;
    bool center = false;
    for(int y = 0; y < 7; y++){
        int ll = (layout[y] & 0xff);
        for(int x = 6; x >= 0; x--){
            int state = ll & 0x1;
            if(state == 1){
                if(x == 3 && y == 3) center = true;
                count++;
            }
            ll = ll >> 1;
        }
    }
    
    if(center && count == 1)
        return 100;
    else
        return count;
}


//插入一条记录
void SQLManager::insert_or_update_item(GameItem item){
    int len = item.moves.size() * 2;
    unsigned char* moves = new unsigned char[len];
    unsigned char* layout = new unsigned char[7];
    
    int len_of_layout = 7;
    for(int i = 0; i < len_of_layout; i++){
        layout[i] = (char)(item.layout[i] & 0xff);
    }
    
    int len_of_moves = item.moves.size();
    for(int j = 0; j < len_of_moves; j++){
        Move move = item.moves.at(j);
        moves[j*2]   = (char)(( (move.x1 & 0xf) << 4) | (move.y1 & 0xf));
        moves[j*2+1] = (char)(( (move.x2 & 0xf) << 4) | (move.y2 & 0xf));
    }
    
    sqlite3_stmt* stmt;
    if(item.id >= 0) {
        //update
        std::string sql = "update records set layout = :layout, moves = :moves, move_count = :move_count where ID = :ID";
        int ret = sqlite3_prepare(_db, sql.c_str(), sql.length(), &stmt, NULL);
        
        ret = sqlite3_bind_blob(stmt, 1, layout, len_of_layout, NULL);
        
        ret = sqlite3_bind_blob(stmt, 2, moves, len_of_moves*2, NULL);
        
        ret = sqlite3_bind_int(stmt, 3, score(layout));
        
        ret = sqlite3_bind_int(stmt, 4, item.id);
    } else{
        //insert
        std::string sql = "insert into records values(NULL, :layout, :moves, :move_count)";
        int ret = sqlite3_prepare(_db, sql.c_str(), sql.length(), &stmt, NULL);
        
        ret = sqlite3_bind_blob(stmt, 1, layout, len_of_layout, NULL);
        
        ret = sqlite3_bind_blob(stmt, 2, moves, len_of_moves*2, NULL);
        
        ret = sqlite3_bind_int(stmt, 3, item.moves.size());
    }
    int result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) CCLOG("error insert item:%s", sqlite3_errmsg(_db));
}

//读取一条记录
GameItem SQLManager::get_item_by_index(int index){
    sqlite3_stmt* stmt;
    std::string sql = "select ID, layout, moves, move_count from records where ID = :DDD";
    int ret = sqlite3_prepare(_db, sql.c_str(), sql.length(), &stmt, NULL);
    ret = sqlite3_bind_int(stmt, 1, index);
    
    GameItem item;
    ret = sqlite3_step(stmt);
    if(ret != SQLITE_ROW) {
        CCLOG("read item error:%s", sqlite3_errmsg(_db));
        sqlite3_finalize(stmt);
        return item;
    }
    
    item.id = sqlite3_column_int(stmt, 0);
    const char* read_layout = (const char*)sqlite3_column_blob(stmt, 1);
    const char* read_moves = (const char*)sqlite3_column_blob(stmt, 2);
    
    for(int i = 0; i < 7; i++){
        item.layout[i] = (read_layout[i] & 0xff);
    }
    
    int move_count = sqlite3_column_int(stmt, 3);
    for(int i = 0; i < move_count;i++){
        char from = read_moves[i*2];
        char to   = read_moves[i*2+1];
        
        int x1 = (from >> 4) & 0xf;
        int y1 = from & 0xf;
        int x2 = (to >> 4) & 0xf;
        int y2 = to & 0xf;
        Move m(x1, y1, x2, y2);
        item.moves.push_back(m);
    }
    
    sqlite3_finalize(stmt);
    return item;
}

//新成绩诞生
bool SQLManager::new_record(GameItem item){
    int current_move = item.moves.size();
    
    std::string sql = "select ID, move_count from records";
    sqlite3_stmt* stmt;
    int ret = sqlite3_prepare(_db, sql.c_str(), sql.length(), &stmt, NULL);
    
    while(SQLITE_ROW == sqlite3_step(stmt)){
        int count = sqlite3_column_int(stmt, 1);
        if(current_move > count) return false;
    }
    return true;
}

//获取前20名记录
std::vector<int> SQLManager::get_first_sorted_ids(){
    std::vector<int> result;
    
    std::string sql = "select ID from records order by move_count desc limit 0, 20";
    sqlite3_stmt* stmt;
    int ret = sqlite3_prepare(_db, sql.c_str(), sql.length(), &stmt, NULL);
    
    while(SQLITE_ROW == sqlite3_step(stmt)){
        int id = sqlite3_column_int(stmt, 0);
        result.push_back(id);
    }
    
    return result;
}

std::vector<int> SQLManager::get_first_sorted_moves(){
    std::vector<int> result;
    
    std::string sql = "select move_count from records order by move_count desc limit 0, 20";
    sqlite3_stmt* stmt;
    int ret = sqlite3_prepare(_db, sql.c_str(), sql.length(), &stmt, NULL);
    
    while(SQLITE_ROW == sqlite3_step(stmt)){
        int moves = sqlite3_column_int(stmt, 0);
        result.push_back(moves);
    }
    
    return result;
}



