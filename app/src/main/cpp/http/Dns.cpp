//
// Created by wengzc on 2019/4/22.
//

#include "Dns.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

vector<string> Dns::lookup(const string &hostname) {
    vector<string> result;
    struct hostent * h;
    h = gethostbyname(hostname.c_str());
    if (h != NULL){
        char ** p_start = h->h_addr_list;
        while (true){
            char * addr_item = * p_start;
            if (addr_item == NULL){
                break;
            }
            char * ip_addr = inet_ntoa(*((struct in_addr *)addr_item));
            result.push_back(ip_addr);
            p_start++;
        }
    }
    return result;
}
