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
        : headers(builder.getHeaders()), url(builder.getUrl()){
}

HttpUrl * Request::getUrl() {
    return url;
}


Request Request::Builder::build() {
    return Request(*this);
}

Request::Builder::Builder() {
    method = "GET";
    headers.add("Connection", "Keep-Alive");
    //headers.add("Accept-Encoding", "gzip");
}

Request::Builder & Request::Builder::setUrl(HttpUrl * urlArg) {
    url = urlArg;
    headers.add("Host", urlArg->getHost());
    return * this;
}

Request::Builder &Request::Builder::setUrl(string &url) {
    HttpUrl * retUrl = HttpUrl::get(url);
    return setUrl(retUrl);
}

Headers::Builder &Request::Builder::getHeaders() {
    return headers;
}

HttpUrl *Request::Builder::getUrl() {
    return url;
}