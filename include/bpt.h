#ifndef BPT_H
#define BPT_H


#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include<assert.h>

#ifndef UNIT_TEST
#include"predefined.h";
#else
//
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
    off_t slot;        /* where to store new block 储存的位置*/
    off_t root_offset; /* where is the root of internal nodes */
    off_t leaf_offset; /* where is the first leaf */
}meta_t;



//最后记录的数据结构
struct record_t{
    key_t key;
    value_t value;
};

//索引的数据结构
struct index_t{
    key_t key;
    off_t child;
};

//b+树的叶子节点结构,b+树叶子节点会储存数据
struct leaf_node_t{
    typedef record_t *child_t;

    //off_t 偏移量，此处为叶子节点的结构特征，不知有何深意。
    // 猜测是因为每段叶子节点的内存是连续的。加上偏移量，储存对应数据结构的地址。类似数组下标。
    off_t parent;
    off_t next;
    off_t prev;
    size_t n;   //叶子节点大小
    record_t children[BP_ORDER];    //叶子节点阶数
};

//b+树的内部节点，内部节点是根节点和叶子节点之间的节点
struct internal_node_t{
    typedef index_t *child_t;

    off_t parent; /* parent node offset */
    off_t next;
    off_t prev;
    size_t n; /* how many children */
    index_t children[BP_ORDER];

};

struct record_t {
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
        int search_range(key_t *left, const key_t &right,
                     value_t *values, size_t max, bool *next = NULL) const;
        meta_t get_meta() const{
            return meta;
    };

    #ifndef UNIT_TEAT
    private:
    #else
    public:
    #endif
        char path[512];     //自定义一个空的char[512]
        meta_t meta;

        //initial empty tree
        void init_from_enmty();

        off_t search_leaf(off_t index, const key_t &key) const;
        off_t search_leaf(const key_t &key) const{
            return search_leaf(search_leaf(key), key);
        }



        template<class T>
        int map(T *block, off_t offset) const{
            return map(block, offset, sizeof(T));
        }

        //自定义函数，将磁盘数据读入缓冲区block，返回值是读取成功的元素个数
        int map(void *block, off_t offset, size_t size) const{
            // cout << "使用了map的重载函数，还没有自定义完成" << endl;
            open_file();
            fseek(fd, offset, SEEK_SET);    //将文件读写指针设置到指定位置
            //read 1 个 元素 form fd to block ,每个元素大小为size, 返回成功读取元素的个数
            size_t rd = fread(block, size, 1, fd);  
            close_file();

            return rd - 1;
        }

        template<class T>
        int unmap(T *block, off_t offset){
            return unmap(block, offset, sizeof(T));
        }

        int map(void *block, off_t offset, size_t size) const{
            open_file();
            fseek(fd, offset, SEEK_SET);
            size_t wd = fwrite(block, size, 1, fd);
            close_file();

            return wd - 1;
        }

        //mutable修饰词永远可变，可以使得参数在常函数中也可以被修改
        //这里定义了一个文件描述符fd，以及文件被使用的次数fd_level，防止同时修改.类似智能指针的概念
        mutable FILE *fd;
        mutable int fd_level;

        void open_file(const char *mode = "rd+") const{
             //以rb+模式打开文件， 可以写在任意位置读写文件，
            if(fd_level == 0){
                fd = fopen(path, mode); //失败返回nullptr
            }
            fd_level++;
        }

        void close_file() const{
            //关闭文件
            if(fd_level == 1){
                fclose(fd);
            }
            fd_level--;
        }

        // alloc from disk
        off_t alloc(size_t size){
            off_t slot = meta.slot;
            meta.slot += size;
            return slot;
        }
        //alloc for leaf-node
        off_t alloc(leaf_node_t *leaf){
            leaf->n = 0;
            meta.leaf_node_num++;
            return alloc(sizeof(leaf_node_t));
        }
        //alloc for internal-node
        off_t alloc(internal_node_t *node){
            node->n = 1;
            meta.internal_node_num++;
            return alloc(sizeof(internal_node_t));
        }
    


};

}

#endif