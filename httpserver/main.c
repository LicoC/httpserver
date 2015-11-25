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
#include "request_parser.h"

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
            http_request request;
            parseHttpRequest(buf, &request);
            
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
