//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_HTTPCLIENT_H
#define OKHTTPNATIVE_HTTPCLIENT_H

class Call;
class Request;

class HttpClient {

public :

    //构造器
    class Builder {

        friend  class HttpClient;

    public :

    private :
        int callTimeout;
        int connectTimeout;
        int readTimeout;
        int writeTimeout;
    };
    //构造器 end

    friend class Builder;

    HttpClient (const Builder & b = Builder()){
        callTimeout = b.callTimeout;
        connectTimeout = b.connectTimeout;
        readTimeout = b.readTimeout;
        writeTimeout = b.writeTimeout;
    }

    Call * new_call(Request);

protected :
    int callTimeout;
    int connectTimeout;
    int readTimeout;
    int writeTimeout;

private :

};


#endif //OKHTTPNATIVE_HTTPCLIENT_H
