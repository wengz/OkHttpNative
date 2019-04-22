#include <jni.h>
#include <string>

#include "http/HttpClient.h"

using namespace std;

void testFun (){
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

    testFun();

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

