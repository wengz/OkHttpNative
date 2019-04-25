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

    //内部类构造器
    class Builder {

    public :
        Builder();
        Builder & setUrl (HttpUrl url);
        Builder & setUrl (string & url);
        Request build();
        Headers::Builder & getHeaders();
        HttpUrl getUrl();

        Builder & post(RequestBody * rb);

        Builder & setMethod(string method, RequestBody * rb);

    private :
        string method;
        Headers::Builder headers;
        RequestBody * body;
        HttpUrl url;
    };
    //--end--内部类构造器


    Request(Builder & builder);

    std::string getMethod();

    Headers & getHeaders();

    HttpUrl getUrl();

private :
    std::string method;
    HttpUrl url;
    Headers headers;

};


#endif //OKHTTPNATIVE_REQUEST_H
