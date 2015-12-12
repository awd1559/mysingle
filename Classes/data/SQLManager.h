//
//  SQLManager.h
//  my_single
//
//  Created by awd on 10/14/14.
//
//

#ifndef __my_single__SQLManager__
#define __my_single__SQLManager__

#include "sqlite3.h"
struct GameItem;

class SQLManager {
public:
    static SQLManager* getInstance();
    void init_tables();

    void open();
    void close();
    
    //插入一条记录
    void insert_or_update_item(GameItem item);
    //读取一条记录
    GameItem get_item_by_index(int index);
    //新成绩诞生
    bool new_record(GameItem item);
    //获取前20名记录
    std::vector<int> get_first_sorted_ids();
    std::vector<int> get_first_sorted_moves();
    
private:
    sqlite3* _db;
};
#endif /* defined(__my_single__SQLManager__) */
