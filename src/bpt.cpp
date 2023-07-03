
#include "../include/bpt.h"
#include<stdlib.h>
#include<list>

#include <algorithm>

using std::lower_bound;


namespace bpt{

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

}