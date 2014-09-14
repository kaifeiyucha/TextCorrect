#include "lexicon.h"
#include <iostream>
#include <string>

int main(int argc, const char *argv[])
{
    Lexicon lex("a.txt");
    
    lex.printf_lexicon();
    return 0;
}
