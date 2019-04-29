//
// Created by wengzc on 2019/4/22.
//

#include <unistd.h>
#include <cerrno>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <android/log.h>
#include <unistd.h>
#include <cerrno>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <android/log.h>

#include "Http1Codec.h"
#include "../../Headers.h"
#include "../../Response.h"
#include "../../HttpClient.h"
#include "../../Dns.h"
#include "../../Util.h"
#include "../../Request.h"
#include "../../Response.h"
#include "StatusLine.h"

void Http1Codec::writeSocket (const void * buffer, int length){

    int bytes_left;
    int written_bytes;
    char * ptr;

    ptr = (char *)buffer;
    bytes_left = length;
    while (bytes_left > 0){
        written_bytes = write(socketFd, ptr, bytes_left);
        if (written_bytes <= 0){
            if (errno == EINTR){
                written_bytes = 0;
            }else{
                throw new runtime_error("socket write error");
            }
        }
        bytes_left -= written_bytes;
        ptr += written_bytes;
    }
}


void Http1Codec::writeSocket(const string & str) {
    const char * write_buff = str.c_str();
    writeSocket(write_buff, strlen(write_buff));
}

void Http1Codec::writeRequest(const Headers & hds, const string & requestLine) {
    writeSocket(requestLine);
    writeSocket("\r\n");
    for (int i = 0, size = hds.size(); i < size; i++){
        writeSocket(hds.name(i));
        writeSocket(": ");
        writeSocket(hds.value(i));
        writeSocket("\r\n");
    }
    writeSocket("\r\n");
}

void Http1Codec::writeRequestHeaders(Request * req) {
    writeRequest(req->getHeaders(), "POST /testpost HTTP/1.1");
}

Headers * Http1Codec::readHeaders() {
    Headers::Builder hb;
    string line;
    while (true){
        line = readHeaderLine();
        if (line.size() <= 2){
            break;
        }

        if (line.find("chunked") != string::npos ){
            transferChunked = true;
        }

        if (line.find("Content-Length") != string::npos){
            responseContentLength = getContentLength(line);
        }

        hb.addLenient(line);
    }
    return hb.build();
}


Response *  Http1Codec::readResponseHeaders(bool expectContinue) {

    //fixme 异常时指针释放
    StatusLine * statusLine = StatusLine::parse(readLine());

    if (expectContinue && statusLine->code == StatusLine::HTTP_CONTINUE ){
        return nullptr;
    }

    Response * response = new Response();
    response->setProtocol(statusLine->protocol);
    response->setCode(statusLine->code);
    response->setMessage(statusLine->message);
    response->setHeaders(readHeaders());
    delete statusLine;

    return response;
}

void appendBytes (char * * orgBytes, long orgByteLength, char * addBytes, long addByteLength){
    long newByteLength = orgByteLength + addByteLength;
    char * newBytes = static_cast<char *>(malloc(newByteLength));
    memcpy(newBytes, *orgBytes, orgByteLength);
    memcpy(newBytes + orgByteLength, addBytes, addByteLength);
    (* orgBytes) = newBytes;
    delete [] (* orgBytes);
    delete [] addBytes;
}

char *Http1Codec::responseBodyBytes(long *contentLength) {
    char * buff;
    long curLength = 0;
    if (transferChunked){//Transfer-Encoding: chunked
        while (true){
            string chunkedSizeLine = readLine();
            unsigned long chunkedSize = hexToDec(chunkedSizeLine.c_str(), chunkedSizeLine.size() - 2);
            char * chunkBuff = new char[chunkedSize];
            int readCount = readn(chunkBuff, chunkedSize);
            appendBytes(&buff, curLength, chunkBuff, readCount);
            if (readCount != chunkedSize){
                throw runtime_error("chunked body read content size not match ");
            }
            curLength += readCount;
            readLine();//读取块末尾\r\n
            if (chunkedSize == 0){
                break;
            }
        }
        *contentLength = curLength;
    }else{
        buff = new char[responseContentLength];
        memset(buff, 0, responseContentLength);
        int readCount = readn(buff, responseContentLength);
        *contentLength = readCount;
    }

    return buff;
}


string Http1Codec::readHeaderLine() {
    string line = readLine();
    return line;
}

size_t Http1Codec::readLine2(int fd, void *vptr, size_t maxlen)
{
    size_t n, rc;
    char c, *ptr;
    ptr = (char*)vptr;
    for(n = 1 ;n < maxlen; n++)
    {
        if( (rc = read(fd, &c, 1)) == 1)
        {
            *ptr++ = c;
            if(c == '\n')
                break;
        }else if( rc == 0) {
            *ptr = 0;
            return (n-1);
        }else{
            return -1;
        }
    }
    *ptr = 0;
    return n;
}


string Http1Codec::readLine() {
    size_t  max_char = 1024;
    char * bufp = new char[max_char];
    char * star_ptr = bufp;
    int ret;
    int nread;
    int nleft = max_char;

    while (true){
        ret = recvPeek(bufp, nleft);
        if (ret <= 0){
            throw runtime_error("socket read error");
        }
        nread = ret;
        for (int i = 0; i < nread; i++){
            if (bufp[i] == '\n'){
                readn(bufp, i+1);
                bufp += (i+1);
                int count = bufp - star_ptr;
                return string(star_ptr, count);
            }
        }

        if (nread > nleft){
            throw runtime_error("socket read error");
        }

        nleft -= nread;
        ret = readn(bufp, nread);

        if (ret != nread){
            throw runtime_error("socket read error");
        }

        bufp  += nread;
    }
}

int Http1Codec::readn(void *vptr, size_t n) {
    size_t  nleft = n;
    ssize_t nread = 0;
    char * ptr = (char *) vptr;
    while (nleft > 0){
        nread = read(socketFd, ptr, nleft);
        if (-1 == nread){
            if (EINTR == errno){
                nread = 0;
            }else{
                throw runtime_error("socket read error");
            }
        }else if (0 == nread){
            break;
        }
        nleft -= nread;
        ptr += nread;
    }
    return n - nleft;
}

ssize_t Http1Codec::recvPeek(void *buf, size_t len) {
    while (true){
        int ret = recv(socketFd, buf, len, MSG_PEEK);
        if (ret == -1 && errno == EINTR){
            //fixme 更详细的错误判断，抛出异常?
            continue;
        }
        return ret;
    }
}

Http1Codec::Http1Codec():transferChunked(false) {

}


void Http1Codec::openResponseBody() {

}


Http1Codec::Http1Codec(Call *arg_call) {
    call = arg_call;
}

Http1Codec::Http1Codec(int connectedSocket):transferChunked(false) {
    socketFd = connectedSocket;
}

char * Http1Codec::readBytes(int &size) {
    int size_limit = 1024*1024;
    char * p_buff = new char[size_limit];
    int r_count = readn(p_buff, size_limit);
    size = r_count;
    return p_buff;
}
