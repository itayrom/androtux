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

#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <dirent.h>
#include <fcntl.h>

#include "clientManager.h"
#include "inputHandler.h"
#include "bthandler.h"

#define PORT "3490"
#define BACKLOG 10

#define BUFFER_SIZE 1024
#define UDP_PACKET_SIZE 1024

#define KEY_EVENT 'k'
#define REL_EVENT 'r'
#define ABS_EVENT 'a'
#define SCROLL_EVENT 's'
#define COMMAND_EVENT 'c'

#define KEYBOARD_KEY 'k'
#define MOUSE_KEY 'm'
#define GAMEPAD_KEY 'g'

#define log(str) printf("> %s\n", str)

int _sockfd, _newfd;
struct addrinfo hints, *servinfo, *p;
struct sockaddr_storage _remoteAddr;
socklen_t _addrLength;
struct sigaction sa;
char _remoteIP[INET6_ADDRSTRLEN];
int rv;
int _numbytes;
char _recvBuffer[BUFFER_SIZE];

void sigchld_handler(int s);
void* get_in_addr(struct sockaddr *sa);

int initWireless();
int initBlutooth();
int handleInput(client *client, char *input);

int _clientId, _eventFd, _x, _y, _scroll;
char *_tokenP;

#endif
