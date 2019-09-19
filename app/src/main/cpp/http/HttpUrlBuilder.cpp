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
        this->scheme = "http";
    }else if (compareIgnoreCase(scheme, "https")){
        this->scheme = "https";
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

/** Adds a path segment. If the input is ".." or equivalent, this pops a path segment. */
void HttpUrl::Builder::push(string input, int pos, int limit, bool addTrailingSlash, bool alreadyEncoded) {
    string segment = canonicalize(input, pos, limit, PATH_SEGMENT_ENCODE_SET, alreadyEncoded, false, false, true);
    if (isDot(segment)){// Skip '.' path segments.
        return ;
    }
    if (isDotDot(segment)){
        pop();
        return;
    }
    if (encodedPathSegments.back().empty()){
        encodedPathSegments[encodedPathSegments.size()-1] = segment;
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

/**
     * Removes a path segment. When this method returns the last segment is always "", which means
     * the encoded path will have a trailing '/'.
     *
     * <p>Popping "/a/b/c/" yields "/a/b/". In this case the list of path segments goes from ["a",
     * "b", "c", ""] to ["a", "b", ""].
     *
     * <p>Popping "/a/b/c" also yields "/a/b/". The list of path segments goes from ["a", "b", "c"]
     * to ["a", "b", ""].
     */
void HttpUrl::Builder::pop() {
    string removed = encodedPathSegments.back();
    encodedPathSegments.pop_back();

    // Make sure the path ends with a '/' by either adding an empty string or clearing a segment.
    if (removed.empty() && !encodedPathSegments.empty()){
        //encodedPathSegments.pop_back();
        //encodedPathSegments.push_back("");
        encodedPathSegments[encodedPathSegments.size()-1] = "";
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

/**
* Returns the index of the ':' in {@code input} that is after scheme characters. Returns -1 if
* {@code input} does not have a scheme that starts at {@code pos}.
*/
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
            continue; // Scheme character. Keep going.
        }else if (c == ':'){
            return i; // Scheme prefix!
        }else {
            return -1; // Non-scheme character before the first ':'.
        }
    }
    return -1; // No ':'; doesn't start with a scheme.
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

HttpUrl::Builder & HttpUrl::Builder::parse(HttpUrl * base, string input) {
    int pos = skipLeadingAsciiWhiteSpace(input, 0, input.length());
    int limit = skipTrailingAsciiWhiteSpace(input, pos, input.length());

    // Scheme.
    int schemeDelimiterOffset = Builder::schemeDelimiterOffset(input, pos, limit);
    if (schemeDelimiterOffset != -1){
        if (regionMatches(input, "https:", true, pos, 0, 6)){
            scheme = "https";
            pos += string("https:").length();
        }else if (regionMatches(input, "http:", true, pos, 0, 5)){
            scheme = "http";
            pos += string("http:").length();
        }else {
            throw runtime_error("Expected URL scheme 'http' or 'https'");
        }

    }else if (base != nullptr){
        this->scheme = base->scheme;
    }else {
        throw runtime_error("Expected URL scheme 'http' or 'https' but no colon was found");
    }

    // Authority.
    bool hasUsername = false;
    bool hasPassword = false;
    int slash_count = HttpUrl::Builder::slashCount(input, pos, limit);
    if (slash_count >= 2 || base == nullptr || base->scheme != this->scheme){
        // Read an authority if either:
        //  * The input starts with 2 or more slashes. These follow the scheme if it exists.
        //  * The input scheme exists and is different from the base URL's scheme.
        //
        // The structure of an authority is:
        //   username:password@host:port
        //
        // Username, password and port are optional.
        //   [username[:password]@]host[:port]

        pos += slash_count;

        while (true){
            int componentDelimiterOffset = delimiterOffset(input, pos, limit, "@/\\?#");
            int c = componentDelimiterOffset != limit ?
                    input[componentDelimiterOffset] : -1;

            switch (c){
                case '@':
                    // User info precedes.
                    if (!hasPassword){
                        int passwordColonOffset = delimiterOffset(input, pos, componentDelimiterOffset, ":");
                        string canonicalUsername = canonicalize(input, pos, passwordColonOffset, USERNAME_ENCODE_SET, true, false, false, true);
                        this->encodedUsername = hasUsername ?
                                this->encodedUsername + "%40" +canonicalUsername
                                : canonicalUsername;

                        if (passwordColonOffset != componentDelimiterOffset){
                            hasPassword = true;
                            this->encodedPassword =
                                    canonicalize(input, passwordColonOffset + 1, componentDelimiterOffset, PASSWORD_ENCODE_SET,
                                            true, false, false, true);
                        }
                        hasUsername = true;

                    }else {
                        this->encodedPassword = this->encodedPassword + "%40" +
                            canonicalize(input, pos, componentDelimiterOffset, PASSWORD_ENCODE_SET, true, false, false, true);
                    }
                    pos = componentDelimiterOffset +1;
                    break;
                case -1:
                case '/':
                case '\\':
                case '?':
                case '#':
                    // Host info precedes.

                    int portColonOffset = Builder::portColonOffset(input, pos, componentDelimiterOffset);
                    if (portColonOffset + 1 < componentDelimiterOffset){
                        host = canonicalizeHost(input, pos, portColonOffset);
                        port = parsePort(input, portColonOffset + 1, componentDelimiterOffset);
                        if (port == -1){
                            throw runtime_error("Invalid URL port");
                        }
                    } else {
                        host = canonicalizeHost(input, pos, portColonOffset);
                        port = defaultPort(scheme);
                    }
                    if (host.empty()){
                        throw runtime_error("Invalid URL host");
                    }
                    pos = componentDelimiterOffset;
                    goto authority;
            }

            authority: break;
        }

    }else{
        // This is a relative link. Copy over all authority components. Also maybe the path & query.
        this->encodedUsername = base->encodedUsername();
        this->encodedPassword = base->encodedPassword();
        this->host = base->host;
        this->port = base->port;
        this->encodedPathSegments.clear();
        vector<string> baseEncodedPathSegments = base->encodedPathSegments();
        this->encodedPathSegments.insert(encodedPathSegments.begin(), baseEncodedPathSegments.begin(), baseEncodedPathSegments.end());
        if (pos == limit || input[pos] == '#'){
            encodedQuery(base->encodedQuery());
        }
    }

    // Resolve the relative path.
    int pathDelimiterOffset = delimiterOffset(input, pos, limit, "?#");
    resolvePath(input, pos, pathDelimiterOffset);
    pos = pathDelimiterOffset;

    //Query
    if (pos < limit && input[pos] == '?'){
        int queryDelimiterOffset = delimiterOffset(input, pos, limit, "#");
        this->encodedQueryNamesAndValues = queryStringToNamesAndValues(
                canonicalize(input, pos+1, queryDelimiterOffset, QUERY_ENCODE_SET, true, false, true, true)
                );
        pos = queryDelimiterOffset;
    }

    //Fragment
    if (pos < limit && input[pos] == '#'){
        this->encodedFragment = canonicalize(input, pos + 1, limit,  FRAGMENT_ENCODE_SET, true, false, false, false);
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

HttpUrl::Builder & HttpUrl::Builder::addPathSegments(string pathSegments, bool alreadyEncoded) {
    int offset = 0;
    do {
        int segmentEnd = delimiterOffset(pathSegments, offset, pathSegments.size(), "/\\");
        bool addTrailingSlash = segmentEnd < pathSegments.size();
        push(pathSegments, offset, segmentEnd, addTrailingSlash, alreadyEncoded);
        offset = segmentEnd + 1;
    }while( offset <= pathSegments.size());
    return * this;
}

HttpUrl::Builder &HttpUrl::Builder::addPathSegment(string pathSegment) {
    push(pathSegment, 0, pathSegment.size(), false, false);
    return * this;
}

HttpUrl::Builder &HttpUrl::Builder::addEncodedPathSegment(string encodedPathSegment) {
    push(encodedPathSegment, 0, encodedPathSegment.size(), false, true);
    return * this;
}

HttpUrl::Builder &HttpUrl::Builder::addPathSegments(string pathSegments) {
    return addPathSegments(pathSegments, false);
}

HttpUrl::Builder &HttpUrl::Builder::addEncodedPathSegments(string pathSegments) {
    return addPathSegments(pathSegments, true);
}

HttpUrl::Builder & HttpUrl::Builder::setPathSegment(int index, string pathSegment) {
    string canonicalPathSegment = canonicalize(pathSegment, 0, pathSegment.size(), PATH_SEGMENT_ENCODE_SET,
            false, false, false, true);
    if (isDot(canonicalPathSegment) || isDotDot(canonicalPathSegment)){
        throw runtime_error("unexpected path segment: "+pathSegment);
    }
    encodedPathSegments[index] = canonicalPathSegment;
    return * this;
}

HttpUrl::Builder & HttpUrl::Builder::setEncodedPathSegment(int index, string encodedPathSegment) {
    string canonicalPathSegment = canonicalize(encodedPathSegment, 0, encodedPathSegment.size(), PATH_SEGMENT_ENCODE_SET,
            true, false, false, true);
    if (isDot(canonicalPathSegment) || isDotDot(canonicalPathSegment)){
        throw runtime_error("unexpected path segment: "+encodedPathSegment);
    }
    return * this;
}

HttpUrl::Builder &HttpUrl::Builder::removePathSegment(int index) {
    encodedPathSegments.erase(encodedPathSegments.begin()+index);
    if (encodedPathSegments.empty()){
        encodedPathSegments.push_back("");// Always leave at least one '/'.
    }
    return * this;
}

void HttpUrl::Builder::resolvePath(string input, int pos, int limit) {
    // Read a delimiter.
    if (pos == limit){
        // Empty path: keep the base path as-is.
        return;
    }
    char c = input[pos];
    if (c == '/' || c == '\\'){
        // Absolute path: reset to the default "/".
        encodedPathSegments.clear();
        encodedPathSegments.push_back("");
    }else{
        // Relative path: clear everything after the last '/'.
        encodedPathSegments[encodedPathSegments.size()-1] = "";
    }

    // Read path segments.
    for (int i = pos; i < limit;){
        int pathSegmentDelimiterOffset = delimiterOffset(input, i, limit, "/\\");
        bool segmentHasTrailingSlash = pathSegmentDelimiterOffset < limit;
        push(input, i, pathSegmentDelimiterOffset, segmentHasTrailingSlash, true);
        i = pathSegmentDelimiterOffset;
        if (segmentHasTrailingSlash){
            i++;
        }
    }
}

HttpUrl::Builder &HttpUrl::Builder::encodePath(string encodedPath) {
    if (!encodedPath[0] == '/'){
        throw runtime_error("unexpected encodedPath: "+encodedPath);
    }
    resolvePath(encodedPath, 0, encodedPath.size());
    return * this;
}

HttpUrl::Builder &HttpUrl::Builder::query(string query) {
    if (query.empty()){
        this->encodedQueryNamesAndValues = queryStringToNamesAndValues(canonicalize(query, 0, query.size(), QUERY_ENCODE_SET, false, false, true, true));
    }
    return * this;
}

HttpUrl::Builder &HttpUrl::Builder::encodedQuery(string query) {
    if (query.empty()){
        this->encodedQueryNamesAndValues = queryStringToNamesAndValues(canonicalize(query, 0, query.size(), QUERY_ENCODE_SET, true, false, true, true));
    }
    return * this;
}

/** Encodes the query parameter using UTF-8 and adds it to this URL's query string. */
HttpUrl::Builder &HttpUrl::Builder::addQueryParameter(string name, string value) {
    if (name.empty()){
        throw runtime_error("name is empty");
    }
    encodedQueryNamesAndValues.push_back(
            canonicalize(name, 0, name.size(), QUERY_COMPONENT_ENCODE_SET, false, false, true, true)
            );
    if (!value.empty()){
        encodedQueryNamesAndValues.push_back(canonicalize(value, 0, value.size(), QUERY_COMPONENT_ENCODE_SET, false, false, true, true));
    }else{
        encodedQueryNamesAndValues.push_back("");
    }
    return * this;
}

HttpUrl::Builder &
HttpUrl::Builder::addEncodedQueryParameter(string encodedName, string encodedValue) {
    if (encodedName.empty()){
        throw runtime_error("encodedName is empty");
    }
    encodedQueryNamesAndValues.push_back(
            canonicalize(encodedName, 0, encodedName.size(), QUERY_COMPONENT_ENCODE_SET, true, false, true, true)
    );
    if (!encodedValue.empty()){
        encodedQueryNamesAndValues.push_back(canonicalize(encodedValue, 0, encodedValue.size(), QUERY_COMPONENT_ENCODE_SET, true, false, true, true));
    }else{
        encodedQueryNamesAndValues.push_back("");
    }
    return * this;
}

void HttpUrl::Builder::removeAllCanonicalQueryParameters(string canonicalName) {
    for (int i = encodedQueryNamesAndValues.size() - 2; i >= 0; i -= 2){
        if (canonicalName == encodedQueryNamesAndValues[i]){
            encodedQueryNamesAndValues.erase(encodedQueryNamesAndValues.begin()+i+1);
            encodedQueryNamesAndValues.erase(encodedQueryNamesAndValues.begin()+i);
            if (encodedQueryNamesAndValues.empty()){
                return;
            }
        }
    }
}

HttpUrl::Builder &HttpUrl::Builder::removeAllQueryParameters(string name) {
    if (name.empty()){
        throw runtime_error("name is empty");
    }
    if (encodedQueryNamesAndValues.empty()){
        return * this;
    }
    string nameToRemove = canonicalize(name, 0, name.size(), QUERY_COMPONENT_ENCODE_SET, false, false, true, true);
    removeAllCanonicalQueryParameters(nameToRemove);
    return * this;
}

HttpUrl::Builder &HttpUrl::Builder::removeAllEncodedQueryParameters(string encodedName) {
    if (encodedName.empty()){
        throw runtime_error("encodedName is empty");
    }
    if (encodedQueryNamesAndValues.empty()){
        return * this;
    }
    string nameToRemove = canonicalize(encodedName, 0, encodedName.size(), QUERY_COMPONENT_ENCODE_SET, true, false, true, true);
    removeAllCanonicalQueryParameters(nameToRemove);
    return * this;
}

HttpUrl::Builder &HttpUrl::Builder::setQueryParameter(string name, string value) {
    removeAllQueryParameters(name);
    addQueryParameter(name, value);
    return * this;
}

HttpUrl::Builder &
HttpUrl::Builder::setEncodedQueryParameter(string encodedName, string encodedValue) {
    removeAllEncodedQueryParameters(encodedName);
    addEncodedQueryParameter(encodedName, encodedValue);
    return * this;
}

HttpUrl::Builder &HttpUrl::Builder::setFragment(string fragment) {
    if (!fragment.empty()){
        this->encodedFragment = canonicalize(fragment, 0, fragment.size(), FRAGMENT_ENCODE_SET, false, false, false, false);
    }else{
        this->encodedFragment = fragment;
    }
    return * this;
}

HttpUrl::Builder &HttpUrl::Builder::seEncodedtFragment(string fragment) {
    if (!fragment.empty()){
        this->encodedFragment = canonicalize(fragment, 0, fragment.size(), FRAGMENT_ENCODE_SET, true, false, false, false);
    }else{
        this->encodedFragment = fragment;
    }
    return * this;
}

/**
* Re-encodes the components of this URL so that it satisfies (obsolete) RFC 2396, which is
* particularly strict for certain components.
 */
HttpUrl::Builder &HttpUrl::Builder::reencodeForUri() {
    for (int i = 0, size = encodedPathSegments.size(); i < size; i++){
        string pathSegment = encodedPathSegments[i];
        encodedPathSegments[i] = canonicalize(pathSegment, 0, pathSegment.size(), PATH_SEGMENT_ENCODE_SET_URI,
                true, true, false, true);
    }
    if (!encodedQueryNamesAndValues.empty()){
        for (int i = 0, size = encodedQueryNamesAndValues.size(); i < size; i++){
            string component = encodedQueryNamesAndValues[i];
            if (!component.empty()){
                encodedQueryNamesAndValues[i] = canonicalize(component, 0, component.size(), QUERY_COMPONENT_ENCODE_SET_URI,
                        true, true, true, true);
            }
        }
    }
    if (!encodedFragment.empty()){
        encodedFragment = canonicalize(encodedFragment, 0, encodedFragment.size(), FRAGMENT_ENCODE_SET_URI,
                true, true, false, false);
    }
    return * this;
}

string HttpUrl::Builder::toString() {
    string result;
    if (!scheme.empty()){
        result += scheme;
        result += "://";
    }else{
        result += "//";
    }

    if (!encodedUsername.empty() || !encodedPassword.empty()){
        result += encodedUsername;
        if (!encodedPassword.empty()){
            result += ':';
            result += encodedPassword;
        }
        result += '@';
    }

    if (!host.empty()){
        if (host.find(":") != string::npos){
            // Host is an IPv6 address.
            result += "[";
            result += host;
            result += "]";

        }else{
            result += host;
        }
    }

    if (port != -1 || !scheme.empty()){
        int effectivePort = Builder::effectivePort();
        if (scheme.empty() || effectivePort != defaultPort(scheme)){
            result += ":";
            //fixme int需转成string？？
            result += effectivePort;
        }
    }

    pathSegmentsToString(result, encodedPathSegments);

    if (!encodedQueryNamesAndValues.empty()){
        result += '?';
        namesAndValuesToQueryString(result, encodedQueryNamesAndValues);
    }

    if (!encodedFragment.empty()){
        result += '#';
        result += encodedFragment;
    }
    return result;
}




