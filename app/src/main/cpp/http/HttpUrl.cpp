//
// Created by wengzc on 2019/4/22.
//

#include "HttpUrl.h"
#include "Util.h"
#include "HttpUrlBuilder.h"


/**
 * A uniform resource locator (URL) with a scheme of either {@code http} or {@code https}. Use this
 * class to compose and decompose Internet addresses. For example, this code will compose and print
 * a URL for Google search: <pre>   {@code
 *
 *   HttpUrl url = new HttpUrl.Builder()
 *       .scheme("https")
 *       .host("www.google.com")
 *       .addPathSegment("search")
 *       .addQueryParameter("q", "polar bears")
 *       .build();
 *   System.out.println(url);
 * }</pre>
 *
 * which prints: <pre>   {@code
 *
 *     https://www.google.com/search?q=polar%20bears
 * }</pre>
 *
 * As another example, this code prints the human-readable query parameters of a Twitter search:
 * <pre>   {@code
 *
 *   HttpUrl url = HttpUrl.parse("https://twitter.com/search?q=cute%20%23puppies&f=images");
 *   for (int i = 0, size = url.querySize(); i < size; i++) {
 *     System.out.println(url.queryParameterName(i) + ": " + url.queryParameterValue(i));
 *   }
 * }</pre>
 *
 * which prints: <pre>   {@code
 *
 *   q: cute #puppies
 *   f: images
 * }</pre>
 *
 * In addition to composing URLs from their component parts and decomposing URLs into their
 * component parts, this class implements relative URL resolution: what address you'd reach by
 * clicking a relative link on a specified page. For example: <pre>   {@code
 *
 *   HttpUrl base = HttpUrl.parse("https://www.youtube.com/user/WatchTheDaily/videos");
 *   HttpUrl link = base.resolve("../../watch?v=cbP2N1BQdYc");
 *   System.out.println(link);
 * }</pre>
 *
 * which prints: <pre>   {@code
 *
 *   https://www.youtube.com/watch?v=cbP2N1BQdYc
 * }</pre>
 *
 * <h3>What's in a URL?</h3>
 *
 * A URL has several components.
 *
 * <h4>Scheme</h4>
 *
 * <p>Sometimes referred to as <i>protocol</i>, A URL's scheme describes what mechanism should be
 * used to retrieve the resource. Although URLs have many schemes ({@code mailto}, {@code file},
 * {@code ftp}), this class only supports {@code http} and {@code https}. Use {@link URI
 * java.net.URI} for URLs with arbitrary schemes.
 *
 * <h4>Username and Password</h4>
 *
 * <p>Username and password are either present, or the empty string {@code ""} if absent. This class
 * offers no mechanism to differentiate empty from absent. Neither of these components are popular
 * in practice. Typically HTTP applications use other mechanisms for user identification and
 * authentication.
 *
 * <h4>Host</h4>
 *
 * <p>The host identifies the webserver that serves the URL's resource. It is either a hostname like
 * {@code square.com} or {@code localhost}, an IPv4 address like {@code 192.168.0.1}, or an IPv6
 * address like {@code ::1}.
 *
 * <p>Usually a webserver is reachable with multiple identifiers: its IP addresses, registered
 * domain names, and even {@code localhost} when connecting from the server itself. Each of a
 * webserver's names is a distinct URL and they are not interchangeable. For example, even if {@code
 * http://square.github.io/dagger} and {@code http://google.github.io/dagger} are served by the same
 * IP address, the two URLs identify different resources.
 *
 * <h4>Port</h4>
 *
 * <p>The port used to connect to the webserver. By default this is 80 for HTTP and 443 for HTTPS.
 * This class never returns -1 for the port: if no port is explicitly specified in the URL then the
 * scheme's default is used.
 *
 * <h4>Path</h4>
 *
 * <p>The path identifies a specific resource on the host. Paths have a hierarchical structure like
 * "/square/okhttp/issues/1486" and decompose into a list of segments like ["square", "okhttp",
 * "issues", "1486"].
 *
 * <p>This class offers methods to compose and decompose paths by segment. It composes each path
 * from a list of segments by alternating between "/" and the encoded segment. For example the
 * segments ["a", "b"] build "/a/b" and the segments ["a", "b", ""] build "/a/b/".
 *
 * <p>If a path's last segment is the empty string then the path ends with "/". This class always
 * builds non-empty paths: if the path is omitted it defaults to "/". The default path's segment
 * list is a single empty string: [""].
 *
 * <h4>Query</h4>
 *
 * <p>The query is optional: it can be null, empty, or non-empty. For many HTTP URLs the query
 * string is subdivided into a collection of name-value parameters. This class offers methods to set
 * the query as the single string, or as individual name-value parameters. With name-value
 * parameters the values are optional and names may be repeated.
 *
 * <h4>Fragment</h4>
 *
 * <p>The fragment is optional: it can be null, empty, or non-empty. Unlike host, port, path, and
 * query the fragment is not sent to the webserver: it's private to the client.
 *
 * <h3>Encoding</h3>
 *
 * <p>Each component must be encoded before it is embedded in the complete URL. As we saw above, the
 * string {@code cute #puppies} is encoded as {@code cute%20%23puppies} when used as a query
 * parameter value.
 *
 * <h4>Percent encoding</h4>
 *
 * <p>Percent encoding replaces a character (like {@code \ud83c\udf69}) with its UTF-8 hex bytes
 * (like {@code %F0%9F%8D%A9}). This approach works for whitespace characters, control characters,
 * non-ASCII characters, and characters that already have another meaning in a particular context.
 *
 * <p>Percent encoding is used in every URL component except for the hostname. But the set of
 * characters that need to be encoded is different for each component. For example, the path
 * component must escape all of its {@code ?} characters, otherwise it could be interpreted as the
 * start of the URL's query. But within the query and fragment components, the {@code ?} character
 * doesn't delimit anything and doesn't need to be escaped. <pre>   {@code
 *
 *   HttpUrl url = HttpUrl.parse("http://who-let-the-dogs.out").newBuilder()
 *       .addPathSegment("_Who?_")
 *       .query("_Who?_")
 *       .fragment("_Who?_")
 *       .build();
 *   System.out.println(url);
 * }</pre>
 *
 * This prints: <pre>   {@code
 *
 *   http://who-let-the-dogs.out/_Who%3F_?_Who?_#_Who?_
 * }</pre>
 *
 * When parsing URLs that lack percent encoding where it is required, this class will percent encode
 * the offending characters.
 *
 * <h4>IDNA Mapping and Punycode encoding</h4>
 *
 * <p>Hostnames have different requirements and use a different encoding scheme. It consists of IDNA
 * mapping and Punycode encoding.
 *
 * <p>In order to avoid confusion and discourage phishing attacks, <a
 * href="http://www.unicode.org/reports/tr46/#ToASCII">IDNA Mapping</a> transforms names to avoid
 * confusing characters. This includes basic case folding: transforming shouting {@code SQUARE.COM}
 * into cool and casual {@code square.com}. It also handles more exotic characters. For example, the
 * Unicode trademark sign (™) could be confused for the letters "TM" in {@code http://ho™mail.com}.
 * To mitigate this, the single character (™) maps to the string (tm). There is similar policy for
 * all of the 1.1 million Unicode code points. Note that some code points such as "\ud83c\udf69" are
 * not mapped and cannot be used in a hostname.
 *
 * <p><a href="http://ietf.org/rfc/rfc3492.txt">Punycode</a> converts a Unicode string to an ASCII
 * string to make international domain names work everywhere. For example, "σ" encodes as "xn--4xa".
 * The encoded string is not human readable, but can be used with classes like {@link InetAddress}
 * to establish connections.
 *
 * <h3>Why another URL model?</h3>
 *
 * <p>Java includes both {@link URL java.net.URL} and {@link URI java.net.URI}. We offer a new URL
 * model to address problems that the others don't.
 *
 * <h4>Different URLs should be different</h4>
 *
 * <p>Although they have different content, {@code java.net.URL} considers the following two URLs
 * equal, and the {@link Object#equals equals()} method between them returns true:
 *
 * <ul>
 *   <li>http://square.github.io/
 *   <li>http://google.github.io/
 * </ul>
 *
 * This is because those two hosts share the same IP address. This is an old, bad design decision
 * that makes {@code java.net.URL} unusable for many things. It shouldn't be used as a {@link
 * java.util.Map Map} key or in a {@link Set}. Doing so is both inefficient because equality may
 * require a DNS lookup, and incorrect because unequal URLs may be equal because of how they are
 * hosted.
 *
 * <h4>Equal URLs should be equal</h4>
 *
 * <p>These two URLs are semantically identical, but {@code java.net.URI} disagrees:
 *
 * <ul>
 *   <li>http://host:80/
 *   <li>http://host
 * </ul>
 *
 * Both the unnecessary port specification ({@code :80}) and the absent trailing slash ({@code /})
 * cause URI to bucket the two URLs separately. This harms URI's usefulness in collections. Any
 * application that stores information-per-URL will need to either canonicalize manually, or suffer
 * unnecessary redundancy for such URLs.
 *
 * <p>Because they don't attempt canonical form, these classes are surprisingly difficult to use
 * securely. Suppose you're building a webservice that checks that incoming paths are prefixed
 * "/static/images/" before serving the corresponding assets from the filesystem. <pre>   {@code
 *
 *   String attack = "http://example.com/static/images/../../../../../etc/passwd";
 *   System.out.println(new URL(attack).getPath());
 *   System.out.println(new URI(attack).getPath());
 *   System.out.println(HttpUrl.parse(attack).encodedPath());
 * }</pre>
 *
 * By canonicalizing the input paths, they are complicit in directory traversal attacks. Code that
 * checks only the path prefix may suffer!
 * <pre>   {@code
 *
 *    /static/images/../../../../../etc/passwd
 *    /static/images/../../../../../etc/passwd
 *    /etc/passwd
 * }</pre>
 *
 * <h4>If it works on the web, it should work in your application</h4>
 *
 * <p>The {@code java.net.URI} class is strict around what URLs it accepts. It rejects URLs like
 * "http://example.com/abc|def" because the '|' character is unsupported. This class is more
 * forgiving: it will automatically percent-encode the '|', yielding "http://example.com/abc%7Cdef".
 * This kind behavior is consistent with web browsers. {@code HttpUrl} prefers consistency with
 * major web browsers over consistency with obsolete specifications.
 *
 * <h4>Paths and Queries should decompose</h4>
 *
 * <p>Neither of the built-in URL models offer direct access to path segments or query parameters.
 * Manually using {@code StringBuilder} to assemble these components is cumbersome: do '+'
 * characters get silently replaced with spaces? If a query parameter contains a '&amp;', does that
 * get escaped? By offering methods to read and write individual query parameters directly,
 * application developers are saved from the hassles of encoding and decoding.
 *
 * <h4>Plus a modern API</h4>
 *
 * <p>The URL (JDK1.0) and URI (Java 1.4) classes predate builders and instead use telescoping
 * constructors. For example, there's no API to compose a URI with a custom port without also
 * providing a query and fragment.
 *
 * <p>Instances of {@link HttpUrl} are well-formed and always have a scheme, host, and path. With
 * {@code java.net.URL} it's possible to create an awkward URL like {@code http:/} with scheme and
 * path but no hostname. Building APIs that consume such malformed values is difficult!
 *
 * <p>This class has a modern API. It avoids punitive checked exceptions: {@link #get get()}
 * throws {@link IllegalArgumentException} on invalid input or {@link #parse parse()}
 * returns null if the input is an invalid URL. You can even be explicit about whether each
 * component has been encoded already.
 */

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

