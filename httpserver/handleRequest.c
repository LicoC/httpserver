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

char* handleGetMethod(http_request request)
{
    if (request.method != HTTP_GET)
        return NULL;
    
    char* response_body = NULL;
    getHtmlFile("/users/licorice/Code/httpserver/httpserver/html/index.html",&response_body);
    
    char* response = generateResponse(response_body);
    
    //这里的处理太挫了，总是要记得手动去释放，后面看看有什么好的办法
    free(response_body);
    return response;
}
