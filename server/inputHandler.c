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

static int devIdCount = 0;

int setup_uinput_device(client *client) {
	int i = 0;
	char devName[DEVICE_NAME_SIZE];
	
	if (!(client->uinpFd = open("/dev/uinput", O_WRONLY, O_NDELAY))) {
		printf("Unable to open /dev/uinput.\n");
		return 0;
	}
	
	memset(&client->uinp, 0, sizeof(client->uinp));

	client->devId = devIdCount;
	sprintf(devName, "AndroTux%d\n", devIdCount++);
	printf("Device name: %s\n", devName);	
	strncpy(client->uinp.name, devName, UINPUT_MAX_NAME_SIZE);
	client->uinp.id.version = 4;
	client->uinp.id.bustype = BUS_USB;
	
	ioctl(client->uinpFd, UI_SET_EVBIT, EV_KEY);
	ioctl(client->uinpFd, UI_SET_EVBIT, EV_REL);
	ioctl(client->uinpFd, UI_SET_EVBIT, EV_ABS);
	ioctl(client->uinpFd, UI_SET_EVBIT, EV_SYN);

	ioctl(client->uinpFd, UI_SET_RELBIT, REL_X);
	ioctl(client->uinpFd, UI_SET_RELBIT, REL_Y);
	ioctl(client->uinpFd, UI_SET_RELBIT, REL_WHEEL);
	
	for (i = 0; i < KEY_MAX; i++) {
		ioctl(client->uinpFd, UI_SET_KEYBIT, i);
	}

	client->uinp.absmin[ABS_X] = 0;
	client->uinp.absmax[ABS_X] = 1023;
	client->uinp.absmin[ABS_Y] = 0;
	client->uinp.absmax[ABS_Y] = 1023;

	ioctl(client->uinpFd, UI_SET_KEYBIT, BTN_MOUSE);
	ioctl(client->uinpFd, UI_SET_KEYBIT, BTN_TOUCH);
	
	ioctl(client->uinpFd, UI_SET_KEYBIT, BTN_LEFT);
	ioctl(client->uinpFd, UI_SET_KEYBIT, BTN_MIDDLE);
	ioctl(client->uinpFd, UI_SET_KEYBIT, BTN_RIGHT);
	ioctl(client->uinpFd, UI_SET_KEYBIT, BTN_FORWARD);
	ioctl(client->uinpFd, UI_SET_KEYBIT, BTN_BACK);
	
	ioctl(client->uinpFd, UI_SET_ABSBIT, ABS_X);
	ioctl(client->uinpFd, UI_SET_ABSBIT, ABS_Y);

	if (write(client->uinpFd, &(client->uinp), sizeof(client->uinp)) < 0) {
		perror ("error: write");
		return 0;
	}
	
	if (ioctl(client->uinpFd, UI_DEV_CREATE)) {
		printf("Unable to create uinput device.\n");
		return 0;
	}
	
	return 1;
}

void close_uinput_device(client *client) {
	ioctl(client->uinpFd, UI_DEV_DESTROY);
	close(client->uinpFd);
}

void press_key(client *client, int key) {
	// key pressed
	memset(&(client->event), 0, sizeof(client->event));
	gettimeofday(&client->event.time, NULL);
	client->event.type = EV_KEY;
	client->event.code = key;
	client->event.value = 1;
	write(client->uinpFd, &client->event, sizeof(client->event));
	
	client->event.type = EV_SYN;
	client->event.code = SYN_REPORT;
	client->event.value = 0;
	write(client->uinpFd, &(client->event), sizeof(client->event));
	
	// key released
	memset(&client->event, 0, sizeof(client->event));
	gettimeofday(&client->event.time, NULL);
	client->event.type = EV_KEY;
	client->event.code = key;
	client->event.value = 0;
	write(client->uinpFd, &(client->event), sizeof(client->event));
	
	client->event.type = EV_SYN;
	client->event.code = SYN_REPORT;
	client->event.value = 0;
	write(client->uinpFd, &(client->event), sizeof(client->event));
}

void mouse_move(client *client, int x, int y) {
	memset(&(client->event), 0, sizeof(client->event));
	gettimeofday(&client->event.time, NULL);
	client->event.type = EV_REL;
	client->event.code = REL_X;
	client->event.value = x;
	write(client->uinpFd, &client->event, sizeof(client->event));
	
	client->event.type = EV_REL;
	client->event.code = REL_Y;
	client->event.value = y;
	write(client->uinpFd, &client->event, sizeof(client->event));
	
	client->event.type = EV_SYN;
	client->event.code = SYN_REPORT;
	client->event.value = 0;
	write(client->uinpFd, &(client->event), sizeof(client->event));
}

void mouse_scroll(client *client, int amount) {
	memset(&(client->event), 0, sizeof(client->event));
	gettimeofday(&client->event.time, NULL);
	client->event.type = EV_REL;
	client->event.code = REL_WHEEL;
	client->event.value = amount;
	write(client->uinpFd, &client->event, sizeof(client->event));
	
	client->event.type = EV_SYN;
	client->event.code = SYN_REPORT;
	client->event.value = 0;
	write(client->uinpFd, &(client->event), sizeof(client->event));
}

void controller_abs(client *client, int x, int y) {
	memset(&(client->event), 0, sizeof(client->event));
	gettimeofday(&client->event.time, NULL);
	client->event.type = EV_ABS;
	client->event.code = ABS_X;
	client->event.value = x;
//	client->event.code = ABS_Y;
//	client->event.value = y;
	write(client->uinpFd, &client->event, sizeof(client->event));
	
	client->event.type = EV_SYN;
	client->event.code = SYN_REPORT;
	client->event.value = 0;
	write(client->uinpFd, &(client->event), sizeof(client->event));

}
