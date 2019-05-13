//
// Created by wengzc on 2019/4/29.
//

#include "HttpMethod.h"

bool HttpMethod::permitsRequestBody(const string & method) {
    if ( method == "GET" || method == "HEAD" ){
        return false;
    }
    return true;
}
