//
// Created by wengzc on 2019/4/25.
//

#include "StatusLine.h"

const int StatusLine::HTTP_TEMP_REDIRECT = 307;
const int StatusLine::HTTP_PERM_REDIRECT = 308;
const int StatusLine::HTTP_CONTINUE = 100;

StatusLine::StatusLine(Protocol protocol, int code, std::string & message)
    :protocol(protocol), code(code), message(message){

}

StatusLine StatusLine::parse(const string & statusLine) throw (runtime_error){
    // H T T P / 1 . 1   2 0 0   T e m p o r a r y   R e d i r e c t
    // 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0

    // Parse protocol like "HTTP/1.1" followed by a space.
    int codeStart;
    Protocol protocol;
    if (statusLine.find("HTTP/1.") == 0){
        if (statusLine.size() < 9 || statusLine[8] != ' '){
            throw runtime_error("status line 格式不正确");
        }
        int httpMinorVersion = statusLine[7] - '0';
        codeStart = 9;
        if (httpMinorVersion == 0){
            protocol = Protocol::http_1_0;
        }else if (httpMinorVersion == 1){
            protocol = Protocol::http_1_1;
        }else {
            throw runtime_error("status line 格式不正确");
        }

    }else if (statusLine.find("ICY ") == 0){
        protocol = Protocol ::http_1_0;
        codeStart = 4;

    }else {
        throw runtime_error("status line 格式不正确");
    }

    // Parse response code like "200". Always 3 digits.
    if (statusLine.size() < codeStart + 3){
        throw runtime_error("status line 格式不正确");
    }
    int code;
    code = atoi(statusLine.substr(codeStart, codeStart+3).c_str());

    // Parse an optional response message like "OK" or "Not Modified". If it
    // exists, it is separated from the response code by a space.
    string message = "";
    if (statusLine.size() > codeStart + 3){
        if (statusLine[codeStart + 3] != ' '){
            throw runtime_error("status line 格式不正确");
        }
        message = statusLine.substr(codeStart+4);
    }

    return StatusLine(protocol, code, message);
}
