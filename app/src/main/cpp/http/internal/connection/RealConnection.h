//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_REALCONNECTION_H
#define OKHTTPNATIVE_REALCONNECTION_H

#include <string>

#include "../../Protocol.h"
#include "../../core/Uncopyable.h"

using namespace std;

class Http1Codec;

class RealConnection : private Uncopyable{

public :

    RealConnection (Protocol prot, string host, int port);

    //连接主机域名指向的服务器
    void connectServer();

    //已经连接的socket描述符
    int connectedSocket();

    virtual ~RealConnection();

    //在连接上创建流编解码工具
    static Http1Codec * newStream(shared_ptr<RealConnection> connection);

private :
    //socket 描述符
    int socketFd = -1;

    //连接使用的协议
    Protocol protocol;

    //主机域名
    string host;

    //主机端口
    int port;
};


#endif //OKHTTPNATIVE_REALCONNECTION_H
