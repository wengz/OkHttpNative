//
// Created by wengzc on 2019/4/22.
//

#include <android/log.h>
#include <algorithm>
#include "LogUtil.h"

using namespace std;

const int LogUtil::LOG_SEGMENT_LIMIT = 3 * 1024;

const std::string LogUtil::LOG_TAG = "NDK HTTP";

void LogUtil::debug(const std::string &msg) {

    int pos = 0;
    int contentLength = msg.size();
    int segmentLength;
    while (pos < contentLength){
        segmentLength = LOG_SEGMENT_LIMIT;
        int rest = contentLength - pos;
        segmentLength = min(rest, segmentLength);
        string segmentContent = msg.substr(pos, segmentLength);
        __android_log_write(ANDROID_LOG_DEBUG, LOG_TAG.c_str(), segmentContent.c_str());
        pos += segmentLength;
    }

}