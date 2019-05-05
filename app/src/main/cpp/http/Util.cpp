//
// Created by wengzc on 2019/4/22.
//

#include <algorithm>
#include <math.h>
#include <stdlib.h>

#include "Util.h"
#include "HttpUrl.h"

//十六进制转为十进制
unsigned long hexToDec(const char *hex, int length)
{
    int i;
    unsigned long rslt = 0;
    for (i = 0; i < length; i++)
    {
        int index_i_value = 0;
        if (hex[i] >= 'a' && hex[i] <= 'f'){
            index_i_value = (hex[i]-'a')+10;
        }else if (hex[i] >= 'A' && hex[i] <= 'F'){
            index_i_value = (hex[i]-'A')+10;
        }else{
            index_i_value = hex[i] - '0';
        }
        rslt += index_i_value * pow(16, length - 1 - i);
    }
    return rslt;
}

string & trim( string & s){
    if (s.empty()){
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ")+1);
    return s;
}

/**
   * Returns the index of the first character in {@code input} that contains a character in {@code
   * delimiters}. Returns limit if there is no such character.
   */
int delimiterOffset (string input, int pos, int limit, string delimiters){
    for (int i = pos; i < limit; i++){
        if (delimiters.find(input[i]) != string::npos ){
            return i;
        }
    }
    return limit;
}

string strToLower(string str) {
    std::string strTmp = str;
    transform(strTmp.begin(), strTmp.end(), strTmp.begin(), towlower);
    return strTmp;
}

string strToUpper(string str) {
    std::string strTmp = str;
    transform(strTmp.begin(), strTmp.end(), strTmp.begin(), towupper);
    return strTmp;
}

bool compareIgnoreCase(string str_a, string str_b) {
    string str1 = strToLower(str_a);
    string str2 = strToLower(str_b);
    return (str1 == str2);
}

int decodeHexDigit(char c){
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

int skipLeadingAsciiWhiteSpace(string & input, int pos, int limit) {
    for (int i = pos; i < limit; i++){
        switch (input[i]){
            case '\t':
            case '\n':
            case '\f':
            case '\r':
            case ' ':
                continue;
            default:
                return i;
        }
    }
    return limit;
}

int skipTrailingAsciiWhiteSpace(string &input, int pos, int limit) {
    for (int i = limit - 1; i >= pos; i--) {
        switch (input[i]) {
            case '\t':
            case '\n':
            case '\f':
            case '\r':
            case ' ':
                continue;
            default:
                return i + 1;
        }
    }
    return pos;
}

bool regionMatches(const string &s1, const string &s2, bool ignore_case, int offset_1, int offset_2, int length) {
    int to = offset_1;
    int po = offset_2;
    if ((offset_1 < 0 || offset_2 < 0)
        || (offset_1 + length > s1.length())
        || (offset_2 + length > s2.length()) ){
        return false;
    }
    while (length-- > 0){
        char c1 = s1[to++];
        char c2 = s2[po++];
        if (c1 == c2){
            continue;
        }
        if (ignore_case){
            char u1 = toupper(c1);
            char u2 = toupper(c2);
            if (u1 == u2){
                continue;
            }
            if (tolower(c1) == tolower(c2)){
                continue;
            }
        }
        return false;
    }
    return true;
}

string hostHeader(HttpUrl &url, bool include_default_port) {
    string result = url.getHost().find(":") != string::npos
                    ? "[" + url.getHost() + "]"
                    : url.getHost();
    if (include_default_port || url.getPort() != HttpUrl::defaultPort(url.getScheme())){
        (result += ":") += url.getPort();
    }
    return result;
}

int getContentLength(string line) {
    int start_index = line.find(':')+2;
    int end_index = line.find('\r');
    string length_str = line.substr(start_index, end_index - start_index);
    int res = atoi(length_str.c_str());
    return res;
}

int decodeHexDeigit(char c) {
    if (c >= '0' && c <= '9'){
        return c - '0';
    }
    if (c >= 'a' && c <= 'f'){
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F'){
        return c - 'A' + 10;
    }
    return -1;
}

string utilCanonicalizeHost(string host) {

    /**
     * todo
     *
     * ipv6
     *
     * java IDN.toASCII 转化域名中非ascii字符
     *
     */
    string result = strToLower(host);
    return result;
}
