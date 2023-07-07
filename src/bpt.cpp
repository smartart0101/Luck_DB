
#include "../include/bpt.h"
#include<stdlib.h>
#include<list>

#include <algorithm>

using std::lower_bound;     //fist >=
using std::upper_bound;     //fist >
using std::binary_search;



namespace bpt{

#define OFFSET_META 0       //定义元数据偏移量
#define OFFSET_BLOCK OFFSET_META + sizeof(meta_t)       //定义元数据偏移量


    //自定义每个节点的开头,返回该节点的子节点
    template<class T>
    inline typename T::child_t begin(T &node){
        return node.children();
    }
    //自定义每个节点的结束，返回该节点子节点 + 
    template<class T>
    inline typename T::child_t end(T &node){
        return node.children() + node.n;
    }

    //封装std::lower_bound,在升序容器：叶子节点中查找，返回第一个>= key值的位置，若查不到，返回end()
    inline record_t *find(leaf_node_t &node, const key_t &key){
        return lower_bound(begin(node), end(node), key);
    }
    //查找内部节点，
    inline index_t *find(internal_node_t &node, const key_t &key){
        return upper_bound(begin(node), end(node) - 1, key);
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

    //范围搜索，但是为什么一个指针参数，一个引用参数
    //这里分别传入左右范围，查找的内容，
    int bplus_tree::search_range(key_t *left, const key_t &right,
                                value_t * values, size_t max, bool *next) const
    {
        //首先判断给定的两个范围是否正确
        if(left == nullptr || keycmp(*left, right) > 0){
            return -1;
        }

        off_t off_left = search_leaf(*left);
        off_t off_right = search_leaf(right);
        off_t off = off_left;
        size_t i = 0;
        record_t *begin_point, *end_point;

        leaf_node_t leaf;
        while(off != off_right && off != 0 && i < max){
            //从磁盘中读出数据
            map(&leaf, off);    //read from off to sizeof(leaf_node_t)

            if(off_left == off){
                begin_point = find(leaf, *left);
            }else{
                begin_point = begin(leaf);
            }

            //copy,从自定义的开头到自定义的结尾
            end_point = leaf.children + leaf.n;
            for(; begin_point != end_point && i < max; begin_point++, i++){
                values[i] = begin_point->value;
            }

            off = leaf.next;
        }

        //while 循环结束，values内有了值
        if(i < max){
            map(&leaf, off_right);
            begin_point = find(leaf, *left);
            end_point = upper_bound(begin(leaf), end(leaf), right);

            for(;begin_point != end_point && i < max; begin_point++, i++){
                values[i] = begin_point->value;
            }
        }

        //标记next迭代器
        if(next != nullptr){
            if(i == max && begin_point != end_point){
                *next = true;
                *left = begin_point->key;
            }else{
                *next = false;
            }
        }
        return i;
    }

    int bplus_tree::remove(const key_t &key){

        internal_node_t parent;
        leaf_node_t leaf;
        //find parent node
        off_t parent_off = search_index(key);
        map(&parent, parent_off);
        //find current node
        index_t *where = find(parent, key);
        off_t offset = where->child;
        map(&leaf, offset);

        //验证
        if(!binary_search(begin(leaf), end(leaf), key)){
            return -1;
        }
        //
        size_t min_n = meta.leaf_node_num == 1 ? 0 : meta.order / 2;
        assert(leaf.n >= min_n && leaf.n <= meta.order);

        //delete key
        record_t *to_delete = find(leaf, key);
        std::copy(to_delete + 1, end(leaf), to_delete);
        leaf.n--;

        //合并或借
        if(leaf.n < min_n){
            bool borrowed = borrow_key(false, leaf);
        }

        


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

    off_t bplus_tree::search_index(const key_t &key) const
    {
        //内部节点的根节点
        off_t org = meta.root_offset;
        int height = meta.height;
        //循环里面，重复寻找每层的索引，除了叶子节点
        while(height > 1){      //为什么
            internal_node_t node;
            map(&node, org);
            
            index_t *i = upper_bound(begin(node), end(node) - 1, key);
            org = i->child;
            height--;
        }
        return org; //没有找到应该返回-1
    }

    bool bplus_tree::borrow_key(bool from_right, leaf_node_t &borrower){
        //计算偏移量  可是lender_off 不是空值吗？
        off_t lender_off = from_right ? borrower.next : borrower.prev;
        leaf_node_t lender;

        map(&lender, lender_off);

        assert(lender.n >= meta.order / 2);
        if(lender.n != meta.order / 2){
            typename leaf_node_t::child_t where_to_lend, where_to_put;

            //决定偏移量和
            if(from_right){
                where_to_lend = begin(lender);
            }
        }


        return false;
    }



}