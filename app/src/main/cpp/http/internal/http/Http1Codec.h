//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_HTTP1CODEC_H
#define OKHTTPNATIVE_HTTP1CODEC_H

#include <unistd.h>
#include <cerrno>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../../Headers.h"
#include "../../Response.h"
#include "../connection/RealConnection.h"

class Call;
class Request;
class HttpClient;
class Response;

class Http1Codec {

public :

    Http1Codec ();

    Http1Codec(shared_ptr<RealConnection> connection);

    void writeRequestHeaders(Request & req);

    Response * readResponseHeaders(bool expectContinue);

    void writeRequest(const Headers & hds, const string & requestLine);

    char * readBytes(int & size);

    int readn(void * vptr, size_t n);

    char * responseBodyBytes (long * contentLength);

    void writeSocket (const void * buffer, int length) const;

    void writeSocket (const string & str);

private :

    Headers * readHeaders ();

    /**
     * 网络连接
     */
    shared_ptr<RealConnection> connection;

    /**
     * http回应体长度
     */
    long responseContentLength;

    /**
     * http回应分段接收
     */
    bool transferChunked;

    string readHeaderLine();

    ssize_t recvPeek (void * buf, size_t len);

    string readLine();

};


#endif //OKHTTPNATIVE_HTTP1CODEC_H
