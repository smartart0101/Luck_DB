#ifndef BPT_H
#define BPT_H


#include<stdio.h>

#ifndef UNIT_TEST
#include"predefined.h";
#endif

namespace bpt{

//b+树相关信息的集合
typedef struct{
    size_t order; /* `order` of B+ tree */
    size_t value_size; /* size of value */
    size_t key_size;   /* size of key */
    size_t internal_node_num; /* how many internal nodes */
    size_t leaf_node_num;     /* how many leafs */
    size_t height;            /* height of tree (exclude leafs) */
    off_t slot;        /* where to store new block */
    off_t root_offset; /* where is the root of internal nodes */
    off_t leaf_offset; /* where is the first leaf */
}meta_t;

//封装b+树，
class bplus_tree{
    public:
        //构造函数
        bplus_tree(const char *path, bool force_empty = false);

        int insert(const key_t &key, value_t value);
        int remove(const key_t &key);
        int update(const key_t &key, value_t value);
        meta_t get_meta() const{
            return meta;
    };

    #ifndef UNIT_TEAT
    private:
    #else
    public:
    #endif
        char path[512];
        meta_t meta;

};

}

#endif