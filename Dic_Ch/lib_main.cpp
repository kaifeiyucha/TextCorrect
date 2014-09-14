#include "lib.h"
#include <iostream>
#include <string>

using namespace std;

const char* dict_path = "dict/jieba.dict.utf8";
const char* model_path = "dict/hmm_model.utf8";

int main(int argc, const char *argv[])
{
    Lexicon lex("Chinese.txt", 
            "charactor_set.txt",
            dict_path,
            model_path);
    
    lex.printf_lexicon();
    return 0;
}
