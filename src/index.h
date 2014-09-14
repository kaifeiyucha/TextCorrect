#ifndef INDEX_H
#define INDEX_H 
///////////////////////////////////////gaiguode/////////////////
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>

class Index{
    public:
        typedef std::map<char, std::set<std::string> >::size_type line_no;
          Index()
          {};

/*          Index(const std::string &filename){
            open_file(filename);
          }
 */
          
          void open_file(const std::string &);
          std::set< std::string > run_index(const std::string &);//根据用户输入的词，找到并集
          void printall()const;    

    private:
        int  get_len_UTF8(unsigned char );
        void to_stringUTF8(const std::string &, std::vector<uint32_t> &);  
        void store_file(std::ifstream &infile);//把词库存在vector中
         // void build_map();//建立一个关联字母和空的单词集合的map
        void build_index();//将所有的单词读入，并建立相应的索引

        std::vector<std::string> words_text_;//储存词典
        std::map< uint32_t, std::set<std::string> > word_map_;//map<词， 所含词的所有set>
};
#endif  /*INDEX_H*/
