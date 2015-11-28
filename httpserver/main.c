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
#include "handleRequest.h"

#define INVALID_VALUE (-1)

#define ERROR_RETURN(ret)                    \
if (ret == -1)                       \
{                                       \
printf("Error, program exited!\n"); \
return 0;                           \
}                                       \

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
        
        if (confd == INVALID_VALUE)
            continue;
        
        char buf[1024];
        
        ssize_t len = read(confd, (void*)buf, 1024);
        if (len > 0)
        {
            http_request request;
            parseHttpRequest(buf, &request);
            
            char* response = NULL;

            switch (request.method)
            {
            case HTTP_GET:
                    response = handleGetMethod(request);
                    break;
            case HTTP_POST:
                    break;
            default:
                    puts("Undefined!");
                    break;
            }
            
            if (NULL == response)
                continue;
            
            send(confd, (void*)response, strlen(response), 0);
            
            free(response);
            response = NULL;
            //break;
        }
    }
    
    //close(sockfd);
    
    return 0;
}
