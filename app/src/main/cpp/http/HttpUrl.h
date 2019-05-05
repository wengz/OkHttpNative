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

    class Builder;

/**
   * Returns 80 if {@code scheme.equals("http")}, 443 if {@code scheme.equals("https")} and -1
   * otherwise.
   */
    static int defaultPort(string scheme);

    static void pathSegmentsToString(string & out, vector<string> pathSegments);
    static void namesAndValuesToQueryString(string & out, vector<string> namesAndValues);

    /**
   * Cuts {@code encodedQuery} up into alternating parameter names and values. This divides a query
   * string like {@code subject=math&easy&problem=5-2=3} into the list {@code ["subject", "math",
   * "easy", null, "problem", "5-2=3"]}. Note that values may be null and may contain '='
   * characters.
   */
    static vector<string> queryStringToNamesAndValues(string encodedQuery);
    static string canonicalize(const string & input, int pos, int limit, const string encode_set,
                               bool alreadyEncoded, bool strict, bool plusIsSpace, bool asciiOnly);

    static int codePointEndIndex(const string & s, int startIndex);

    static int codePoint(const string & s, int startIndex, int endIndex);

    static bool percentEncoded(const string & s, int pos, int limit);

/**
   * Returns a new {@code HttpUrl} representing {@code url} if it is a well-formed HTTP or HTTPS
   * URL, or null if it isn't.
   */
    static HttpUrl parse(string & url);

    /**
   * Returns a new {@code HttpUrl} representing {@code url}.
   *
   */
    static HttpUrl get(string & url);


    static string percentDecode(string & input, int pos, int limit, bool plusIsSpace);

    static vector<string> percentDecode(vector<string> list, bool plusIsSpace);

    HttpUrl(Builder & builder);

    string getScheme() const;

    bool isHttps();

    /**
   * Returns the username, or an empty string if none is set.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code encodedUsername()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>{@code ""}</td></tr>
   *   <tr><td>{@code http://username@host/}</td><td>{@code "username"}</td></tr>
   *   <tr><td>{@code http://username:password@host/}</td><td>{@code "username"}</td></tr>
   *   <tr><td>{@code http://a%20b:c%20d@host/}</td><td>{@code "a%20b"}</td></tr>
   * </table>
   */
    string encodedUsername();

    /**
   * Returns the decoded username, or an empty string if none is present.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code username()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>{@code ""}</td></tr>
   *   <tr><td>{@code http://username@host/}</td><td>{@code "username"}</td></tr>
   *   <tr><td>{@code http://username:password@host/}</td><td>{@code "username"}</td></tr>
   *   <tr><td>{@code http://a%20b:c%20d@host/}</td><td>{@code "a b"}</td></tr>
   * </table>
   */
    string userName();

    /**
   * Returns the password, or an empty string if none is set.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code encodedPassword()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>{@code ""}</td></tr>
   *   <tr><td>{@code http://username@host/}</td><td>{@code ""}</td></tr>
   *   <tr><td>{@code http://username:password@host/}</td><td>{@code "password"}</td></tr>
   *   <tr><td>{@code http://a%20b:c%20d@host/}</td><td>{@code "c%20d"}</td></tr>
   * </table>
   */
    string encodedPassword();

    /**
   * Returns the decoded password, or an empty string if none is present.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code password()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>{@code ""}</td></tr>
   *   <tr><td>{@code http://username@host/}</td><td>{@code ""}</td></tr>
   *   <tr><td>{@code http://username:password@host/}</td><td>{@code "password"}</td></tr>
   *   <tr><td>{@code http://a%20b:c%20d@host/}</td><td>{@code "c d"}</td></tr>
   * </table>
   */
    string getPassword();

    /**
   * Returns the host address suitable for use with {@link InetAddress#getAllByName(String)}. May
   * be:
   *
   * <ul>
   *   <li>A regular host name, like {@code android.com}.
   *   <li>An IPv4 address, like {@code 127.0.0.1}.
   *   <li>An IPv6 address, like {@code ::1}. Note that there are no square braces.
   *   <li>An encoded IDN, like {@code xn--n3h.net}.
   * </ul>
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code host()}</th></tr>
   *   <tr><td>{@code http://android.com/}</td><td>{@code "android.com"}</td></tr>
   *   <tr><td>{@code http://127.0.0.1/}</td><td>{@code "127.0.0.1"}</td></tr>
   *   <tr><td>{@code http://[::1]/}</td><td>{@code "::1"}</td></tr>
   *   <tr><td>{@code http://xn--n3h.net/}</td><td>{@code "xn--n3h.net"}</td></tr>
   * </table>
   */
    string getHost();

    /**
   * Returns the explicitly-specified port if one was provided, or the default port for this URL's
   * scheme. For example, this returns 8443 for {@code https://square.com:8443/} and 443 for {@code
   * https://square.com/}. The result is in {@code [1..65535]}.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code port()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>{@code 80}</td></tr>
   *   <tr><td>{@code http://host:8000/}</td><td>{@code 8000}</td></tr>
   *   <tr><td>{@code https://host/}</td><td>{@code 443}</td></tr>
   * </table>
   */
    int getPort();

    /**
 * Returns the number of segments in this URL's path. This is also the number of slashes in the
 * URL's path, like 3 in {@code http://host/a/b/c}. This is always at least 1.
 *
 * <p><table summary="">
 *   <tr><th>URL</th><th>{@code pathSize()}</th></tr>
 *   <tr><td>{@code http://host/}</td><td>{@code 1}</td></tr>
 *   <tr><td>{@code http://host/a/b/c}</td><td>{@code 3}</td></tr>
 *   <tr><td>{@code http://host/a/b/c/}</td><td>{@code 4}</td></tr>
 * </table>
 */
    int pathSize();

    /**
   * Returns the entire path of this URL encoded for use in HTTP resource resolution. The returned
   * path will start with {@code "/"}.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code encodedPath()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>{@code "/"}</td></tr>
   *   <tr><td>{@code http://host/a/b/c}</td><td>{@code "/a/b/c"}</td></tr>
   *   <tr><td>{@code http://host/a/b%20c/d}</td><td>{@code "/a/b%20c/d"}</td></tr>
   * </table>
   */
    string encodedPath();

    /**
   * Returns a list of encoded path segments like {@code ["a", "b", "c"]} for the URL {@code
   * http://host/a/b/c}. This list is never empty though it may contain a single empty string.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code encodedPathSegments()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>{@code [""]}</td></tr>
   *   <tr><td>{@code http://host/a/b/c}</td><td>{@code ["a", "b", "c"]}</td></tr>
   *   <tr><td>{@code http://host/a/b%20c/d}</td><td>{@code ["a", "b%20c", "d"]}</td></tr>
   * </table>
   */
    vector<string> encodedPathSegments();

    /**
   * Returns a list of path segments like {@code ["a", "b", "c"]} for the URL {@code
   * http://host/a/b/c}. This list is never empty though it may contain a single empty string.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code pathSegments()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>{@code [""]}</td></tr>
   *   <tr><td>{@code http://host/a/b/c"}</td><td>{@code ["a", "b", "c"]}</td></tr>
   *   <tr><td>{@code http://host/a/b%20c/d"}</td><td>{@code ["a", "b c", "d"]}</td></tr>
   * </table>
   */
    vector<string> getPathSegments();

    /**
   * Returns the query of this URL, encoded for use in HTTP resource resolution. The returned string
   * may be null (for URLs with no query), empty (for URLs with an empty query) or non-empty (all
   * other URLs).
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code encodedQuery()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>null</td></tr>
   *   <tr><td>{@code http://host/?}</td><td>{@code ""}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&k=key+lime}</td><td>{@code
   *       "a=apple&k=key+lime"}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&a=apricot}</td><td>{@code "a=apple&a=apricot"}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&b}</td><td>{@code "a=apple&b"}</td></tr>
   * </table>
   */
    string encodedQuery();

    /**
   * Returns this URL's query, like {@code "abc"} for {@code http://host/?abc}. Most callers should
   * prefer {@link #queryParameterName} and {@link #queryParameterValue} because these methods offer
   * direct access to individual query parameters.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code query()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>null</td></tr>
   *   <tr><td>{@code http://host/?}</td><td>{@code ""}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&k=key+lime}</td><td>{@code "a=apple&k=key
   *       lime"}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&a=apricot}</td><td>{@code "a=apple&a=apricot"}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&b}</td><td>{@code "a=apple&b"}</td></tr>
   * </table>
   */
    string query();

    /**
   * Returns the number of query parameters in this URL, like 2 for {@code
   * http://host/?a=apple&b=banana}. If this URL has no query this returns 0. Otherwise it returns
   * one more than the number of {@code "&"} separators in the query.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code querySize()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>{@code 0}</td></tr>
   *   <tr><td>{@code http://host/?}</td><td>{@code 1}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&k=key+lime}</td><td>{@code 2}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&a=apricot}</td><td>{@code 2}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&b}</td><td>{@code 2}</td></tr>
   * </table>
   */
    int querySize();

    /**
   * Returns the first query parameter named {@code name} decoded using UTF-8, or null if there is
   * no such query parameter.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code queryParameter("a")}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>null</td></tr>
   *   <tr><td>{@code http://host/?}</td><td>null</td></tr>
   *   <tr><td>{@code http://host/?a=apple&k=key+lime}</td><td>{@code "apple"}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&a=apricot}</td><td>{@code "apple"}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&b}</td><td>{@code "apple"}</td></tr>
   * </table>
   */
    string queryParameters(string name);

    /**
   * Returns the distinct query parameter names in this URL, like {@code ["a", "b"]} for {@code
   * http://host/?a=apple&b=banana}. If this URL has no query this returns the empty set.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code queryParameterNames()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>{@code []}</td></tr>
   *   <tr><td>{@code http://host/?}</td><td>{@code [""]}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&k=key+lime}</td><td>{@code ["a", "k"]}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&a=apricot}</td><td>{@code ["a"]}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&b}</td><td>{@code ["a", "b"]}</td></tr>
   * </table>
   */
    set<string> queryParameterNames();

    /**
   * Returns all values for the query parameter {@code name} ordered by their appearance in this
   * URL. For example this returns {@code ["banana"]} for {@code queryParameterValue("b")} on {@code
   * http://host/?a=apple&b=banana}.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code queryParameterValues("a")}</th><th>{@code
   *       queryParameterValues("b")}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>{@code []}</td><td>{@code []}</td></tr>
   *   <tr><td>{@code http://host/?}</td><td>{@code []}</td><td>{@code []}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&k=key+lime}</td><td>{@code ["apple"]}</td><td>{@code
   *       []}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&a=apricot}</td><td>{@code ["apple",
   *       "apricot"]}</td><td>{@code []}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&b}</td><td>{@code ["apple"]}</td><td>{@code
   *       [null]}</td></tr>
   * </table>
   */
    vector<string> queryParameterValues(string name);

    /**
   * Returns the name of the query parameter at {@code index}. For example this returns {@code "a"}
   * for {@code queryParameterName(0)} on {@code http://host/?a=apple&b=banana}. This throws if
   * {@code index} is not less than the {@linkplain #querySize query size}.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code queryParameterName(0)}</th><th>{@code
   *       queryParameterName(1)}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>exception</td><td>exception</td></tr>
   *   <tr><td>{@code http://host/?}</td><td>{@code ""}</td><td>exception</td></tr>
   *   <tr><td>{@code http://host/?a=apple&k=key+lime}</td><td>{@code "a"}</td><td>{@code
   *       "k"}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&a=apricot}</td><td>{@code "a"}</td><td>{@code
   *       "a"}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&b}</td><td>{@code "a"}</td><td>{@code "b"}</td></tr>
   * </table>
   */
    string queryParameterName(int index);

    /**
   * Returns the value of the query parameter at {@code index}. For example this returns {@code
   * "apple"} for {@code queryParameterName(0)} on {@code http://host/?a=apple&b=banana}. This
   * throws if {@code index} is not less than the {@linkplain #querySize query size}.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code queryParameterValue(0)}</th><th>{@code
   *       queryParameterValue(1)}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>exception</td><td>exception</td></tr>
   *   <tr><td>{@code http://host/?}</td><td>null</td><td>exception</td></tr>
   *   <tr><td>{@code http://host/?a=apple&k=key+lime}</td><td>{@code "apple"}</td><td>{@code
   *       "key lime"}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&a=apricot}</td><td>{@code "apple"}</td><td>{@code
   *       "apricot"}</td></tr>
   *   <tr><td>{@code http://host/?a=apple&b}</td><td>{@code "apple"}</td><td>null</td></tr>
   * </table>
   */
    string queryParameterValue(int index);

    /**
   * Returns this URL's encoded fragment, like {@code "abc"} for {@code http://host/#abc}. This
   * returns null if the URL has no fragment.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code encodedFragment()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>null</td></tr>
   *   <tr><td>{@code http://host/#}</td><td>{@code ""}</td></tr>
   *   <tr><td>{@code http://host/#abc}</td><td>{@code "abc"}</td></tr>
   *   <tr><td>{@code http://host/#abc|def}</td><td>{@code "abc|def"}</td></tr>
   * </table>
   */
    string encodedFragment();

    /**
   * Returns this URL's fragment, like {@code "abc"} for {@code http://host/#abc}. This returns null
   * if the URL has no fragment.
   *
   * <p><table summary="">
   *   <tr><th>URL</th><th>{@code fragment()}</th></tr>
   *   <tr><td>{@code http://host/}</td><td>null</td></tr>
   *   <tr><td>{@code http://host/#}</td><td>{@code ""}</td></tr>
   *   <tr><td>{@code http://host/#abc}</td><td>{@code "abc"}</td></tr>
   *   <tr><td>{@code http://host/#abc|def}</td><td>{@code "abc|def"}</td></tr>
   * </table>
   */
    string getFragment();

    /**
   * Returns the URL that would be retrieved by following {@code link} from this URL, or null if
   * the resulting URL is not well-formed.
   */
    HttpUrl resolve(string link);

    Builder newBuilder();

    Builder newBuilder(string link);

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

    /** Either "http" or "https". */
    string scheme;

    /** Decoded username. */
    string username;

    /** Decoded password. */
    string password;

    /** Canonical hostname. */
    string host;

    /** Either 80, 443 or a user-specified port. In range [1..65535]. */
    int port;

    /**
   * A list of canonical path segments. This list always contains at least one element, which may be
   * the empty string. Each segment is formatted with a leading '/', so if path segments were ["a",
   * "b", ""], then the encoded path would be "/a/b/".
   */
    vector<string> pathSegments = vector<string>();

    /**
   * Alternating, decoded query names and values, or null for no query. Names may be empty or
   * non-empty, but never null. Values are null if the name has no corresponding '=' separator, or
   * empty, or non-empty.
   */
    vector<string> queryNameAndValues = vector<string>();

    /** Decoded fragment. */
    string fragment;

    /** Canonical URL. */
    string url;
};


#endif //OKHTTPNATIVE_HTTPURL_H
