/*******************************************************************************
 *  android awesome getty (aagetty)
 *
 *  Copyright (C) 2015 Abylay Ospan 
 *  aospan@netup.ru
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <time.h>
#include <utmp.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <termios.h>
#include <unistd.h>
#include <sys/param.h>

void usage(){
    printf("Example:\n-d /dev/ttyS0\n-b /sbin/sh\n");
}

int main (int argc, char **argv){
    int c;
    char * dev = NULL;
    char * bin = NULL;
    int fd;
    struct termios  config;
    char * args[2];

    while ((c = getopt (argc, argv, "d:b:")) != -1){
        switch (c){
            case 'd':
                dev = optarg;
                break;
            case 'b':
                bin = optarg;
                break;
        }
    }

    if(dev == NULL || bin == NULL){
        usage();
        goto DONE;
    }

    printf("Starting with shell=%s dev=%s\n", bin, dev);

    if ((fd = open(dev, O_RDWR, 0)) < 0){
        printf("Error: can't open device. Error=%d(%s)\n", errno, strerror(errno));
        goto DONE;
    }

    /* set speed to 115200. TODO:configurable speed */
    if(tcgetattr(fd, &config) < 0) {
        printf("Can't get attrs from tty port \n");
        goto DONE;
    }

    if(cfsetispeed(&config, B115200) < 0 || cfsetospeed(&config, B115200) < 0) {
        printf("Can't alter tty port config\n");
        goto DONE;
    }

    if(tcsetattr(fd, TCSAFLUSH, &config) < 0) {
        printf("Can't set attrs to tty port \n");
        goto DONE;
    }
    
    /* fd will be stdin,stdout,stderr */
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);

    /* start shell */
    putenv ("PATH=/system/bin:/system/xbin:/sbin:/bin");
    args[0] = bin;
    args[1] = NULL;

    execvp(bin, args);

    exit(0);

DONE:
    return -1;
}
