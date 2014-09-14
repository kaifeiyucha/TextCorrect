#include "Cache.h"
#include <iostream>
#include <set>

using namespace std;

int main(int argc, const char *argv[])
{
    LRUCache Lru(2);
    set<string> s1;
    s1.insert("google");
    s1.insert("baidu");
    s1.insert("QQ");

    set<string> s2;
    s2.insert("mother");
    s2.insert("father");
    s2.insert("child");

    Lru.put_into_cache("net", s1);
    Lru.put_into_cache("family", s2);

    cout <<"net        "<< Lru.get_from_cache("net") << endl;
//    cout <<"family     "<< Lru.get_from_cache("family") << endl;

    Lru.put_into_cache("hello", s2);
    cout << "hello    " <<Lru.get_from_cache("hello") << endl;

    cout << "family   " <<Lru.get_from_cache("family") << endl;
    cout << "net   " <<Lru.get_from_cache("net") << endl;

    return 0;
}
