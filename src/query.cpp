#include "query.h"
#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "index.h"
#include "Cache.h"
//#include "CacheManager.h"

using namespace std;

#define ERR_EXIT(m)\
    do{\
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

Query::Query(const string &dicEn, const string &dicCh)
    :dicEn_(dicEn),
     dicCh_(dicCh),
     cache_(100)
{
        open_lexicon(0);//打开英文词库
        open_lexicon(1);//打开中文词库
}


void Query::open_lexicon(int flag){//打开词库
    string dic;

    if(flag == 0){
        dic = dicEn_;
    }
    else{
        dic = dicCh_;
    }
    
    index_.open_file(dic);

    ifstream infile;
    infile.open(dic.c_str());
    if(!infile)
        throw runtime_error("open dic failed.");

    read_from_lexicon(infile);//从词库中读取数据
    infile.close();
    infile.clear();
}

//在Cache中寻找
string Query::find_to_cache(const string &word){
   
    string word_right = cache_.get_from_cache(word);//从cache中读取
    string str;

   if(!word_right.empty()){//不为空，代表cache中有这个单词
        str = word_right + string("\r\n");
        //cacheManager_.return_cache(cache_.get_order());归还Cache块
        cout << "find in cache" << endl;
        
       // cacheManager_.returnCache(cache_->get_order());归还cache块
        return str;
   }else{
        cout << "cache isn't exit the word" << endl;
        return string();//返回空对象
   }
}



string Query::find_to_index(const string &word){//在index中寻找
   
    set<string>  tempset = index_.run_index(word);//返回查询词的所有候选索引
    set<string>::const_iterator loc = tempset.find(word);
    if(loc != tempset.end()){//在索引中找到了
        set<string> set ;
        set.insert(*loc);
        cache_.put_into_cache(word, set);//放在cache中

        string final = word+"\r\n";
        cout << "find in index" << endl;
       // cacheManager_.return_cache(cache_.get_order());
        return final;
    }else{
        return string();
    }
}

string Query::final_word(const string &word){//根据最小编辑距离 词频 纠错
    
    priority_queue<pqueue> que;
    string str;
    map<string, int>::const_iterator it = words_.begin();
    for( ; it != words_.end(); ++it){
       int mini = edit_distance(word, it->first);//mini editdistance
        if(mini < 3){
               add_priority(it->first,mini, it->second , que);
        }
        else{
            continue;
        }

   }

   str =  printf_result(que);
   
   //把找到的候选词加到cache中
   cache_.put_into_cache(word, finalset_);
   cout << "add a new pair into cache." << endl;
  // cacheManager_.return_cache(cache_.get_order());
   return str;
    
}


string Query::run_query(const string &word){//入口程序，查询
    ostringstream os;
    os << "get " << cache_.get_order() << " cache." << endl;
    
    string word_right;
    //先在cache中寻找
    word_right= find_to_cache(word);
    if(!word_right.empty())
        return word_right;
 
    //再从index中寻找
    word_right = find_to_index(word);
    if(!word_right.empty())
        return word_right;
    //最后在词典中中寻找
    else
        return final_word(word);//最后根据最小编辑距离和词频，寻找恰当的词
    
    
}

void Query::read_from_lexicon(ifstream &infile){//从词库中读取数据
            
    string line;
    while(getline(infile, line)){
         char buf[100];
         int frequence;
         sscanf(line.c_str() ,"%s %d", buf, &frequence);
         words_[string(buf)] = frequence;
    }
}



void Query::add_priority(const string &word_old, int mini, int frequence,priority_queue<pqueue>  &que){
        
    pqueue q;

    q.word_ = word_old;
    q.mini_distance_ = mini;
    q.frequency_ = frequence;

    que.push(q);
}


int Query::get_len_UTF8(unsigned char c){
    int cnt = 0;
    while(c & (1 << (7-cnt))){
        ++cnt;
    }
    return cnt;
}

void Query::to_stringUTF8(const string &s, vector<uint32_t> &vec){
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

int Query::min_dis(int a, int b, int c){
    int ret = (a<b) ? a : b;
    ret = (ret < c) ? ret : c;
    return ret;
}

int Query::edit_distance_uint32(vector<uint32_t> &w1, vector<uint32_t> &w2){
    int len_1 = w1.size();
    int len_2 = w2.size();
    int memo[100][100];
    memset(memo, 0, sizeof(memo));
    for(int i =1; i <= len_1; ++i)
        memo[i][0] = i;
    for(int j =1; j <= len_2; ++j)
        memo[0][j] = j;
    for( int i=1; i <= len_1; ++i){
        for(int j =1; j<= len_2; ++j){
            if(w1[i-1] == w2[j-1]){
                memo[i][j] = memo[i-1][j-1];
            }else{
                memo[i][j] = min_dis(memo[i-1][j-1], memo[i][j-1]+1, memo[i-1][j]+1);
            }
        }
    }

    return memo[len_1][len_2];
}

int Query::edit_distance(const string &word1, const string &word2){
    vector<uint32_t>  w1, w2;
    to_stringUTF8(word1, w1);
    to_stringUTF8(word2, w2);
    return edit_distance_uint32(w1, w2);
}




string Query::printf_result(priority_queue<pqueue>  que){
  
    string str;
    string str_final="";
    for(int i=0; i<4; i++)
    {

        str = que.top().word_;
        str_final += str+"  ";
	finalset_.insert(str);
        que.pop();
      
    }
    return str_final;
}
