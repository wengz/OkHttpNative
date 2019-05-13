//
// Created by wengzc on 2019/4/25.
//

#ifndef OKHTTPNATIVE_STATUSLINE_H
#define OKHTTPNATIVE_STATUSLINE_H

#include <string>
#include <stdexcept>

#include "../../Protocol.h"

using namespace std;

class StatusLine {

public :

    static const int HTTP_TEMP_REDIRECT;
    static const int HTTP_PERM_REDIRECT;
    static const int HTTP_CONTINUE;

    static StatusLine parse(const string & statusLine) throw (runtime_error);

    StatusLine(Protocol protocol, int code, std::string & message);

    Protocol protocol;
    int code;
    std::string message;

};


#endif //OKHTTPNATIVE_STATUSLINE_H
