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

#include "server.h"

int main(int argc, char *argv[]) {
	int i;
	fd_set master;
	fd_set read_fds;
	int fdmax;
	client *newClient, *client;
	
	_opt = sizeof(_btRemoteAddr);
	_addrLength = sizeof(_remoteAddr);

	printf("AndroTux Server\n");
	printf("---------------\n");

	initWireless();
	initBluetooth();

	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	
	FD_SET(_sockfd, &master);
	FD_SET(_btSockFd, &master);
	fdmax = (_sockfd >= _btSockFd) ? _sockfd : _btSockFd;

	initializeClientsArray();

	while(1) {
		read_fds = master;
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) != -1) {
			for (i = 0; i <= fdmax; i++) {
				if (FD_ISSET(i, &read_fds)) {
					if (i == _sockfd || i == _btSockFd) { // new connection
						if (i == _sockfd)
							_newfd = accept(i, (struct sockaddr*)&_remoteAddr, &_addrLength);
						else
							_newfd = accept(i, (struct sockaddr*)&_btRemoteAddr, &_opt);

						if (_newfd != -1) {
							if ((newClient = createClient(_newfd)) == NULL) {
								close(i);
								FD_CLR(i, &master);
								continue;
							}

							FD_SET(_newfd, &master);
							fdmax = (_newfd > fdmax) ? _newfd : fdmax;
							
							// get a string representation of the client's address into _remoteIp
							if (i == _sockfd)
								inet_ntop(_remoteAddr.ss_family, get_in_addr((struct sockaddr*)&_remoteAddr), _remoteIP, INET6_ADDRSTRLEN);
							else
								ba2str(&_btRemoteAddr.rc_bdaddr, _remoteIP);
								
							printf("new connection from %s on socket %d\n", _remoteIP, _newfd);
							
							if (newClient == NULL || setup_uinput_device(newClient) < 0) {
								perror("device setup");
								newClient->kbFd = -1;	
								newClient->mFd = -1;	
								newClient->gpFd = -1;	
								goto close_conn;
							}
						}else{
							perror("accept");
						}
					}else{ // data from client
						client = getClient(i);
						memset(_recvBuffer, '\0', BUFFER_SIZE);
						
						if ((_numbytes = recv(i, _recvBuffer, BUFFER_SIZE, 0)) > 0) { // data arrived from client
							for (i = 0; i < MAX_EVENTS; i++) {	
								memset(&client->events[i], 0, sizeof(client->events[i]));
							}
							
							handleInput(client, _recvBuffer);
						} else { // connection closed
							close_conn:
							if (_numbytes == 0) { // initiated by client
								printf("socket %d closed the connection\n", i);
							}else{ // due to error
								perror("recv");
							}
							
							close_uinput_device(client);

							removeClient(i);
							
							close(i);
							FD_CLR(i, &master);
						}
					}
				}
			}
		}else{
			perror("select");
			exit(4);
		}
	}
	
	sdp_close(sdp_session);
	
	return 0;
}

int initWireless() {
	int yes = 1;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
       
	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
       
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((_sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
	       
		if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}
	       
		if (bind(_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(_sockfd);
			perror("server: bind");
			continue;
		}
	       
		break;
	}
       
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}
       
	freeaddrinfo(servinfo);
       
	if (listen(_sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	log("Wireless initialized");
	
	return 0;
}

int initBluetooth() {
	memset(&_btLocAddr, 0, sizeof _btLocAddr);
	memset(&_btRemoteAddr, 0, sizeof _btRemoteAddr);
	_opt = sizeof(_btRemoteAddr);
	
	sdp_session = register_service();

	_btLocAddr.rc_family = AF_BLUETOOTH;
	_btLocAddr.rc_bdaddr = *BDADDR_ANY;
	_btLocAddr.rc_channel = (uint8_t)SVC_CHANNEL;

	if ((_btSockFd = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) == -1) {
		perror("bluetooth: socket");
	}

	if (bind(_btSockFd, (struct sockaddr*)&_btLocAddr, sizeof(_btLocAddr)) == -1) {
		close(_btSockFd);
		perror("bluetooth: bind");
	}
	
	if (listen(_btSockFd, BACKLOG) == -1) {
		perror("bluetooth: listen");
	}

	log("Bluetooth initialized");

	return 0;
}

void sigchld_handler(int s) {
	while (waitpid(-1, NULL, WNOHANG > 0));
}

void* get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int handleInput(client *client, char *input) {
	switch (input[0]) {
		case KEY_EVENT:
			client->events[0].type = EV_KEY;
			client->events[0].code = atoi(input+3); 
			client->events[0].value = (input[2] == '0') ? 0 : 1;

			switch (input[1]) {
				case KEYBOARD_KEY:
					_eventFd = client->kbFd;
					break;
				case MOUSE_KEY:
					_eventFd = client->mFd;
					break;
				case GAMEPAD_KEY:
					_eventFd = client->gpFd;
					break;
			}
			handleEvents(_eventFd, client->events, 1);
			break;
		case REL_EVENT:
			client->events[0].type = EV_REL;
			client->events[0].code = REL_X;
			client->events[1].type = EV_REL;
			client->events[1].code = REL_Y;
			
			_tokenP = strtok(input+1, ",");
			client->events[0].value = atof(_tokenP);
			_tokenP = strtok(NULL, ",");
			client->events[1].value = atof(_tokenP);
			
			handleEvents(client->mFd, client->events, 2);
			break;
		case ABS_EVENT:
			client->events[0].type = EV_ABS;
			client->events[0].code = ABS_X;
			client->events[1].type = EV_ABS;
			client->events[1].code = ABS_Y;

			_tokenP = strtok(input+1, ",");
			client->events[0].value = atof(_tokenP);
			_tokenP = strtok(NULL, ",");
			client->events[1].value = atof(_tokenP);

			handleEvents(client->gpFd, client->events, 2);
			break;
		case SCROLL_EVENT:
			client->events[0].type = EV_REL;
			client->events[0].code = REL_WHEEL;
			client->events[0].value = atoi(input+1);
			handleEvents(client->mFd, client->events, 1);
			break;
		case COMMAND_EVENT:
		/*	if (fork() == 0) {
				execv("/usr/bin/vlc", args);
			}*/
			break;
	};
}
