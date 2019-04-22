//
// Created by wengzc on 2019/4/22.
//

#include "HttpClient.h"
#include "RealCall.h"

Call *HttpClient::newCall(Request * request) {
    return RealCall::newRealCall(this, request);
}
