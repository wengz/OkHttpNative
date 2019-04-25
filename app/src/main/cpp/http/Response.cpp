//
// Created by wengzc on 2019/4/22.
//

#include "Response.h"

Response & Response::setProtocol(Protocol protocol) {
    this->protocol = protocol;
    return *this;
}

Response & Response::setCode(int code) {
    this->code = code;
    return * this;
}

Response & Response::setMessage(string msg) {
    this->message = msg;
    return *this;
}

Response & Response::setHeaders(Headers *headers) {
    this->headers = headers;
    return *this;
}

Response &Response::setResponseBody(ResponseBody *responseBody) {
    this->responseBody = responseBody;
    return *this;
}
