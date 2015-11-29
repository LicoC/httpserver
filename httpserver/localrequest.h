//
//  localrequest.h
//  httpserver
//
//  Created by licorice on 28/11/15.
//  Copyright © 2015年 licorice. All rights reserved.
//

#ifndef localrequest_h
#define localrequest_h
#include "http_parser.h"

//本地定义一个首部信息结构体，用来存储 http_parser解析出来的首部信息
//请求和响应可共用这一个
typedef struct httpHeader
{
    char* fieldname;
    char* fieldvalue;
    struct httpHeader* nextField;
}httpHeader;

//define a httpRequest struct to wrap the parsed http request
typedef struct httpRequest
{
    enum http_method method;
    char* url;
    char* version;
    httpHeader* headers;
    char* body;
}http_request;

httpHeader* add_httpHeader(http_request* request);

httpHeader* create_httpHeader();

#endif /* localrequest_h */
