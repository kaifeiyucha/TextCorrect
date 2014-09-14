#include "index.h"
#include <iostream>
#include <string>
#include <set>
#include <iterator>


using namespace std;

int main(int argc, const char *argv[])
{
    Index I;
    string file = "c.txt";
    I.open_file(file);
     I.printall();
    string  word;
    cin >> word; 
     set<string> set_index = I.run_index(word);
     set<string>::iterator it = set_index.begin();
     set<string>::iterator itend = set_index.end();
     for(;it != itend; ++it)
     { 
         cout << (*it) << endl;
     }
    return 0;
}
