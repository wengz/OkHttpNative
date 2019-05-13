//
// Created by wengzc on 2019/4/22.
//

#include "RequestBody.h"
#include "internal/http/Http1Codec.h"

#include <cstring>

RequestBody RequestBody::create(const MediaType &contentType, const char * content, int offset, int byteCount) {
    return RequestBody(contentType, content, offset, byteCount);
}

MediaType RequestBody::getContentType() {
    return contentType;
}

long RequestBody::getContentLength() {
    return contentLength;
}

void RequestBody::writeTo(const Http1Codec & codec) {
    codec.writeSocket(content, getContentLength());
}

RequestBody::RequestBody(const MediaType &contentType, const char *content, int offset, int byteCount)
    :contentType(contentType), contentLength(byteCount){
    this->content = new char[byteCount];
    memcpy(this->content, content+offset, byteCount);
}

RequestBody::~RequestBody() {
    delete [] this->content;
}

void RequestBody::print() const {
    //string tempContent(content);
    //LogUtil::debug(tempContent);
}

RequestBody::RequestBody(const RequestBody & old) {
    contentType = old.contentType;
    contentLength = old.contentLength;
    this->content = new char[contentLength];
    memcpy(this->content, old.content, contentLength);
}

RequestBody &RequestBody::operator=(const RequestBody &rhs) {
    contentType = rhs.contentType;
    contentLength = rhs.contentLength;
    this->content = new char[contentLength];
    memcpy(this->content, rhs.content, contentLength);
    return *this;
}
