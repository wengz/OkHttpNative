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
    string str = "sigalhu233";
    regex rg("[a-z0-9]+");
    if (regex_match(str, rg)){
    }else{
    }
}

extern "C" JNIEXPORT void
JNICALL
Java_com_wengzc_okhttpnative_MainActivity_trigerJNI(
        JNIEnv *env,
        jobject /* this */) {

    testFun();
}

