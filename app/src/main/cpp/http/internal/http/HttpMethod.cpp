//
// Created by wengzc on 2019/4/29.
//

#include "HttpMethod.h"

bool HttpMethod::permitsRequestBody(const string & method) {
    if (method.compare("GET") == 0 || method.compare("HEAD") == 0){
        return false;
    }
    return true;
}
