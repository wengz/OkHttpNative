//
// Created by wengzc on 2019/4/22.
//

#ifndef OKHTTPNATIVE_UTIL_H
#define OKHTTPNATIVE_UTIL_H

#include <string>

using namespace std;

class HttpUrl;

unsigned long hexToDec(const char *hex, int length);

string & trim(string &);

int delimiterOffset (string input, int pos, int limit, string delimiters);

string strToLower(string str);

string strToUpper(string str);

bool compareNoCase(string str_a, string str_b);

int decodeHexDigit(char c);

int skipLeadingAsciiWhiteSpace(string & input, int pos, int limit);

int skipTrailingAsciiWhiteSpace(string & input, int pos, int limit);

bool regionMatches(const string & s1, const string & s2, bool ignore_case, int offset_1, int offset_2, int length );

string hostHeader(HttpUrl & url, bool include_default_port);

int getContentLength(string line);

#endif //OKHTTPNATIVE_UTIL_H
