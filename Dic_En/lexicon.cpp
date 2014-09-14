#include "lexicon.h"

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

Lexicon::Lexicon(const string &filename){
    read_file(filename);
}

void Lexicon::store_file(const string &filename){
    ifstream infile;
    infile.open(filename.c_str());
    if(!infile)
        ERR_EXIT("open file fail");

    string line;
    while(getline(infile, line)){
        handle_punct(line);        
        lines_.push_back(line);
    }
    infile.close();
}

void Lexicon::handle_punct(string &line){
    for(size_t i=0; i<line.size(); i++){
        if(isalpha(line[i]))
            line[i]= tolower(line[i]);
        else
            line[i]=' ';
    }
}

void Lexicon::build_map(){
    for(line_no ix = 0; ix != lines_.size(); ++ix){
        istringstream iss(lines_[ix]);
        string word;
        while(iss >> word){
            words_count[word]++;

        }
    }
}

void Lexicon::printf_lexicon(){
    int fd = open("DicEn.txt", O_WRONLY|O_CREAT, 0666);
    if(fd == -1)
        ERR_EXIT("open b.txt fail");
    close(STDOUT_FILENO);
    dup(fd);


    for(map<string, int>::iterator it = words_count.begin(); it != words_count.end(); ++it){
        cout << it->first <<"   "  << it->second << endl;    
    }

    close(fd);
}

