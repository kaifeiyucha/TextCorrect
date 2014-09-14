#ifndef LEXICON_H
#define LEXICON_H 

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <cstdio>
#include "MixSegment.hpp"

using namespace CppJieba;

class Lexicon{
    public:
        typedef  std::vector<std::string>::size_type line_no;
           Lexicon(const std::string &filename,
                   const std::string &setfile,
                   const char *dict_path,
                   const char *model_path);

        void read_file(const std::string &filename){
            store_file(filename);
            build_map();
        }
        
        
        void printf_lexicon();//把词典写在c.txt中 

       


    private:
        void store_file(const std::string &);
        void build_map();
        void formate_file(std::string &);//格式化文章
        void read_charactor_set(const std::string &); //读入符号集合

        std::vector<std::string> lines_;//存放每行内容
        std::map<std::string, int > words_count;//存放单词和他们出现次数
        std::set<std::string> exclude_;//存放符号集合
        CppJieba::MixSegment segment_;        
};
#endif  /*LEXICON_H*/
