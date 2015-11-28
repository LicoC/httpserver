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

//define a httpRequest struct to wrap the parsed http request
typedef struct httpRequest
{
    enum http_method method;
    char* request_url;
    char* version;
    char* Connection;
    char* Accept;
    char* User_Agent;
}http_request;


#endif /* localrequest_h */
