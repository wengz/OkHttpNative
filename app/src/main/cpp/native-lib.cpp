#include <jni.h>
#include <string>
#include <regex>
#include "log/LogUtil.h"

#include "http/HttpClient.h"

using namespace std;

void testOkHttpNative (){
    HttpClient client;
    string url("http://www.baidu.com");
    Request request = Request::Builder().setUrl(url).build();
    client.newCall(&request)->execute();
}

extern "C" JNIEXPORT jstring
JNICALL
Java_com_wengzc_okhttpnative_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


void testFun (){
    //string str("text/plain; charset=us-ascii");
    string str("qqq zabceee  wbz abcd");
    regex ex("\\w*(abc)\\w*");
    smatch sm;
    string::const_iterator iterator_start = str.begin();
    string::const_iterator iterator_end = str.end();
    string temp;
    while (regex_search(iterator_start, iterator_end, sm, ex)){
        temp = sm[0];
        iterator_start = sm[0].second;
    }

}

extern "C" JNIEXPORT void
JNICALL
Java_com_wengzc_okhttpnative_MainActivity_trigerJNI(
        JNIEnv *env,
        jobject /* this */) {

    testFun();
}

