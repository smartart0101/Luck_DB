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

        void add(std::string const &content){
            _current.push_back(content);
        }

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