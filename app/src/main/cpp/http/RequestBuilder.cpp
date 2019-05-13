//
// Created by wengzc on 2019/5/5.
//

#include "RequestBuilder.h"
#include "Request.h"

Request Request::Builder::build() {
    return Request(*this);
}

Request::Builder::Builder() {
    method = "GET";
    headers.add("Connection", "Keep-Alive");
}

Request::Builder & Request::Builder::setUrl(HttpUrl & urlArg) {
    url = urlArg;
    headers.add("Host", urlArg.getHost());
    return * this;
}

Request::Builder &Request::Builder::setUrl(string &url) {
    HttpUrl retUrl = HttpUrl::get(url);
    return setUrl(retUrl);
}

Headers::Builder &Request::Builder::getHeaders() {
    return headers;
}

HttpUrl Request::Builder::getUrl() {
    return url;
}

Request::Builder &Request::Builder::post(RequestBody &rb) {
    return setMethod("post", rb);
}

Request::Builder &Request::Builder::setMethod(string method, RequestBody & rb) {
    this->method = method;
    this->body = rb;
    //带消息体的方法添加特别的头信息
    //headers.add("content-type", "text/plain");
    //headers.add("content-length", "3");
    return * this;
}

string Request::Builder::getMethod() {
    return method;
}

RequestBody Request::Builder::getBody() {
    return body;
}
