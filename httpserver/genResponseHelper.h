//
//  genResponseHelper.h
//  httpserver
//
//  Created by licorice on 28/11/15.
//  Copyright © 2015年 licorice. All rights reserved.
//

#ifndef genResponseHelper_h
#define genResponseHelper_h
#include <stdio.h>

size_t getHtmlFile(const char* filename, char** filecontent);

char* generateResponse();

#endif /* genResponseHelper_h */
