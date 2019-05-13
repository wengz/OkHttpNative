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

class RealCall : public Call{

public:
    static RealCall * newRealCall(shared_ptr<HttpClient> client, Request req);

    Request getRequest();

    unique_ptr<Response> execute();

    void cancel();

    virtual ~RealCall();

private:
    shared_ptr<HttpClient> client;
    Request originRequest;
    shared_ptr<RealConnection> connection;
    shared_ptr<Http1Codec> codec;

    RealCall(shared_ptr<HttpClient> client, Request req);
};


#endif //OKHTTPNATIVE_REALCALL_H
