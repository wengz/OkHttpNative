//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_DNS_H
#define OKHTTPNATIVE_DNS_H

#include <string>

using namespace std;

class Dns {

public:
    static vector<string> lookup(const string & hostname);
};


#endif //OKHTTPNATIVE_DNS_H
