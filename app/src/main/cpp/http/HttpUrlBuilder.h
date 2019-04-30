//
// Created by wengzc on 2019/4/30.
//

#ifndef OKHTTPNATIVE_HTTPURLBUILDER_H
#define OKHTTPNATIVE_HTTPURLBUILDER_H

#include <string>
#include <vector>
#include "HttpUrl.h"


using namespace std;

//构造器
class HttpUrl::Builder {

public :
    Builder();
    HttpUrl build();
    Builder & setScheme(string scheme);
    Builder & setUsername(string username);
    Builder & setEncodedUsername(string encodedUserName);
    Builder & setPassword(string password);
    Builder & setEncodedPassword(string encodedPassword);
    Builder & setHost (string host);
    Builder & setPort(int port);
    int effectivePort();
    Builder & parse(string input);
    string getHost();
    string getScheme();

private :
    string scheme;
    string encodedUsername;
    string encodedPassword;
    string host;
    int port;
    vector<string> encodedPathSegments;
    vector<string> encodedQueryNamesAndVcanonicalizealues;
    string encodedFragment;

    void push(string input, int pos, int limit, bool addTrailingSlash, bool alreadyEncoded);
    void pop();
    bool isDot(string input);
    bool isDotDot(string input);

    static string canonicalizeHost(string & input, int pos, int limit);
    static int schemeDelimiterOffset (string & input, int pos, int limit);
    static int slashCount(string & input, int pos, int limit);
    static int portColonOffset (string & input, int pos, int limit);
    static int parsePort (string & input, int pos, int limit);

};//构造器


#endif //OKHTTPNATIVE_HTTPURLBUILDER_H
