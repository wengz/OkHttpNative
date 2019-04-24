//
// Created by wengzc on 2019/4/24.
//

#include "MediaType.h"

const string MediaType::TOKEN = "([a-zA-Z0-9-!#$%&'*+.^_`{|}~]+)";
const string MediaType::QUOTED = "\"([^\"]*)\"";
const regex MediaType::TYPE_SUBTYPE = regex(string(TOKEN).append("/").append(TOKEN));
const regex MediaType::PARAMETER = regex(
        string(";\\s*(?:").append(TOKEN).append("=(?:").append(TOKEN).append("|").append(QUOTED).append("))?")
    );

/**
 * 返回str表示的MediaType
 */
MediaType MediaType::get(string str) throw (runtime_error){
    smatch typeSubtype;
    string type;
    string subtype;
    if (regex_search(str, typeSubtype, TYPE_SUBTYPE)){
        type = typeSubtype[1];
        subtype = typeSubtype[2];
    }else{
        throw runtime_error("MediaType 格式不正确");
    }

    string charset;
    smatch parameter;
    string::const_iterator iterator_start = str.begin();
    string::const_iterator iterator_end = str.end();
    while (regex_search(iterator_start, iterator_end, parameter, PARAMETER)){
        string name = parameter[1];
        string charsetParameter;
        if (name.empty() || (name.compare("charset") != 0)){
            continue;
        }
        string token = parameter[2];
        if (!token.empty()){
            //token为单引号包裹的字符串是非法的，去除单引号
            if(token[0] == '\'' && token[token.length()-1] == '\'' && token.length() > 2){
                charsetParameter = token.substr(1, token.length()-2);
            }else{
                charsetParameter = token;
            }

        }else{
            charsetParameter = parameter[3];
        }
        charset = charsetParameter;
        iterator_start = parameter[0].second;
    }

    return  MediaType(str, type, subtype, charset);
}

/**
 * 返回str表示的MediaType
 */
MediaType MediaType::parse(string str)  {
    try{
        return get(str);
    }catch (...){}
    return MediaType();
}

/**
 * 返回主要的媒体类型，如 "text", "image", "audio", "video", "application" 等
 */
string MediaType::getType() const {
    return type;
}

/**
 * 返回特定的子媒体类型，如 "plain" or "png", "mpeg", "mp4" or "xml"
 */
string MediaType::getSubtype() const {
    return subtype;
}

/**
 * 返回与此媒体类型关联的字符集
 */
string MediaType::getCharset() const {
    return charset;
}

/**
 * 返回与此媒体类型关联的字符集，若原字符集未指定时返回defaultValue
 */
string MediaType::getCharset(string &defaultValue) const {
    if (charset.empty()){
        return charset;
    }
    return defaultValue;
}

MediaType::MediaType(string &mediaType, string &type, string &subtype, string &charset)
    :mediaType(mediaType), type(type), subtype(subtype), charset(charset){

}

MediaType::MediaType() {

}
