//
// Created by wengzc on 2019/4/22.
//

#include "Request.h"

std::string Request::getMethod() {
    return method;
}

Headers & Request::getHeaders() {
    return headers;
}

Request::Request(Request::Builder & builder)
        : headers(builder.getHeaders()), url(builder.getUrl()) , method(builder.getMethod()), body(builder.getBody()){
}

HttpUrl Request::getUrl() {
    return url;
}

RequestBody *Request::getBody() {
    return body;
}


Request Request::Builder::build() {
    return Request(*this);
}

Request::Builder::Builder() {
    method = "GET";
    headers.add("Connection", "Keep-Alive");
    //headers.add("Accept-Encoding", "gzip");
}

Request::Builder & Request::Builder::setUrl(HttpUrl urlArg) {
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

Request::Builder &Request::Builder::post(RequestBody *rb) {
    return setMethod("post", rb);
}

Request::Builder &Request::Builder::setMethod(string method, RequestBody *rb) {
    this->method = method;
    this->body = rb;
    headers.add("content-type", "text/plain");
    headers.add("content-length", "3");
    return * this;
}

string Request::Builder::getMethod() {
    return method;
}

RequestBody *Request::Builder::getBody() {
    return body;
}

