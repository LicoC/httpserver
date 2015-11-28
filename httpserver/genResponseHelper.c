//
//  genResponseHelper.c
//  httpserver
//
//  Created by licorice on 28/11/15.
//  Copyright © 2015年 licorice. All rights reserved.
//

#include "genResponseHelper.h"
#include <stdlib.h>
#include <string.h>

char* generateResponse(char* response_body)
{
    char* response = NULL;
    char response_head[] = "HTTP/1.x 200 OK\r\nContent-Type: text/html\r\n\r\n";
    
    size_t contentSize = strlen(response_body);
    if (-1 == contentSize)
        return NULL;
    
    size_t nResponse = strlen(response_head) + contentSize;
    
    response = malloc(nResponse);
    
    strcpy(response, response_head);
    strcat(response, response_body);
    
    return response;
}
                                         
/*
 *  getHtmlFile 获取请求报文 url 中指定的 html 文件
 *  filename: 要读取的文件名
 *  filecontent: 读到的文件内容
 *  return: 读取文件失败，返回-1.成功，返回读到的内容大小
 */
size_t getHtmlFile(const char* filename, char** filecontent)
{
    //get html file as body
    FILE* pFile = fopen(filename, "r");
    if (NULL == pFile)
    {
        puts("Can't get html file\n");
        return -1;
    }
    
    fseek(pFile, 0, SEEK_END);
    size_t contentSize = ftell(pFile);
    rewind(pFile);
    
    char* response_body = malloc(contentSize);
    if (NULL == response_body)
    {
        fclose(pFile);
        return -1;
    }
    
    contentSize = fread(response_body, 1, contentSize, pFile);
    
    fclose(pFile);
    
    *filecontent = response_body;
    return contentSize;
}
