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

#ifndef __CLIENT_MANAGER_H__
#define __CLIENT_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <linux/input.h>
#include <sys/types.h>
#include <linux/uinput.h>

#define MAX_DEVICES 4
#define MAX_EVENTS 2
#define BUFFER_SIZE 1024

typedef struct client client;

struct client {
	int fd;
	int devId;
	int kbFd;
	int mFd;
	int gpFd;
	struct uinput_user_dev kbDev;
	struct uinput_user_dev mDev;
	struct uinput_user_dev gpDev;
	struct input_event events[MAX_EVENTS];
	struct sockaddr* addr;
};

void initializeClientsArray();
client* createClient(int fd);
int removeClient(int fd);
client* getClient(int fd);

#endif
