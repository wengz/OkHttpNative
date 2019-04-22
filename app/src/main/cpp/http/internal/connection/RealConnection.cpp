//
// Created by wengzc on 2019/4/22.
//

#include "RealConnection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <android/log.h>
#include <jni.h>
#include <cerrno>
#include <zconf.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <android/log.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/signal.h>
#include <android/log.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <endian.h>
#include <vector>

#include "../../Dns.h"

RealConnection::RealConnection(Protocol prot, string host, int port)
        :protocol(prot), host(host), m_port(port){
}

RealConnection::~RealConnection() {
    if (socketFd != -1){
        close(socketFd);
    }
}

void RealConnection::connectServer() {
    vector<string> ips = Dns::lookup(host);
    vector<string>::iterator itr = ips.begin();
    while (itr != ips.end()){
        const char * server_ip = (*itr).c_str();
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(server_ip);
        server_addr.sin_port = htons(m_port);
        socketFd = socket(PF_INET, SOCK_STREAM, 0);
        int con_res = connect(socketFd, (struct sockaddr *) & server_addr, sizeof(server_addr));
        if (con_res == 0){
            break;
        }
        itr++;
    }
}

int RealConnection::connectedSocket() {
    return socketFd;
}

Http1Codec * RealConnection::newStream() {
    return new Http1Codec(socketFd);
}