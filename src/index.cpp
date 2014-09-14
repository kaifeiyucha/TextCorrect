#include "index.h"
#include <string>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <cstdint>
/////////////////////////////////////////zhieshigaide///////////////
using namespace std;

#define ERR_EXIT(m)\
    do{\
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

/*Index::Index(const string &filename){
    open_file(filename);
}*/

void Index::open_file(const string &filename){
    ifstream infile;
    infile.open(filename.c_str());
    if(!infile)
        ERR_EXIT("open file b.txt fail");
    
    store_file(infile);
   // build_map();
    build_index();

    infile.close();
}
//测试函数
void Index::printall() const {
    for(auto &item: words_text_){
        cout << item << endl;    
    }
    for(auto &item: word_map_){
        cout << item.first << " ";
            for(auto &item2: item.second){
                   cout << item2 << " ";
            }
        cout << endl;
    }
}


void Index::store_file(ifstream &infile){
   string line;
   string word;
   while(getline(infile, line)){
       char buf[100];
       sscanf(line.c_str(), "%s",buf);
       word = buf;
       words_text_.push_back(word);
   }
}


void Index::build_index(){
    for(line_no num = 0; num < words_text_.size(); num++){
       string temp = words_text_[num];
       vector<uint32_t> vec;
       //将string转化成uint32_t 类型的数组
       to_stringUTF8(temp, vec);
       
       for(vector<uint32_t>::size_type i =0; i != vec.size(); ++i){
       word_map_[ vec[i] ].insert(temp);
        }
    }
}


set<string> Index::run_index(const string &query_word){
      vector<uint32_t> vec;
      string word = query_word;
      
      to_stringUTF8(word, vec);

      map<uint32_t, set<string> >::iterator MapIter;
      set<string> Union;
      for(vector<uint32_t>::size_type i = 0; i<vec.size(); ++i){
      MapIter = word_map_.find(vec[i]);
      vector<string>  temp;
      set_union(MapIter->second.begin(), MapIter->second.end(), Union.begin(), Union.end(), back_inserter(temp));
     
      Union = set<string>(temp.begin(), temp.end());
      }
      return Union;
}


//这是计算string占几位
int Index::get_len_UTF8(unsigned char c){
    int cnt = 0;
    while(c & (1 << (7-cnt))){
        ++cnt;
    }
    return cnt;
}

//把字符串解析成uint32_t 的数组
void Index::to_stringUTF8(const string &s, vector<uint32_t> &vec){
    vec.clear();
    for(string::size_type i = 0; i != s.size(); i++){
        int len = get_len_UTF8(s[i]);
        uint32_t t = (unsigned char)s[i];
        if(len > 1 ){
            --len;
            //拼接字符串
            while(len){
                t = (t << 8)+(unsigned char)s[++i];
                len--;
            }
        }
        vec.push_back(t);
    }
}




