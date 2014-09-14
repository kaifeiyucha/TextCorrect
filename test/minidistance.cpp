#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

int MinInt(int t1, int t2, int t3){
    int min_int;
    min_int = t1>t2 ? t2 : t1;
    min_int = t3>min_int ? min_int : t3;
    return min_int;
}


int MiniDistance(const string &word1, const string &word2, int i, int j){
    if(i == -1 && j == -1)      // apple  apple
        return 0;
    else if(i == -1)//   null apple
        return j+1;
    else if(j == -1)//    apple null
        return i+1;

    if(word1[i-1] == word2[j-1]){
        return MiniDistance(word1, word2, i-1, j-1);
    }else{
        int t1 = MiniDistance(word1, word2, i, j-1)+1;//add
        int t2 = MiniDistance(word1, word2, i-1, j)+1;//delete
        int t3 = MiniDistance(word1, word2, i-1, j-1)+1;//update
        return MinInt(t1, t2, t3);    
    }

}


int main(int argc, const char *argv[])
{
    string word1, word2;
while( cin >> word1 >> word2 )//1 use     2 lexicion
       cout << MiniDistance(word1, word2, word1.size(), word2.size()) << endl;
    return 0;
}
