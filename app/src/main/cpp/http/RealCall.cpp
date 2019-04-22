//
// Created by wengzc on 2019/4/23.
//

#include "RealCall.h"
#include "internal/http/Http1Codec.h"

RealCall *  RealCall::newRealCall(HttpClient * client, Request * req){
    return new RealCall(client, req);
}

Request * RealCall::getRequest() {
    return originRequest;
}

void RealCall::execute(){
    connection->connectServer();
    codec = connection->newStream();
    codec->writeRequestHeaders(originRequest);
    codec->readResponseHeaders();
    releaseResource();
}

void RealCall::cancel() {

}

RealCall::RealCall(HttpClient *client, Request *req)
        :client(client), originRequest(req){
    connection = new RealConnection(http_1_1, req->getUrl()->getHost(), req->getUrl()->getPort());
}


void RealCall::releaseResource() {
    delete codec;
    delete connection;
}

RealCall::~RealCall() {

};

