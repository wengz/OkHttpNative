//
// Created by wengzc on 2019/4/22.
//

#ifndef NDK_HTTP_CLIENT_LOGUTIL_H
#define NDK_HTTP_CLIENT_LOGUTIL_H

#include <string>

class LogUtil {

public :

    /**
     * android logcat对较长信息进行截断，使用此方法进行分段打印
     */
    static void debug (const std::string & msg);

private :

    /**
     * 日志分段大小
     */
    static const int LOG_SEGMENT_LIMIT;
    static const std::string LOG_TAG;
};


#endif //NDK_HTTP_CLIENT_LOGUTIL_H
