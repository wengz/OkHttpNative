//
// Created by wengzc on 2019/4/22.
//

#include "HttpClient.h"
#include "RealCall.h"

Call * HttpClient::newCall(shared_ptr<HttpClient> client, Request request) {
    LogUtil::debug("HttpClient::newCall(shared_ptr<HttpClient> client, Request request)");
    return RealCall::newRealCall(client, request);
}
