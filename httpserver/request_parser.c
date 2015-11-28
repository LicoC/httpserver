//
//  request_parser.c
//  httpserver
//
//  Created by licorice on 25/11/15.
//  Copyright © 2015年 licorice. All rights reserved.
//

#include <stdio.h>
#include "request_parser.h"
#include <stdlib.h>
#include <string.h>

int handle_request_url(http_parser* parser, const char* data, size_t len)
{
    puts("handle_request_url\n");
    puts(data);
    http_request* request = (http_request*)parser->data;
    request->method = parser->method;
    
    request->request_url = malloc(len + 1);
    memcpy(request->request_url, data, len);
    
    request->request_url[len] = '\0';
    return 0;
}

int handle_header_field(http_parser* parser, const char* data, size_t len)
{
    puts("handle_header_field\n");
    return 0;
}

int handle_header_value(http_parser* parser, const char* data, size_t len)
{
    puts("handle_header_value\n");
    return 0;
}

int handle_request_body(http_parser* parser, const char* data, size_t len)
{
    puts("handle_request_body\n");

    return 0;
}

int on_message_begin(http_parser* parser)
{
    puts("on_message_begin\n");

    return 0;
}

int on_message_complete(http_parser* parser)
{
    puts("on_message_complete\n");

    return 0;
}

int on_headers_complete(http_parser* parser)
{
    puts("on_headers_complete\n");

    return 0;
}

/*
 * parseHttpRequest
 * requestBuf: 读到的请求内容
 * request: 解析到的请求报文内容
 * return: 是否解析成功
 */
int parseHttpRequest(const char* requestBuf, http_request* request)
{
    http_parser_settings settings;
    settings.on_url = handle_request_url;
    settings.on_header_field = handle_header_field;
    settings.on_header_value = handle_header_value;
    settings.on_body = handle_request_body;
    settings.on_message_begin = on_message_begin;
    settings.on_message_complete = on_message_complete;
    settings.on_headers_complete = on_headers_complete;
    
    http_parser *parser = malloc(sizeof(http_parser));
    http_parser_init(parser, HTTP_REQUEST);
    parser->data = request;
    
    http_parser_execute(parser, &settings, requestBuf, strlen(requestBuf));
    
    puts("Method: is");
    puts(http_method_str(request->method));
    puts("Url is:");
    puts(request->request_url);
    
    return 0;
}

void initHttpRequest(http_request* request)
{
    request->request_url = NULL;
    request->User_Agent = NULL;
    request->Accept = NULL;
    request->Connection = NULL;
    request->version = NULL;
}

int clearHttpRequest(http_request* request)
{
    free(request->request_url);
    free(request->Accept);
    free(request->Connection);
    free(request->User_Agent);
    free(request->version);
    return 0;
}

