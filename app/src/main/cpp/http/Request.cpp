//
// Created by wengzc on 2019/4/22.
//

#include "Request.h"
#include "RequestBuilder.h"

std::string Request::getMethod() {
    return method;
}

Headers & Request::getHeaders() {
    return headers;
}

Request::Request(Request::Builder & builder)
        : headers(builder.getHeaders()), url(builder.getUrl()) , method(builder.getMethod()), body(builder.getBody()) {
    LogUtil::debug("Request::Request(Request::Builder & builder)");
}

HttpUrl Request::getUrl() {
    return url;
}

RequestBody Request::getBody() {
    return body;
}



