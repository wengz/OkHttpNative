//
// Created by wengzc on 2019/4/22.
//

#include "ResponseBody.h"

#include "internal/http/Http1Codec.h"

ResponseBody::~ResponseBody(){

}

char * ResponseBody::bytes(long *contentLength) {
    char * buff = http1Codec->responseBodyBytes(contentLength);
    return buff;
}
