//
// Created by wengzc on 2019/4/22.
//

#include "RequestBody.h"

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

void RequestBody::writeTo(void *target) {

}

RequestBody::RequestBody(const MediaType &contentType, const char *content, int offset, int byteCount)
    :contentType(contentType), contentLength(byteCount){
    this->content = static_cast<char *>(malloc(byteCount));
    memcpy(this->content, content+offset, byteCount);
}

RequestBody::~RequestBody() {
    delete [] this->content;
}

void RequestBody::print() const {
    //string tempContent(content);
    //LogUtil::debug(tempContent);
}
