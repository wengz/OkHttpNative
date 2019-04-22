//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_RESPONSE_H
#define OKHTTPNATIVE_RESPONSE_H

#include "ResponseBody.h"
#include "Headers.h"

class Response {

public :

    //构造器
    class Builder {

    public :

        Builder & setProtocol (string protocol){
            (*this).protocol = protocol;
            return *this;
        }

        Builder & setCode (int code){
            (*this).code = code;
            return *this;
        }

        Builder & setMessage (string message){
            (*this).message = message;
            return * this;
        }

        Builder & setHeaders (Headers & hds){
            (*this).headers = hds.newBuilder();
            return * this;
        }

    private :
        Headers::Builder headers;
        string protocol;
        int code;
        string message;

    };
    //--构造器--end


    Response(){

    }

    ResponseBody & body();

private :
    int code;

};


#endif //OKHTTPNATIVE_RESPONSE_H
