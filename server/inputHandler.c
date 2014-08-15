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

#include "inputHandler.h"

static int setupKeyboard(client *client);
static int setupMouse(client *client);
static int setupGamepad(client *client);

int setup_uinput_device(client *client) {
	setupKeyboard(client);
	setupMouse(client);
	setupGamepad(client);
	
	return 0;
}

static int setupKeyboard(client *client) {
	int i;
	char devName[DEVICE_NAME_SIZE];
	
	if (!(client->kbFd = open("/dev/uinput", O_WRONLY, O_NDELAY))) {
		printf("Unable to open /dev/uinput.\n");
		return 1;
	}
	
	memset(&client->kbDev, 0, sizeof(client->kbDev));

	sprintf(devName, "AndroTux%d[Keyboard]\n", client->devId);
	printf("Device name: %s\n", devName);	
	strncpy(client->kbDev.name, devName, UINPUT_MAX_NAME_SIZE);
	client->kbDev.id.bustype = BUS_USB;
	client->kbDev.id.vendor = 0;
	client->kbDev.id.product = 0;
	client->kbDev.id.version = 1;

	ioctl(client->kbFd, UI_SET_EVBIT, EV_KEY);
	ioctl(client->kbFd, UI_SET_EVBIT, EV_SYN);
	
	for (i = 0; i < 256 ; i++) {
		ioctl(client->kbFd, UI_SET_KEYBIT, i);
	}

	if (write(client->kbFd, &(client->kbDev), sizeof(client->kbDev)) < 0) {
		perror ("error: write");
		return 1;
	}
	
	if (ioctl(client->kbFd, UI_DEV_CREATE)) {
		printf("Unable to create uinput device.\n");
		return 1;
	}

	return 0;
}

static int setupMouse(client *client) {
	char devName[DEVICE_NAME_SIZE];
	
	if (!(client->mFd = open("/dev/uinput", O_WRONLY, O_NDELAY))) {
		printf("Unable to open /dev/uinput.\n");
		return 0;
	}
	
	memset(&client->mDev, 0, sizeof(client->mDev));

	sprintf(devName, "AndroTux%d[Mouse]\n", client->devId);
	printf("Device name: %s\n", devName);	
	strncpy(client->mDev.name, devName, UINPUT_MAX_NAME_SIZE);
	client->mDev.id.bustype = BUS_USB;
	client->mDev.id.vendor = 0;
	client->mDev.id.product = 1;
	client->mDev.id.version = 1;

	ioctl(client->mFd, UI_SET_EVBIT, EV_REL);
	ioctl(client->mFd, UI_SET_EVBIT, EV_KEY);
	ioctl(client->mFd, UI_SET_EVBIT, EV_SYN);
	
	ioctl(client->mFd, UI_SET_RELBIT, REL_X);
	ioctl(client->mFd, UI_SET_RELBIT, REL_Y);
	ioctl(client->mFd, UI_SET_RELBIT, REL_WHEEL);

	ioctl(client->mFd, UI_SET_KEYBIT, BTN_MOUSE);
	ioctl(client->mFd, UI_SET_KEYBIT, BTN_TOUCH);
	
	ioctl(client->mFd, UI_SET_KEYBIT, BTN_LEFT);
	ioctl(client->mFd, UI_SET_KEYBIT, BTN_MIDDLE);
	ioctl(client->mFd, UI_SET_KEYBIT, BTN_RIGHT);
	ioctl(client->mFd, UI_SET_KEYBIT, BTN_FORWARD);
	ioctl(client->mFd, UI_SET_KEYBIT, BTN_BACK);

	if (write(client->mFd, &(client->mDev), sizeof(client->mDev)) < 0) {
		perror ("error: write");
		return 0;
	}
	
	if (ioctl(client->mFd, UI_DEV_CREATE)) {
		printf("Unable to create uinput device.\n");
		return 0;
	}
}

static int setupGamepad(client *client) {
	char devName[DEVICE_NAME_SIZE];
	
	if (!(client->gpFd = open("/dev/uinput", O_WRONLY, O_NDELAY))) {
		printf("Unable to open /dev/uinput.\n");
		return 0;
	}
	
	memset(&client->gpDev, 0, sizeof(client->mDev));

	sprintf(devName, "AndroTux%d[Gamepad]\n", client->devId);
	printf("Device name: %s\n", devName);	
	strncpy(client->gpDev.name, devName, UINPUT_MAX_NAME_SIZE);
	client->gpDev.id.bustype = BUS_USB;
	client->gpDev.id.vendor = 0;
	client->gpDev.id.product = 2;
	client->gpDev.id.version = 1;

	ioctl(client->gpFd, UI_SET_EVBIT, EV_ABS);
	ioctl(client->gpFd, UI_SET_EVBIT, EV_KEY);
	ioctl(client->gpFd, UI_SET_EVBIT, EV_SYN);
	
	ioctl(client->gpFd, UI_SET_ABSBIT, ABS_X);
	ioctl(client->gpFd, UI_SET_ABSBIT, ABS_Y);

	client->gpDev.absmin[ABS_X] = -1023;
	client->gpDev.absmax[ABS_X] = 1023;;
	client->gpDev.absfuzz[ABS_X] = 0;
	client->gpDev.absflat[ABS_X] = 0;

	client->gpDev.absmin[ABS_Y] = -1023;;
	client->gpDev.absmax[ABS_Y] = 1023;;
	client->gpDev.absfuzz[ABS_Y] = 0;
	client->gpDev.absflat[ABS_Y] = 0;

	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_GAMEPAD);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_A);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_B);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_C);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_X);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_Y);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_Z);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_TL);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_TR);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_TL2);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_TR2);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_SELECT);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_START);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_MODE);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_THUMBL);
	ioctl(client->gpFd, UI_SET_KEYBIT, BTN_THUMBR);

	if (write(client->gpFd, &(client->gpDev), sizeof(client->gpDev)) < 0) {
		perror ("error: write");
		return 0;
	}
	
	if (ioctl(client->gpFd, UI_DEV_CREATE)) {
		printf("Unable to create uinput device.\n");
		return 0;
	}
}

void close_uinput_device(client *client) {
	if (client->kbFd > -1) {
		ioctl(client->kbFd, UI_DEV_DESTROY);
		close(client->kbFd);
	}
	
	if (client->mFd > -1) {
		ioctl(client->mFd, UI_DEV_DESTROY);
		close(client->mFd);
	}

	if (client->gpFd > -1) {
		ioctl(client->gpFd, UI_DEV_DESTROY);
		close(client->gpFd);
	}
}

void handleEvents(int fd, struct input_event events[], int n) {
	int i;
	struct input_event event;

	printf("IN>> %d, %d\n", events[0].code, events[1].code);
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, NULL);
	
	for (i = 0; i < n; i++) { 
		event.type = events[i].type;
		event.code = events[i].code;
		event.value = events[i].value;
		write(fd, &event, sizeof(event));
	}	

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(fd, &event, sizeof(event));
}
