//
//  main.c
//  httpserver
//
//  Created by licorice on 21/11/15.
//  Copyright © 2015年 licorice. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define INVALID_VALUE (-1)

#define ERROR_RETURN(ret)                    \
if (ret == -1)                       \
{                                       \
printf("Error, program exited!\n"); \
return 0;                           \
}                                       \

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

//define a enum methond
typedef enum method{IVALID_METHOD = -1, GET, HEAD, POST, PUT, TRACE, OPTIONS, DELETE, METHOD_END}method;

//define a httpRequest struct to wrap the parsed http request
typedef struct httpRequest
{
    method method;
    char* request_url;
    char* version;
    char* Connection;
    char* Accept;
    char* User_Agent;
}http_request;

/*
 * getMethod 返回请求的方法
 * strMethod: 解析到的方法名
 * return: 对应的方法名
 */
method getMethod(const char* strMethod)
{
    if (NULL == strMethod)
        return IVALID_METHOD;
    
    method m = IVALID_METHOD;
    
    if (0 == strcmp(strMethod, "GET"))
        m = GET;
    else if (0 == strcmp(strMethod, "POST"))
        m = POST;
    else if (0 == strcmp(strMethod, "HEAD"))
        m = HEAD;
    else if (0 == strcmp(strMethod, "PUT"))
        m = PUT;
    else if (0 == strcmp(strMethod, "TRACE"))
        m = TRACE;
    else if (0 == strcmp(strMethod, "OPTIONS"))
        m = OPTIONS;
    else if (0 == strcmp(strMethod, "DELETE"))
        m = DELETE;
    
    return m;
}

int parseRequestLine(char* requestLine, http_request* request)
{
    if (!requestLine)
        return -1;
    
    char* p = NULL;
    const char* delim = " ";
    p = strtok(requestLine, delim);
    
    //1.parse method
    request->method = getMethod(p);
    
    p = strtok(NULL, delim);
    request->request_url = p;
    
    p = strtok(NULL, delim);
    request->version = p;
    
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
    char* buf = malloc(strlen(requestBuf));
    strcpy(buf, requestBuf);
    
    const char* delim = "\r\n";
    char* p = NULL;
    p = strtok(buf, delim);
    if (!p)
        return -1;
    
    puts("parsing http request:\n");
    
    //1.处理首部
    http_request tRequest;
    parseRequestLine(p, &tRequest);
    
    return 0;
}

int main(int argc, const char * argv[]) {
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    ERROR_RETURN(sockfd)
    printf("created a socket successd!\n");
    
    struct sockaddr_in socket_addr;
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(8000);
    socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int ret = bind(sockfd, (struct sockaddr*)&socket_addr, sizeof(socket_addr));
    
    ERROR_RETURN(ret)
    printf("bind socket to host\n");
    
    ret = listen(sockfd, 10);
    
    ERROR_RETURN(ret)
    printf("listenning...\n");
    
    while(1)
    {
        struct sockaddr_in socket_client;
        socklen_t clientlen = sizeof(socket_client);
        int confd = accept(sockfd, (void*)&socket_client, &clientlen);
        
        if (confd == -1)
            continue;
        
        char buf[1024];
        
        ssize_t len = read(confd, (void*)buf, 1024);
        if (len > 0)
        {
            puts("Request is:");
            puts(buf);
            
            parseHttpRequest(buf, NULL);
            
            char response_head[] = "HTTP/1.x 200 OK\r\nContent-Type: text/html\r\n\r\n";
            char* response_body = NULL;
            
            size_t contentSize =
                getHtmlFile("/users/licorice/Code/httpserver/httpserver/html/index.html",&response_body);
            if (INVALID_VALUE == contentSize)
            {
                free(response_body);
                continue;
            }
                
            char* response = NULL;
            size_t nResponse = strlen(response_head) + contentSize;

            response = malloc(nResponse);
            
            if (NULL == response)
                continue;
            
            strcat(response, response_body);
            
            send(confd, (void*)response, strlen(response), 0);
            
            free(response_body);
            free(response);
            response = NULL;
            //break;
        }
    }
    
    //close(sockfd);
    
    return 0;
}
