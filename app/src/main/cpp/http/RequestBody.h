//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_REQUESTBODY_H
#define OKHTTPNATIVE_REQUESTBODY_H

#include "MediaType.h"

class RequestBody {

public :

    static RequestBody create(const MediaType & contentType, char * content, int offset, int byteCount );

    virtual MediaType getContentType();

    virtual long getContentLength();

    virtual void writeTo(void * target);

private :
    RequestBody(const MediaType & contentType, char * content, int offset, int byteCount);
    MediaType contentType;
    char * content;
    int contentLength;

    virtual ~RequestBody();
};


#endif //OKHTTPNATIVE_REQUESTBODY_H
