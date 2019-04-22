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

class Call;
class Request;
class HttpClient;

class Http1Codec {

public :

    Http1Codec ();

    Http1Codec(Call * call);

    Http1Codec(int connectedSocket);

    void writeRequestHeaders(Request & req);

    void readResponseHeaders();

    void openResponseBody();

    void writeRequest(const Headers & hds, const string & requestLine);

    char * readBytes(int & size);

    int readn(void * vptr, size_t n);

private :

    int socketFd;
    HttpClient  * client;
    Call * call;

    void writeSocket (const void * buffer, int length);

    void writeSocket (const string & str);

    string readHeaderLine();

    const Headers & readHeaders();

    ssize_t recvPeek (void * buf, size_t len);

    string readLine();

    size_t readLine2(int fd, void *vptr, size_t maxlen);
    
    
    

};


#endif //OKHTTPNATIVE_HTTP1CODEC_H
