
#include "../include/bpt.h"
#include<stdlib.h>
#include<list>

#include <algorithm>

using std::lower_bound;


namespace bpt{

#define OFFSET_META 0       //定义元数据偏移量
#define OFFSET_BLOCK OFFSET_META + sizeof(meta_t)       //定义元数据偏移量


    //自定义每个节点的开头
    template<class T>
    inline typename T::child_t begin(T &node){
        return node.children();
    }

    template<class T>
    inline typename T::child_t end(T &node){
        return node.children() + node.n;
    }

    //封装std::lower_bound,在升序容器中查找，返回第一个>= key值的位置，若查不到，返回end()
    inline record_t *find(leaf_node_t &node, const key_t &key){
        return lower_bound(begin(node), end(node), key);
    }

    //构造函数
    bplus_tree::bplus_tree(const char *p, bool force_empty)
        : fd(nullptr), fd_level(0)
    {
        bzero(path, sizeof(path));  // Set N bytes of path to 0.
        strcpy(path, p);    //p to path

        if(force_empty == false){   //不为空
            if(map(&meta, OFFSET_META) != 0){
                force_empty = true;
            }
        }

        if(force_empty == true){
            open_file("w+");

            //create empty tree if file doesn't exist
            init_from_enmty();
            close_file();
        }

    }

    //搜索
    int bplus_tree::search(const key_t &key, value_t *value) const{
        leaf_node_t leaf;
        map(&leaf, search_leaf(key));

        //查找数据
        record_t *record = find(leaf, key);

        if(record != leaf.children + leaf.n){   //查到了
            *value = record->value;

            return keycmp(record->key, key);    //返回查到数据和已有数据的差别
        }else{
            return -1;
        }
    }

    //范围搜索
    int bplus_tree::search_range(key_t *left, const key_t &right,
                                value_t * values, size_t max, bool *next) const
    {
        if()
    }

    void bplus_tree::init_from_enmty(){

        //init default meta
        // bzero(&meta, sizeof(meta_t));
        //memset(&meta, 0, sizeof(meta)); sizeof(meta) 得到的是指针的大小
        memset(&meta, 0, sizeof(meta_t));    //memset 省去把每一个结构体变量初始化为0的步骤
        meta.order = BP_ORDER;
        meta.value_size = sizeof(value_t);
        meta.key_size = sizeof(key_t);
        meta.height = 1;
        meta.slot = OFFSET_BLOCK;

        //init root node
        internal_node_t root;
        root.next = root.prev = root.parent = 0;

        //init empty treaf 
        leaf_node_t leaf;
        leaf.next = leaf.prev = 0;
        leaf.parent = meta.root_offset;
        meta.leaf_offset = root.children[0].child = alloc(&leaf);

        //save
        unmap(&meta, OFFSET_META);
        unmap(&meta, meta.root_offset);
        unmap(&meta, root.children[0].child);

    }

}