//
// Created by wengzc on 2019/5/5.
//

#ifndef OKHTTPNATIVE_REQUESTBUILDER_H
#define OKHTTPNATIVE_REQUESTBUILDER_H

#include "Request.h"


class Request::Builder {

public :
    Builder();
    Builder & setUrl (HttpUrl url);
    Builder & setUrl (string & url);
    Request build();
    Headers::Builder & getHeaders();
    HttpUrl getUrl();

    Builder & post(RequestBody * rb);

    Builder & setMethod(string method, RequestBody * rb);

    string getMethod();

    RequestBody * getBody();

private :
    string method;
    HttpUrl url;
    Headers::Builder headers;
    RequestBody * body;

};


#endif //OKHTTPNATIVE_REQUESTBUILDER_H
