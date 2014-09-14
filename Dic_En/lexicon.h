#ifndef LEXICON_H
#define LEXICON_H 

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <fstream>

class Lexicon{
    public:
        typedef  std::vector<std::string>::size_type line_no;

        Lexicon(const std::string &filename);

        void read_file(const std::string &filename){
            store_file(filename);
            build_map();
        }
        
        
        void printf_lexicon();//把词典写在b.txt中 

       


    private:
        void store_file(const std::string &);
        void build_map();
        void handle_punct(std::string &);

        std::vector<std::string> lines_;//存放每行内容
        std::map<std::string, int > words_count;//存放单词和他们出现次数

};
#endif  /*LEXICON_H*/
