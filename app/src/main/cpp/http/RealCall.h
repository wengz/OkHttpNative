//
// Created by wengzc on 2019/4/23.
//

#ifndef OKHTTPNATIVE_REALCALL_H
#define OKHTTPNATIVE_REALCALL_H

#include "Call.h"
#include "HttpClient.h"
#include "Request.h"
#include "internal/connection/RealConnection.h"

class Response;

class RealCall :  public Call{

public:
    static RealCall * newRealCall(HttpClient * client, Request  * req);

    Request * getRequest();

    Response * execute();

    void cancel();

    void releaseResource();

    virtual ~RealCall();

private:
    HttpClient * client;
    Request * originRequest;
    RealConnection * connection;
    Http1Codec * codec;

    RealCall(HttpClient * client, Request * req);
};


#endif //OKHTTPNATIVE_REALCALL_H
