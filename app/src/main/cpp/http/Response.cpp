//
// Created by wengzc on 2019/4/22.
//

#include "Response.h"

ResponseBody &Response::body() {
    ResponseBody * responseBody = new ResponseBody;
    return *responseBody;
}
