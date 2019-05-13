//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_RESPONSEBODY_H
#define OKHTTPNATIVE_RESPONSEBODY_H

#include <string>

class Http1Codec;

using namespace std;

class ResponseBody {

public :

    ResponseBody (shared_ptr<Http1Codec> http1Codec):http1Codec(http1Codec){

    };

    virtual char * bytes(long * contentLength);

private :

    shared_ptr<Http1Codec> http1Codec;
};


#endif //OKHTTPNATIVE_RESPONSEBODY_H
