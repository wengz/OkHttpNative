//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_HTTPURL_H
#define OKHTTPNATIVE_HTTPURL_H

#include <string>
#include <vector>
#include <set>

using namespace std;

class HttpUrl {

public :

    //构造器
    class Builder {

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
        int port = -1;
        vector<string> encodedPathSegments;
        vector<string> encodedQueryNamesAndValues;
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


    static int defaultPort(string scheme);
    static void pathSegmentsToString(string & out, vector<string> pathSegments);
    static void namesAndValuesToQueryString(string & out, vector<string> namesAndValues);
    static vector<string> queryStringToNamesAndValues(string encodedQuery);
    static string canonicalize(const string & input, int pos, int limit, const string encode_set,
                               bool alreadyEncoded, bool strict, bool plusIsSpace, bool asciiOnly);
    static int codePointEndIndex(const string & s, int startIndex);
    static int codePoint(const string & s, int startIndex, int endIndex);
    static bool percentEncoded(const string & s, int pos, int limit);
    static HttpUrl get(string & url);
    static string percentDecode(string & input, int pos, int limit, bool plusIsSpace);

    HttpUrl();
    HttpUrl(Builder & builder);
    string getScheme() const;
    bool isHttps();
    string encodedUsername();
    string userName();
    string encodedPassword();
    string getPassword();
    string getHost();
    int getPort();
    int pathSize();
    string encodedPath();
    vector<string> encodedPathSegments();
    vector<string> getPathSegments();
    string encodedQuery();
    string query();
    int querySize();
    string queryParameters(string name);
    set<string> queryParameterNames();
    vector<string> queryParameterValues(string name);
    string queryParameterName(int index);
    string queryParameterValue(int index);
    string encodedFragment();
    string getFragment();

private :
    static const char HEX_DIGITS[16];
    static const string USERNAME_ENCODE_SET;
    static const string PASSWORD_ENCODE_SET;
    static const string PATH_SEGMENT_ENCODE_SET;
    static const string PATH_SEGMENT_ENCODE_SET_URI;
    static const string QUERY_ENCODE_SET;
    static const string QUERY_COMPONENT_REENCODE_SET;
    static const string QUERY_COMPONENT_ENCODE_SET;
    static const string QUERY_COMPONENT_ENCODE_SET_URI;
    static const string FORM_ENCODE_SET;
    static const string FRAGMENT_ENCODE_SET;
    static const string FRAGMENT_ENCODE_SET_URI;

    string scheme;
    string username;
    string password;
    string host;
    int port;
    vector<string> pathSegments = vector<string>();
    vector<string> queryNameAndValues = vector<string>();
    string fragment;
    string url;
};


#endif //OKHTTPNATIVE_HTTPURL_H
