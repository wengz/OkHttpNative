//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_CALL_H
#define OKHTTPNATIVE_CALL_H

class Request;
class Response;


class Call {

public :
    virtual Request * getRequest() = 0;

    virtual Response * execute() = 0;

    virtual void cancel() = 0;

};

#endif //OKHTTPNATIVE_CALL_H