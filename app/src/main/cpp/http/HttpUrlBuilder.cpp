//
// Created by wengzc on 2019/4/30.
//

#include "HttpUrlBuilder.h"
#include "Util.h"

HttpUrl::Builder::Builder() : encodedUsername(""), encodedPassword(""), port(-1){
    encodedPathSegments.push_back("");
}

HttpUrl::Builder & HttpUrl::Builder::setScheme(string scheme) {
    if (compareIgnoreCase(scheme, "http")){
        scheme = "http";
    }else if (compareIgnoreCase(scheme, "https")){
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
    string encoded = canonicalizeHost(host, 0, host.size());
    this->host = encoded;
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
    return input == "." || compareIgnoreCase(input, "%2e");
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
           || compareIgnoreCase(input, "%2e.")
           || compareIgnoreCase(input, ".%2e")
           || compareIgnoreCase(input, "%2e%2e");
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

HttpUrl HttpUrl::Builder::build() {
    return  HttpUrl(* this);
}

string HttpUrl::Builder::getHost() {
    return host;
}

string HttpUrl::Builder::getScheme() {
    return scheme;
}

string HttpUrl::Builder::canonicalizeHost(string &input, int pos, int limit) {
    string percentDecoded = percentDecode(input, pos, limit, false);
    return utilCanonicalizeHost(percentDecoded);
}
