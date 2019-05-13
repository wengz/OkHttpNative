//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_HTTPCLIENT_H
#define OKHTTPNATIVE_HTTPCLIENT_H

#include "Request.h"
#include "Call.h"

class HttpClient {

public :

    //构造器
    class Builder {

        friend  class HttpClient;

    public :

    private :
        int callTimeout;
        int connectTimeout;
        int readTimeout;
        int writeTimeout;
    };
    //构造器 end

    friend class Builder;

    HttpClient (const Builder & b = Builder()){
        callTimeout = b.callTimeout;
        connectTimeout = b.connectTimeout;
        readTimeout = b.readTimeout;
        writeTimeout = b.writeTimeout;
    }

    static Call * newCall(shared_ptr<HttpClient> client, Request request);

protected :
    int callTimeout;
    int connectTimeout;
    int readTimeout;
    int writeTimeout;

private :

};


#endif //OKHTTPNATIVE_HTTPCLIENT_H
