//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_CALL_H
#define OKHTTPNATIVE_CALL_H

#include <memory>

class Request;
class Response;


class Call {

public :

    virtual std::unique_ptr<Response> execute() = 0;

};

#endif //OKHTTPNATIVE_CALL_H