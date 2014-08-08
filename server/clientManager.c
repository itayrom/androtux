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

clientList* createClientList() {
	clientList* list;
	list = (clientList*)malloc(sizeof(clientList));
	list->base = NULL;
	return list;
}

int removeClient(clientList* list, int fd) {
	int status;
	client* c;
	
	status = 0;
	c = getClient(list, fd);
	
	if (c != NULL) {
		if (c == list->base) {
			list->base = c->next;
		} else {	
			(c->prev)->next = c->next;
			if (c->next != NULL) {
				(c->next)->prev = c->prev;
			}
		}
		status = 1;
	}
	
	return status;
}

void addClient(clientList* list, client* c) {
	client* current;
	current = list->base;
	
	c->prev = NULL;
	if (list->base == NULL) {
		list->base = c;
	} else {
		while (current->next != NULL) {
			current = current->next; 
		}

		current->next = c;
	}
	
	c->prev = current;
}

client* getClient(clientList* list, int fd) {
	client* result;
	client* current;
	current = list->base;
	
	if (list->base == NULL) {
		result = NULL;
	} else {
		while (current != NULL) {
			if (current->fd == fd) {
				result = current;
				break;
			}
			
			current = current->next;
		}
	}
	
	return result;
}

client* getClientByAddress(clientList* list, struct sockaddr* addr) {
	char paramAddrString[INET6_ADDRSTRLEN];
	char currentAddrString[INET6_ADDRSTRLEN];
	
	client* result;
	client* current;
	current = list->base;
	
	if (list->base == NULL) {
		result = NULL;
	} else {
		inet_ntop(AF_INET, get_in_addr(addr), paramAddrString, sizeof(paramAddrString));
		
		while (current != NULL) {
			inet_ntop(AF_INET, get_in_addr(current->addr), currentAddrString, sizeof(currentAddrString));
			
			if (strcmp(currentAddrString, paramAddrString) == 0) {
				result = current;
				break;
			}
			
			current = current->next;
		}
	}
	
	return result;
}

client* createClient(int fd,  struct sockaddr* addr) {
	client* newClient;
	newClient = (client*)malloc(sizeof(client));
	newClient->fd = fd;
	newClient->addr = addr;
	//newClient->addr = (struct sockaddr)malloc(sizeof(struct sockaddr));
	//memcpy(newClient->addr, addr, sizeof(struct sockaddr));
	newClient->next = NULL;
	return newClient;
}

