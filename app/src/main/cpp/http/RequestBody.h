//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_REQUESTBODY_H
#define OKHTTPNATIVE_REQUESTBODY_H

#include "MediaType.h"
#include "../log/LogUtil.h"
#include "internal/http/Http1Codec.h"

class RequestBody {

public :

    static RequestBody create(const MediaType & contentType, const char * content, int offset, int byteCount );

    RequestBody() : content(nullptr), contentLength(0) {}

    virtual MediaType getContentType();

    virtual long getContentLength();

    virtual void writeTo(const Http1Codec & codec);

    virtual void print() const;

    RequestBody(const MediaType & contentType, const char * content, int offset, int byteCount);

    RequestBody(const RequestBody &);

    RequestBody& operator= (const RequestBody & rhs);

    virtual ~RequestBody();

private :
    MediaType contentType;
    char * content;
    int contentLength;
};

#endif //OKHTTPNATIVE_REQUESTBODY_H
