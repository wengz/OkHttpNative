#include <jni.h>
#include <string>
#include <regex>
#include "log/LogUtil.h"

#include "http/HttpClient.h"
#include "http/MediaType.h"
#include "http/RequestBody.h"
#include "http/Response.h"
#include "http/Request.h"
#include "http/RequestBuilder.h"

using namespace std;

void testOkHttpNative (){

    std::shared_ptr<HttpClient> s_client(new HttpClient);
    string url("http://www.baidu.com");
    Request request = Request::Builder().setUrl(url).build();
    unique_ptr<Response> response = HttpClient::newCall(s_client, request)->execute();
    long contentLength;

    unique_ptr<char[]> content(response->getResponseBody()->bytes(&contentLength));

//    char * content = response->getResponseBody()->bytes(&contentLength);
//    delete content;
}

void testPost (){
//    MediaType mediaType = MediaType::get("application/json; charset=utf-8");
//    HttpClient client;
//    char * reqBodyContent = "abc";
//    RequestBody requetBody = RequestBody::create(mediaType, reqBodyContent, 0, 3);
//    string url("http://10.112.19.17:8080");
//    Request request = Request::Builder().setUrl(url).post(&requetBody).build();
//    Response  * response = client.newCall(&request)->execute();
//    long contentLength;
//    char * content = response->getResponseBody()->bytes(&contentLength);
//    delete content;
}

extern "C" JNIEXPORT jstring
JNICALL
Java_com_wengzc_okhttpnative_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

void cxxRegexTest (){
    //string str("text/plain; charset=us-ascii");
    string str("qqq zabceee  wbz abcd");
    regex ex("(\\w*)abc(\\w*)");
    smatch sm;
    string::const_iterator iterator_start = str.begin();
    string::const_iterator iterator_end = str.end();
    string temp;
    while (regex_search(iterator_start, iterator_end, sm, ex)){
        temp = sm[0];
        string prefix = sm[1];
        string subfix = sm[2];
        iterator_start = sm[0].second;
    }
}

void testMediaType (){
    MediaType resType = MediaType::get("text/plain; charset=us-ascii");
    LogUtil::debug(resType.getType());
    LogUtil::debug(resType.getSubtype());
    LogUtil::debug(resType.getCharset());

}

void testFun (){
    MediaType resType = MediaType::get("text/plain; charset=us-ascii");
    string fakeContent("fake content");
    RequestBody rqb = RequestBody::create(resType, fakeContent.c_str(), 0, fakeContent.size()+1);
    rqb.print();
}


void testPtr (){
//    unique_ptr<string> uPtr(new string("ooxx"));
//    LogUtil::debug(*uPtr);
//    unique_ptr<string> uPtr2 = std::move(uPtr);
//    LogUtil::debug(*uPtr2);

    shared_ptr<string> sPtr(new string("this is shared ptr content   "));
    LogUtil::debug(*sPtr);
    shared_ptr<string> sPtr2 = sPtr;
    LogUtil::debug(*sPtr2);
    LogUtil::debug(*sPtr);
}

void testHttpUrl (){
    //string raw("abc\tabc abc我abc");
    //string newStr = HttpUrl::canonicalize(raw, 0, raw.size(), string(), false, true, true, true);


    string raw("abc%e4%bd%a0%e5%a5%bd+abc");
    string result = HttpUrl::percentDecode(raw, 0, raw.size(), true);

}

extern "C" JNIEXPORT void
JNICALL
Java_com_wengzc_okhttpnative_MainActivity_trigerJNI(
        JNIEnv *env,
        jobject /* this */) {

    testOkHttpNative();
}