/** Returns either "http" or "https". */
string HttpUrl::getScheme() const {
    return scheme;
}

bool HttpUrl::isHttps() {
    return scheme == "https";
}

string HttpUrl::encodedUsername() {
    if (username.empty()){
        return "";
    }
    int username_start = scheme.size() + 3;// "://".length() == 3.
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
    int path_start = url.find('/', scheme.size() + 3);// "://".length() == 3.
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

HttpUrl HttpUrl::get(string &url) {
    return Builder().parse(nullptr, url).build();
}

HttpUrl::HttpUrl(Builder & builder) {
    this->scheme = builder.scheme;
    this->username = percentDecode(builder.encodedUsername, 0, builder.encodedUsername.size(), false);
    this->password = percentDecode(builder.encodedPassword, 0, builder.encodedPassword.size(), false);
    this->host = builder.host;
    this->port = builder.effectivePort();
    this->pathSegments = percentDecode(builder.encodedPathSegments, false);
    if (!builder.encodedQueryNamesAndValues.empty()){
        this->queryNameAndValues = percentDecode(builder.encodedQueryNamesAndValues, true);
    }
    if (!builder.encodedFragment.empty()){
        this->fragment = percentDecode(builder.encodedFragment, 0, builder.encodedFragment.size(), false);
    }
    this->url = builder.toString();
}

string HttpUrl::percentDecode(string & encoded, int pos, int limit, bool plusIsSpace) {
    int codePoint;
    int codePointEndIndex;
    string out;
    int charCount;
    for ( int i = pos; i < limit; i += charCount){
        codePointEndIndex = HttpUrl::codePointEndIndex(encoded, i);
        codePoint = HttpUrl::codePoint(encoded, i, codePointEndIndex);
        charCount = codePointEndIndex - i+1;
        if (codePoint == '%' && i+2 < limit){
            int d1 = decodeHexDeigit(encoded[i+1]);
            int d2 = decodeHexDeigit(encoded[i+2]);
            if (d1 != -1 && d2 != -1){
                out.append(1, (d1 << 4) + d2);
                i+= 2;
                continue;
            }
        }else if (codePoint == '+' && plusIsSpace){
            out.append(1, ' ');
            continue;
        }
        out.append(1, codePoint);
    }
    return out;
}

vector<string> HttpUrl::percentDecode(vector<string> list, bool plusIsSpace) {
    int size = list.size();
    vector<string> result;
    for (int i = 0; i < size; i++){
        string s = list[i];
        result.push_back(s.empty() ? s : percentDecode(s, 0, s.size(), plusIsSpace));
    }
    return result;
}

HttpUrl::Builder HttpUrl::newBuilder() {
    Builder builder = HttpUrl::Builder();
    builder.scheme = scheme;
    builder.encodedUsername = encodedUsername();
    builder.encodedPassword = encodedPassword();
    builder.host = host;
    // If we're set to a default port, unset it in case of a scheme change.
    builder.port = port != defaultPort(scheme) ? port : -1;
    builder.encodedPathSegments.clear();
    builder.encodedPathSegments.insert(builder.encodedPathSegments.end(),
            encodedPathSegments().begin(), encodedPathSegments().end());
    builder.encodedQuery(encodedQuery());
    builder.encodedFragment = encodedFragment();
    return builder;
}

HttpUrl HttpUrl::resolve(string link) {
    Builder builder = newBuilder();
    return builder.build();
}

HttpUrl::Builder HttpUrl::newBuilder(string link) {
    return Builder().parse(this, link);
}

HttpUrl HttpUrl::parse(string &url) {
    return get(url);
}

HttpUrl::HttpUrl() {

}



