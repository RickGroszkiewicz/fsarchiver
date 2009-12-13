/*
 * fsarchiver: Filesystem Archiver
 *
 * Copyright (C) 2008-2009 Francois Dupoux.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * Homepage: http://www.fsarchiver.org
 */

#include "fsarchiver.h"

#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "error.h"
#include "common.h"
#include "logfile.h"

int fsaprintf(int level, bool showerrno, bool showloc, const char *file, const char *fct, int line, char *format, ...)
{
    char buffer[8192];
    char temp[1024];
    bool msgscreen;
    bool msglogfile;
    va_list ap;
    
    // init
    memset(buffer, 0, sizeof(buffer));
    msgscreen=(level <= g_options.verboselevel);
    msglogfile=(level <= g_options.debuglevel);
    
    if (msgscreen || msglogfile)
    {
        // 1. format errno and its meaning
        if (showerrno)
            strlcatf(buffer, sizeof(buffer), "[errno=%d, %s]: ", errno, strerror(errno));
        
        // 2. format location of the message
        if (showloc)
            strlcatf(buffer, sizeof(buffer), "%s#%d,%s(): ", file, line, fct);
        
        // 3. format text message
        va_start(ap, format);
        vsnprintf(temp, sizeof(temp), format, ap);
        va_end(ap);
        strlcat(buffer, temp, sizeof(buffer));
        
        // 4. show message on screen
        if (msgscreen)
        {   fprintf(stderr, "%s", buffer);
            fflush(stderr);
        }
        
        // 5. write message in logfile if requested
        if (msglogfile)
            logfile_write(buffer, strlen(buffer));
    }
    
    return 0;
}