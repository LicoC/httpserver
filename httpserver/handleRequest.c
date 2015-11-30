//
//  handleRequest.c
//  httpserver
//
//  Created by licorice on 28/11/15.
//  Copyright © 2015年 licorice. All rights reserved.
//

#include "handleRequest.h"
#include "http_parser.h"
#include "genResponseHelper.h"
#include <stdlib.h>
#include <string.h>

char* handleGetMethod(http_request request)
{
    if (request.method != HTTP_GET)
        return NULL;
    
    //todo:这个root 应该从配置文件里读取
    char* root = "/users/licorice/Code/httpserver/httpserver/html";
    char* url = NULL; //资源相对位置
    char* path = NULL; //资源绝对位置
    
    if (0 == strcmp(request.url, "/"))
        url = "/index.html";
    else
        url = request.url;
    
    path = malloc(strlen(root) + strlen(url));
    
    strcat(path, root);
    strcat(path, url);

    char* response_body = NULL;
    getHtmlFile(path, &response_body);
    
    free(path);
    
    //生成响应报文
    char* response = generateResponse(response_body);
    
    //这里的处理太挫了，总是要记得手动去释放，后面看看有什么好的办法
    free(response_body);
    return response;
}

char* handlePostMethod(http_request request)
{
    if (request.method != HTTP_POST)
        return NULL;
    
    puts("Handle POST Method:");
    
    httpHeader* header = request.headers;
    while (header) {
        printf("%s: %s",header->fieldname, header->fieldvalue);
        header = header->nextField;
    }
    printf("Request body: %s", request.body);
    
    //todo:这个root 应该从配置文件里读取
    char* root = "/users/licorice/Code/httpserver/httpserver/html";
    char* url = NULL; //资源相对位置
    char* path = NULL; //资源绝对位置
    
    if (0 == strcmp(request.url, "/"))
        url = "/test_post.html";
    else
        url = request.url;
    
    path = malloc(strlen(root) + strlen(url));
    
    strcat(path, root);
    strcat(path, url);
    
    char* response_body = NULL;
    getHtmlFile(path, &response_body);
    
    free(path);
    
    //生成响应报文
    char* response = generateResponse(response_body);
    
    //这里的处理太挫了，总是要记得手动去释放，后面看看有什么好的办法
    free(response_body);
    return response;

}
