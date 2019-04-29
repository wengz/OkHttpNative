//
// Created by wengzc on 2019/4/29.
//

#ifndef OKHTTPNATIVE_HTTPMETHOD_H
#define OKHTTPNATIVE_HTTPMETHOD_H

#include <string>

using namespace std;

class HttpMethod {

public :

    static bool permitsRequestBody(const string & method);

};


#endif //OKHTTPNATIVE_HTTPMETHOD_H
