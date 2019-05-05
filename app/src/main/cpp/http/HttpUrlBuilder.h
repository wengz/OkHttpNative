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
    friend HttpUrl;

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
    Builder & addPathSegment(string pathSegment);
    Builder & addPathSegments(string pathSegments);
    Builder & addEncodedPathSegment(string pathSegment);
    Builder & addEncodedPathSegments(string pathSegments);
    Builder & setPathSegment(int index, string pathSegment);
    Builder & setEncodedPathSegment(int index, string encodedPathSegment);
    Builder & removePathSegment(int index);
    Builder & encodePath(string encodedPath);
    Builder & query(string query);
    Builder & encodedQuery(string query);
    Builder & addQueryParameter(string name, string value);
    Builder & addEncodedQueryParameter(string encodedName, string encodedValue);
    Builder & setQueryParameter(string name, string value);
    Builder & setEncodedQueryParameter(string encodedName, string encodedValue);
    Builder & removeAllQueryParameters(string name);
    Builder & removeAllEncodedQueryParameters(string encodedName);
    Builder & setFragment(string fragment);
    Builder & seEncodedtFragment(string fragment);
    Builder & parse(HttpUrl * base, string input);
    string getHost();
    string getScheme();
    string toString();

protected:
    Builder & reencodeForUri();

private :
    string scheme;
    string encodedUsername;
    string encodedPassword;
    string host;
    int port;
    vector<string> encodedPathSegments;
    vector<string> encodedQueryNamesAndValues;
    string encodedFragment;

    void push(string input, int pos, int limit, bool addTrailingSlash, bool alreadyEncoded);
    void pop();
    bool isDot(string input);
    bool isDotDot(string input);
    Builder & addPathSegments(string pathSegments, bool alreadyEncoded);
    void resolvePath(string input, int pos, int limit);
    void removeAllCanonicalQueryParameters(string canonicalName);

    static string canonicalizeHost(string & input, int pos, int limit);
    static int schemeDelimiterOffset (string & input, int pos, int limit);
    static int slashCount(string & input, int pos, int limit);
    static int portColonOffset (string & input, int pos, int limit);
    static int parsePort (string & input, int pos, int limit);

};//构造器


#endif //OKHTTPNATIVE_HTTPURLBUILDER_H
