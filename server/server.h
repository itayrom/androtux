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

#ifndef __SERVER_H__
#define __SERVER_H__

#define PORT "3490"
#define BACKLOG 10

#define BUFFER_SIZE 1024
#define UDP_PACKET_SIZE 1024

#define KEY_EVENT 'k'
#define REL_EVENT 'r'
#define ABS_EVENT 'a'
#define SCROLL_EVENT 's'

#define KEYBOARD_KEY 'k'
#define MOUSE_KEY 'm'
#define GAMEPAD_KEY 'g'

int _sockfd, _newfd;
struct addrinfo hints, *servinfo, *p;
struct sockaddr_storage _remoteAddr;
socklen_t _addrLength;
struct sigaction sa;
int yes = 1;
char _remoteIP[INET6_ADDRSTRLEN];
int rv;
int _numbytes;
char _recvBuffer[BUFFER_SIZE];
char _sendBuffer[BUFFER_SIZE];

void sigchld_handler(int s);
void* get_in_addr(struct sockaddr *sa);

void handleWireless();
void handleBlutooth();

int serverSend(client* c, const void* msg, int len, unsigned int flags);
int serverReceive(client* c, void* buf, int len, unsigned int flags);

int processCommandLineArgs(int argc, char* argv[]);

void handleClient_select();

int _clientId, _eventFd, _x, _y, _scroll;
char *_tokenP;

#endif
