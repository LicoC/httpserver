//
//  localrequest.c
//  httpserver
//
//  Created by licorice on 29/11/15.
//  Copyright © 2015年 licorice. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "localrequest.h"

httpHeader* create_httpHeader()
{
    httpHeader* header = malloc(sizeof(httpHeader));
    header->fieldname = NULL;
    header->fieldvalue = NULL;
    header->nextField = NULL;
    
    return header;
}

//新增一个首部信息节点
httpHeader* add_httpHeader(http_request* request)
{
    httpHeader* header = request->headers;
    
    while (header)
    {
        if (header->nextField == NULL)
        {
            header->nextField = create_httpHeader();
            return header->nextField;
        }
        else
            header = header->nextField;
    }
    
    request->headers = create_httpHeader();
    
    return request->headers;
}
