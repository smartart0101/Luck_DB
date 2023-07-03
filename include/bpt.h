#ifndef BPT_H
#define BPT_H


#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include<assert.h>

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

//b+树的叶子节点结构
struct leaf_node_t{
    typedef record_t *child_t;

    //off_t 偏移量，此处为叶子节点的结构特征，不知有何深意。
    // 猜测是因为每段叶子节点的内存是连续的。加上偏移量，储存对应数据结构的地址。类似数组下标。
    off_t parent;
    off_t next;
    off_t prev;
    size_t n;   //叶子节点大小
    record_t children[BP_OEDER];    //叶子节点
};

//最后记录的数据结构
struct record_t{
    key_t key;
    value_t value;
};

//封装b+树，
class bplus_tree{
    public:
        //构造函数
        bplus_tree(const char *path, bool force_empty = false);

        int insert(const key_t &key, value_t value);
        int remove(const key_t &key);
        int update(const key_t &key, value_t value);

        int search(const key_t& key, value_t *value) const;
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

        off_t search_leaf(off_t index, const key_t &key) const;
        off_t search_leaf(const key_t &key) const{
            return search_leaf(search_leaf(key), key);
        }



        template<class T>
        int map(T *block, off_t offset) const{
            return map(block, offset, sizeof(T));
        }

        int map(void *block, off_t offset, size_t size) const{
            cout << "使用了map的重载函数，还没有自定义完成" << endl;
        }
};

}

#endif