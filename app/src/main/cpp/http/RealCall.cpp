//
// Created by wengzc on 2019/4/23.
//

#include "RealCall.h"
#include "internal/http/Http1Codec.h"
#include "Response.h"
#include "internal/http/HttpMethod.h"
#include <memory>

RealCall * RealCall::newRealCall(shared_ptr<HttpClient> client, Request req){
    return new RealCall(client, req);
}

Request RealCall::getRequest() {
    return originRequest;
}

unique_ptr<Response> RealCall::execute(){
    connection->connectServer();
    codec = shared_ptr<Http1Codec>(RealConnection::newStream(connection));
    codec->writeRequestHeaders(originRequest);

    unique_ptr<Response> response;

    if (HttpMethod::permitsRequestBody(originRequest.getMethod()) ){
        originRequest.getBody().writeTo(*codec);
    }

    if (response){
        response = unique_ptr<Response>(codec->readResponseHeaders(false));
    }
    response->setResponseBody(new ResponseBody(codec));
    return response;
}

RealCall::RealCall(shared_ptr<HttpClient> client, Request req)
        :client(client), originRequest(req){
    connection = shared_ptr<RealConnection>(new RealConnection(http_1_1, req.getUrl().getHost(), req.getUrl().getPort()));
}

void RealCall::cancel() {

}

RealCall::~RealCall() {

};

