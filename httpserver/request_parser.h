//
//  request_parser.h
//  httpserver
//
//  Created by licorice on 25/11/15.
//  Copyright © 2015年 licorice. All rights reserved.
//

#ifndef request_parser_h
#define request_parser_h
#include "http_parser.h"
#include "localrequest.h"

int handle_request_url(http_parser* parser, const char* data, size_t len);

int handle_header_field(http_parser* parser, const char* data, size_t len);

int handle_header_value(http_parser* parser, const char* data, size_t len);

int handle_request_body(http_parser* parser, const char* data, size_t len);

int on_message_begin(http_parser* parser);

int on_message_complete(http_parser* parser);

int on_headers_complete(http_parser* parser);

/*
 * parseHttpRequest
 * requestBuf: 读到的请求内容
 * request: 解析到的请求报文内容
 * return: 是否解析成功
 */
int parseHttpRequest(const char* requestBuf, http_request* request);
void initHttpRequest(http_request* request);
int clearHttpRequest(http_request* request);

#endif /* request_parser_h */
