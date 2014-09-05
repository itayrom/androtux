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

#include "clientManager.h"

static client* _clients[MAX_DEVICES];

void initializeClientsArray() {
	int i;

	for (i = 0; i < MAX_DEVICES; i++) {
		_clients[i] = NULL;
	}
}

client* createClient(int fd) {
	int i;

	for (i = 0; i < MAX_DEVICES; i++) {
		if (_clients[i] == NULL) {
			_clients[i] = (client*)malloc(sizeof(client)); 
			_clients[i]->devId = i;	
			_clients[i]->fd = fd;
			return _clients[i];
		}
	}

	return NULL;
}

int removeClient(int fd) {
	int i;
	
	for (i = 0; i < MAX_DEVICES; i++) {
		if(_clients[i] != NULL && _clients[i]->fd == fd) {
			free(_clients[i]);
			_clients[i] = NULL;
			return 1;
		}
	}

	return 0;
}

client* getClient(int fd) {
	int i;
	
	for (i = 0; i < MAX_DEVICES; i++) {
		if(_clients[i] != NULL && _clients[i]->fd == fd) {
			return _clients[i];
		}
	}

	return NULL;
}
