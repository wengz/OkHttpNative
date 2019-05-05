//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_REQUEST_H
#define OKHTTPNATIVE_REQUEST_H

#include <string>

#include "Headers.h"
#include "HttpUrl.h"
#include "RequestBody.h"

using namespace std;

class Request {

public :

    class Builder;

    Request(Builder & builder);

    std::string getMethod();

    Headers & getHeaders();

    HttpUrl getUrl();

    RequestBody * getBody();

private :
    std::string method;
    HttpUrl url;
    Headers headers;
    RequestBody * body;

};


#endif //OKHTTPNATIVE_REQUEST_H
