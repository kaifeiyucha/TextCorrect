#include "lib.h"
#include <sstream>
#include <fstream>
#include <string>
#include <errno.h>
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define ERR_EXIT(m)\
    do{\
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

using namespace std;
using namespace CppJieba;

//const char* const dict_path = "dict/jieba.dict.utf8";
//const char* const model_path = "dict/hmm_model.utf8";

Lexicon::Lexicon(const string &filename, 
                 const string &setfile,
                 const char *dict_path,
                 const char *model_path)
    :segment_(dict_path, model_path)
    {
    read_charactor_set(setfile);
    read_file(filename);
    }   

void Lexicon::store_file(const string &filename){
    ifstream infile;
    infile.open(filename.c_str());
    if(!infile)
        ERR_EXIT("open file fail");

    string textline;
    while(getline(infile, textline)){
        lines_.push_back(textline);
    }
    infile.close();
}
/*
void Lexicon::formate_file(string &line){
    for(size_t i=0; i != line.size(); i++){
        for(set<string>::iterator it = exclude_.begin(); it != exclude_.end(); ++it){
            if(line[i] == (*it))
                line[i] = ' ';
            break;
        }
    }
}
*/

void Lexicon::build_map(){
    for(line_no ix = 0; ix != lines_.size(); ++ix){
//        MixSegment segment(dict_path, model_path);
        vector<string> words;
        segment_.cut(lines_[ix].c_str(), words);//把每行存储在vector中
        for(vector<string>::iterator it = words.begin(); it != words.end(); ++it){
            if(!exclude_.count(*it)) 
               words_count[*it]++;//把每个词存在map中    
            }
    }
}

void Lexicon::read_charactor_set(const string &filename){//读入符号集合
    ifstream infile;
    infile.open(filename.c_str());
    if(!infile){
        ERR_EXIT("CHARACTOR_SET ISN'T OPEN");
    }

    string word;
    while(infile >> word){
        exclude_.insert(word);
    }
    infile.close();
}


void Lexicon::printf_lexicon(){
    int fd = open("Chinese_lib.txt", O_WRONLY|O_CREAT, 0666);
    if(fd == -1)
        ERR_EXIT("open Chinese_lib.txt fail");
    close(STDOUT_FILENO);
   int fd1 = dup(fd);


    for(map<string, int>::iterator it = words_count.begin(); it != words_count.end(); ++it){
        cout << it->first <<"   "  << it->second << endl;    
    }

    close(fd1);
    close(fd);
}

