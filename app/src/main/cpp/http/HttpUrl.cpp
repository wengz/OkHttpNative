//
// Created by wengzc on 2019/4/22.
//

#include "HttpUrl.h"
#include "Util.h"

const char HttpUrl::HEX_DIGITS[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
const string HttpUrl::USERNAME_ENCODE_SET = " \"':;<=>@[]^`{}|/\\?#";
const string HttpUrl::PASSWORD_ENCODE_SET = " \"':;<=>@[]^`{}|/\\?#";
const string HttpUrl::PATH_SEGMENT_ENCODE_SET = " \"<>^`{}|/\\?#";
const string HttpUrl::PATH_SEGMENT_ENCODE_SET_URI = "[]";
const string HttpUrl::QUERY_ENCODE_SET = " \"'<>#";
const string HttpUrl::QUERY_COMPONENT_REENCODE_SET = " \"'<>#&=";
const string HttpUrl::QUERY_COMPONENT_ENCODE_SET = " !\"#$&'(),/:;<=>?@[]\\^`{|}~";
const string HttpUrl::QUERY_COMPONENT_ENCODE_SET_URI = "\\^`{|}";
const string HttpUrl::FORM_ENCODE_SET = " \"':;<=>@[]^`{}|/\\?#&!$(),~";
const string HttpUrl::FRAGMENT_ENCODE_SET = "";
const string HttpUrl::FRAGMENT_ENCODE_SET_URI = " \"#<>\\^`{|}";

string HttpUrl::getScheme() const {
    return scheme;
}

bool HttpUrl::isHttps() {
    return scheme.compare("https") == 0;
}

string HttpUrl::encodedUsername() {
    if (username.empty()){
        return "";
    }
    int username_start = scheme.size() + 3;
    int username_end = delimiterOffset(url, username_start, url.size(), ":@");
    return url.substr(username_start, username_end - username_start);
}

string HttpUrl::userName() {
    return username;
}

string HttpUrl::encodedPassword() {
    if (password.empty()){
        return "";
    }
    int password_start = url.find(':', scheme.size()+3)+1;
    int password_end = url.find('@');
    return url.substr(password_start, password_end - password_start);
}

string HttpUrl::getPassword() {
    return password;
}

string HttpUrl::getHost() {
    return host;
}

int HttpUrl::getPort() {
    return port;
}

int HttpUrl::defaultPort(string scheme) {
    if (scheme.compare("http") == 0){
        return 80;
    }else if (scheme.compare("https") == 0){
        return 443;
    }
    return -1;
}

int HttpUrl::pathSize() {
    return pathSegments.size();
}

string HttpUrl::encodedPath() {
    int path_start = url.find('/', scheme.size() + 3);
    int path_end = delimiterOffset(url, path_start, url.length(), "?#");
    return url.substr(path_start, (path_end - path_start));
}

void HttpUrl::pathSegmentsToString(string & out, vector<string> pathSegments) {
    for (int i = 0; i < pathSegments.size(); i++){
        out += "/";
        out += pathSegments[i];
    }
}

vector<string> HttpUrl::encodedPathSegments() {
    int path_start = url.find('/', scheme.length()+3);
    int path_end = delimiterOffset(url, path_start, url.length(), "?#");
    vector<string> result;
    for (int i =  path_start; i < path_end;){
        i++;
        int segment_end = delimiterOffset(url, i, path_end, "/");
        result.push_back(url.substr(i, (segment_end - i)));
        i = segment_end;
    }
    return result;
}

vector<string> HttpUrl::getPathSegments() {
    return pathSegments;
}

string HttpUrl::encodedQuery() {
    if (queryNameAndValues.size() == 0){
        return "";
    }
    int query_start = url.find('?')+1;
    int query_end = delimiterOffset(url, query_start, url.length(), "#");
    return url.substr(query_start, query_end - query_start);
}

void HttpUrl::namesAndValuesToQueryString(string & out, vector<string> namesAndValues) {
    for (int i = 0, size = namesAndValues.size(); i < size; i += 2){
        string name = namesAndValues[i];
        string value = namesAndValues[i+1];
        if (i > 0){
            out += "&";
        }
        out += name;
        if (!value.empty()){
            out += "=";
            out += value;
        }
    }
}

vector<string> HttpUrl::queryStringToNamesAndValues(string encodedQuery) {
    vector<string> result;
    for ( int pos = 0; pos <= encodedQuery.length(); ){
        int ampersandOffset = encodedQuery.find('&', pos);
        if (ampersandOffset == string::npos){
            ampersandOffset = encodedQuery.length();
        }
        int equalsOffset = encodedQuery.find('=', pos);
        if (equalsOffset == string::npos || equalsOffset > ampersandOffset){
            result.push_back(encodedQuery.substr(pos, ampersandOffset-pos));
            result.push_back("");

        }else{
            result.push_back(encodedQuery.substr(pos, equalsOffset-pos));
            result.push_back(encodedQuery.substr(equalsOffset + 1, ampersandOffset - (equalsOffset + 1) ));
        }
        pos = ampersandOffset + 1;

    }
    return result;
}

string HttpUrl::query() {
    if (queryNameAndValues.size() == 0){
        return "";
    }
    string result;
    namesAndValuesToQueryString(result, queryNameAndValues);
    return result;
}

int HttpUrl::querySize() {
    return queryNameAndValues.size() /2;
}

string HttpUrl::queryParameters(string name) {
    if (queryNameAndValues.empty()){
        return "";
    }
    for ( int i = 0, size = queryNameAndValues.size(); i < size; i += 2 ){
        if (name.compare(queryNameAndValues[i]) == 0){
            return queryNameAndValues[i+1];
        }
    }
    return "";
}

set<string> HttpUrl::queryParameterNames() {
    set<string> result;
    if (queryNameAndValues.empty()){
        return result;
    }
    for ( int i = 0, size = queryNameAndValues.size(); i < size; i += 2 ){
        string key = queryNameAndValues[i];
        result.insert(key);
    }
    return result;
}

vector<string> HttpUrl::queryParameterValues(string name) {
    vector<string> result;
    if (queryNameAndValues.empty()){
        return result;
    }
    for (int i = 0, size = queryNameAndValues.size(); i < size; i += 2){
        if (name.compare(queryNameAndValues[i]) == 0){
            result.push_back(queryNameAndValues[i+1]);
        }
    }
    return result;
}

string HttpUrl::queryParameterName(int index) {
    return queryNameAndValues[index*2];
}

string HttpUrl::queryParameterValue(int index) {
    return queryNameAndValues[index * 2 + 1];
}

string HttpUrl::encodedFragment() {
    if (fragment.empty()){
        return "";
    }
    int fragment_start = url.find('#') + 1;
    return url.substr(fragment_start);
}

string HttpUrl::getFragment() {
    return fragment;
}

string HttpUrl::canonicalize(const string &input, int pos, int limit, const string encode_set,
                              bool already_encoded, bool strict, bool plus_is_space,
                              bool ascii_only) {
    int code_point;
    int code_point_end_index;
    string out;
    for ( int i = pos; i < limit; ){
        code_point_end_index = HttpUrl::codePointEndIndex(input, i);
        code_point = HttpUrl::codePoint(input, i, code_point_end_index);
        if (already_encoded
            && (code_point == '\t' || code_point == '\n' || code_point == '\f' || code_point == '\r')){
            //skip this character
        }else if (code_point == '+' && plus_is_space){
            out.append( already_encoded ? "+" : "%2b" );

        }else if (code_point < 0x20
                  || code_point == 0x7f
                  || code_point >= 0x80 && ascii_only
                  || encode_set.find(code_point) != string::npos
                  || code_point == '%' && (!already_encoded || ( strict && !percentEncoded(input, i, limit)))){
            for (int j = i; j <= code_point_end_index; j++){
                int b = input[j] & 0xff;
                out.append("%");
                out.append(1, HEX_DIGITS[(b >> 4) & 0xf]);
                out.append(1, HEX_DIGITS[b & 0xf]);
            }
        }else{
            string code_point_range = input.substr(i, code_point_end_index - i +1);
            out.append(code_point_range);
        }
        i = code_point_end_index + 1;
    }
    return out;
}


//utf-8码点结尾字节索引
int HttpUrl::codePointEndIndex(const string & s, int startIndex) {
    char start_char = s[startIndex];
    if ( (((start_char >> 7) & 0x0001) ^ 0x0000) == 0 ){//0xxxxxxx
        return startIndex;
    }else if ((((start_char >> 5) & 0x0007) ^ 0x0006) == 0){//110xxxxx 10xxxxxx
        return startIndex + 1;
    }else if ((((start_char >> 4) & 0x000F) ^ 0x000E) == 0){//1110xxxx 10xxxxxx 10xxxxxx
        return startIndex + 2;
    }else if ((((start_char >> 3) & 0x001F) ^ 0x001E) == 0){//11110xxx10xxxxxx10xxxxxx10xxxxxx
        return start_char + 3;
    }
    return 0;
}

int HttpUrl::codePoint(const string & s, int startIndex, int endIndex) {
    unsigned  int cp = 0;
    int byte_count = endIndex-startIndex +1;
    char c1, c2, c3, c4;
    switch (byte_count){//按所占字节数处理
        case 1:
            c1 = s[startIndex];
            cp = c1;
            break;
        case 2:
            c1 = s[startIndex];
            c2 = s[startIndex+1];
            cp |= ((c1 & 0x1F) << 6);
            cp |= (c2 & 0x3F);
            break;
        case 3:
            c1 = s[startIndex];
            c2 = s[startIndex+1];
            c3 = s[startIndex+2];
            cp |= ((c1 & 0xF) << 12);
            cp |= ((c2 & 0x3F) << 6);
            cp |= (c3 & 0x3F);
            break;
        case 4:
            c1 = s[startIndex];
            c2 = s[startIndex+1];
            c3 = s[startIndex+2];
            c4 = s[startIndex+3];
            cp |= ((c1 & 0x7) << 18);
            cp |= ((c2 & 0x3F) <<12);
            cp |= ((c3 & 0x3F) <<6);
            cp |= (c4 & 0x3F);
            break;
    }
    return cp;
}

bool HttpUrl::percentEncoded(const string &s, int pos, int limit) {
    return pos + 2 < limit
           && s[pos] == '%'
           && decodeHexDigit(s[pos+1]) != -1
           && decodeHexDigit(s[pos+2]) != -1;
}

HttpUrl * HttpUrl::get(string &url) {
    return Builder().parse(url).build();
}

HttpUrl::HttpUrl(HttpUrl::Builder & bd) {
    host = bd.getHost();
    port = bd.effectivePort();
    scheme = bd.getScheme();
}

HttpUrl::HttpUrl() {

}

string HttpUrl::percentDecode(string &input, int pos, int limit, bool plusIsSpace) {


    return std::string();
}


HttpUrl::Builder::Builder(){
    encodedPathSegments.push_back("");
}

HttpUrl::Builder & HttpUrl::Builder::setScheme(string scheme) {
    if (compareNoCase(scheme, "http")){
        scheme = "http";
    }else if (compareNoCase(scheme, "https")){
        scheme = "https";
    }else {
        throw runtime_error("unexpected scheme: " + scheme);
    }
    return * this;
}

HttpUrl::Builder & HttpUrl::Builder::setUsername(string username) {
    encodedUsername = canonicalize(username, 0, username.length(), USERNAME_ENCODE_SET, false, false, false, true);
    return * this;
}

HttpUrl::Builder & HttpUrl::Builder::setEncodedUsername(string encodedUsername) {
    encodedUsername = canonicalize(encodedUsername, 0, encodedUsername.length(), USERNAME_ENCODE_SET, true, false, false, true);
    return * this;
}

HttpUrl::Builder & HttpUrl::Builder::setPassword(string password) {
    encodedPassword = canonicalize(password, 0, password.length(), PASSWORD_ENCODE_SET, false, false, false, true);
    return  * this;
}

HttpUrl::Builder & HttpUrl::Builder::setEncodedPassword(string encodedPassword) {
    encodedPassword = canonicalize(encodedPassword, 0, encodedPassword.length(), PASSWORD_ENCODE_SET, true, false, false, true);
    return * this;
}

HttpUrl::Builder & HttpUrl::Builder::setHost(string host) {
    //canonical for host
    this->host = host;
    return * this;
}

HttpUrl::Builder & HttpUrl::Builder::setPort(int port) {
    if (port <= 0 || port > 65535){
        throw runtime_error("unexpected port: " + port);
    }
    this->port = port;
    return * this;
}

int HttpUrl::Builder::effectivePort() {
    return port != -1 ? port : defaultPort(scheme);
}

void HttpUrl::Builder::push(string input, int pos, int limit, bool addTrailingSlash, bool alreadyEncoded) {
    string segment = canonicalize(input, pos, limit, PATH_SEGMENT_ENCODE_SET, alreadyEncoded, false, false, true);
    if (isDot(segment)){
        return ;
    }
    if (isDotDot(segment)){
        pop();
        return;
    }
    if (encodedPathSegments.back().empty()){
        encodedPathSegments.pop_back();
        encodedPathSegments.push_back(segment);
    }else{
        encodedPathSegments.push_back(segment);
    }
    if (addTrailingSlash){
        encodedPathSegments.push_back("");
    }
}

bool HttpUrl::Builder::isDot(string input) {
    return input == "." || compareNoCase(input, "%2e");
}

void HttpUrl::Builder::pop() {
    string removed = encodedPathSegments.back();
    encodedPathSegments.pop_back();

    if (removed.empty() && !encodedPathSegments.empty()){
        encodedPathSegments.pop_back();
        encodedPathSegments.push_back("");
    }else{
        encodedPathSegments.push_back("");
    }
}

bool HttpUrl::Builder::isDotDot(string input) {
    return input == ".."
           || compareNoCase(input, "%2e.")
           || compareNoCase(input, ".%2e")
           || compareNoCase(input, "%2e%2e");
}

int HttpUrl::Builder::schemeDelimiterOffset(string & input, int pos, int limit) {
    if (limit - pos < 2){
        return -1;
    }
    char c0 = input[pos];
    if ( (c0 < 'a' || c0 > 'z') && ( c0 < 'A' || c0 > 'Z' ) ){//Not a scheme start char.
        return -1;
    }
    for (int i = pos + 1; i < limit; i++){
        char c = input[i];

        if ((c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9')
            || c == '+'
            || c == '-'
            || c == '.'){
            continue;
        }else if (c == ':'){
            return i;
        }else {
            return -1;
        }
    }
    return -1;
}

int HttpUrl::Builder::slashCount(string & input, int pos, int limit) {
    int slash_count = 0;
    while (pos < limit){
        char c = input[pos];
        if (c == '\\' || c == '/'){
            slash_count++;
            pos++;
        }else{
            break;
        }
    }
    return slash_count;
}

int HttpUrl::Builder::portColonOffset(string &input, int pos, int limit) {
    for (int i = pos; i < limit; i++){
        switch (input[i]){
            case '[':
                while (++i < limit){
                    if (input[i] == ']'){
                        break;
                    }
                }
                break;
            case ':':
                return i;
        }
    }
    return limit;
}

int HttpUrl::Builder::parsePort(string &input, int pos, int limit) {
    string port_str =  canonicalize(input, pos, limit, "", false, false, false, true);
    int i = atoi(port_str.c_str());
    if (i > 0 && i <= 65535){
        return i;
    }else{
        return -1;
    }
}

HttpUrl::Builder & HttpUrl::Builder::parse(string input) {
    int pos = skipLeadingAsciiWhiteSpace(input, 0, input.length());
    int limit = skipTrailingAsciiWhiteSpace(input, pos, input.length());

    int schemme_dlt_offset = schemeDelimiterOffset(input, pos, limit);
    if (schemme_dlt_offset != -1){
        if (regionMatches(input, "https:", true, pos, 0, 6)){
            scheme = "https";
            pos += string("https:").length();
        }else if (regionMatches(input, "http:", true, pos, 0, 5)){
            scheme = "http";
            pos += string("http:").length();
        }else {
            throw runtime_error("Expected URL scheme 'http' or 'https'");
        }

    }else {
        throw runtime_error("Expected URL scheme 'http' or 'https' but no colon was found");
    }

    int slash_count = HttpUrl::Builder::slashCount(input, pos, limit);
    if (slash_count >= 2){
        pos += slash_count;
        host = input.substr(pos, string::npos);

    }else{
        throw runtime_error("Expected URL ");
    }
    return * this;
}

HttpUrl * HttpUrl::Builder::build() {
    return new HttpUrl(* this);
}

string HttpUrl::Builder::getHost() {
    return host;
}

string HttpUrl::Builder::getScheme() {
    return scheme;
}

string HttpUrl::Builder::canonicalizeHost(string &input, int pos, int limit) {
    return std::string();
}
