/*
**  Android Remote for Tux (AndroTux) 
**  Copyright (C) 2014 Itay Rom <1t4yr0m@gmail.com> Tal Sabo <tal.sabo@gmail.com> 
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "socketFunctions.h"

#ifndef __CLIENT_H__
#define __CLIENT_H__

#define PORT "3490"

#define BUFFER_SIZE 1024
#define COMMANDS_AMOUNT 6

int sockfd, numbytes;
struct addrinfo hints, *servinfo, *p;
int rv;
char s[INET6_ADDRSTRLEN];
int _fromlen;

char _sendBuffer[BUFFER_SIZE];
char _recvBuffer[BUFFER_SIZE];
char _inputBuffer[BUFFER_SIZE]; // a buffer of the user input
char* _commandString; // will contain the command string taken from _inputBuffer

int _exitFlag = 0; // closes the program when set to 1

void* get_in_addr(struct sockaddr* sa);

/*
* reads an integer between the values of "min" and "max" and returns it.
*/
int readInt(int min, int max);
void command_exit();

#endif
