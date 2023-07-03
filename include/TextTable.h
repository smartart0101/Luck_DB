#include <iostream>
#include <vector>
#include <string>

class TextTable{

    public:
        typedef std::vector<std::string> Row;
        TextTable(char horizontal = '-', char vertical = '+', char corner = '+') :
            _horizontal(horizontal),
            _vertical(vertical),
            _corner(corner)
        {}
        //add，参数是string
        void add(std::string const &content){
            _current.push_back(content);
        }
        //结束某一行，但不理解为啥要加上
        void endOfRow(){
            _Rows.push_back(_current);
            _current.assign(0, "");  // str1.assign(str2)， str2->str1
        }



    private:
        char _horizontal;
        char _vertical;
        char _corner;
        Row _current;
        std::vector<Row> _Rows;
};