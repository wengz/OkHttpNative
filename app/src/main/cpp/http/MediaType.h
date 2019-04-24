//
// Created by wengzc on 2019/4/24.
//

#ifndef OKHTTPNATIVE_MEDIATYPE_H
#define OKHTTPNATIVE_MEDIATYPE_H

#include <string>
#include <regex>

using namespace std;

/**
 * <a href="http://tools.ietf.org/html/rfc2045">RFC 2045</a> 描述的媒体类型，用来
 * 标识HTTP请求和响应体中的内容类型
 */
class MediaType {

public :

    static MediaType get(string str) throw (runtime_error);

    static MediaType parse(string string) ;

    string getType() const;

    string getSubtype() const;

    string getCharset() const;

    string getCharset(string & defaultValue) const;

private :
    static const string TOKEN;
    static const string QUOTED;

    static const regex TYPE_SUBTYPE;
    static const regex PARAMETER;

    const string mediaType;
    const string type;
    const string subtype;
    const string charset;

    MediaType();
    MediaType (string & mediaType, string &type, string &subtype, string &charset);
};


#endif //OKHTTPNATIVE_MEDIATYPE_H
