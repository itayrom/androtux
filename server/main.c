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
	//pid_t wChild, btChild;
	
	yes = 1;

	printf("AndroTux Wifi Server\n");
	printf("--------------------\n");

//	if ((wChild = fork()) == 0) {

		sdp_session = register_service();
		isBluetooth = initBtSocket();	
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

		printf("\nwaiting for connections...\n");
		
		handleClient_select();

		sdp_close(sdp_session);
//	} else if (wChild < 0) {
//		perror("fork: wireless");
//	}

//	printf("0. Exit");

	
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

int serverSend(client* c, const void* msg, int len, unsigned int flags) {
	int result;
	int tempResult, cLen, nextLen;
	
	result = cLen = 0;
	nextLen = (len < UDP_PACKET_SIZE) ? len : UDP_PACKET_SIZE;
	
	while(cLen < len) {
		usleep(1000);
		tempResult = send(c->fd, msg + cLen, nextLen, flags);
		
		if (tempResult < 0) {
			return tempResult;
		}
		
		cLen += nextLen;
		nextLen = (len - cLen < nextLen) ? len - cLen : nextLen;
		result += tempResult;
		
		printf("sent: %d of %d\n", cLen, len);
	}
	
	return result;
}

int serverReceive(client* c, void* buf, int len, unsigned int flags) {
	int result, addrLen;
	int tempResult, cLen, nextLen;
	
	addrLen = sizeof(struct sockaddr);

	result = cLen = 0;
	nextLen = (len < UDP_PACKET_SIZE) ? len : UDP_PACKET_SIZE;
	
	while(cLen < len) {
		tempResult = recv(c->fd, buf + cLen, nextLen, flags);

		if (tempResult < 0) {
			printf("1a\n");
			return tempResult;
		}
		
		cLen += nextLen;
		nextLen = (len - cLen < nextLen) ? len - cLen : nextLen;
		result += tempResult;
		
		printf("uploaded: %d of %d\n", cLen, len);
	}
	
	return result;
}

void handleClient_select() {
	int i;
	fd_set master;
	fd_set read_fds;
	int fdmax;
	client *newClient, *client;
	
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	
	FD_SET(_sockfd, &master);
	fdmax = _sockfd;

	initializeClientsArray();

	while(1) {
		read_fds = master;
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) != -1) {
			for (i = 0; i <= fdmax; i++) {
				if (FD_ISSET(i, &read_fds)) {
					if (i == _sockfd) { // new connection
						_addrLength = sizeof(_remoteAddr);
							
						_newfd = accept(i, (struct sockaddr*)&_remoteAddr, &_addrLength);

						if (_newfd != -1) {
							if ((newClient = createClient(_newfd)) == NULL) {
								close(i);
								FD_CLR(i, &master);
								continue;
							}

							FD_SET(_newfd, &master);
							fdmax = (_newfd > fdmax) ? _newfd : fdmax;
							
							inet_ntop(_remoteAddr.ss_family, get_in_addr((struct sockaddr*)&_remoteAddr), _remoteIP, INET6_ADDRSTRLEN);
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
					}else if (i == _btSockFd){
						_addrLength = sizeof(struct sockaddr_rc);
							
						_newfd = accept(i, (struct sockaddr*)&_remoteAddr, &_addrLength);

						if (_newfd != -1) {
							if ((newClient = createClient(_newfd)) == NULL) {
								close(i);
								FD_CLR(i, &master);
								continue;
							}

							FD_SET(_newfd, &master);
							fdmax = (_newfd > fdmax) ? _newfd : fdmax;
							
							ba2str(&_remAddr.rc_bdaddr, _remoteIP);
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
							//printf("From: %d | Messaged: %s\n", client->devId, _recvBuffer);
							for (i = 0; i < MAX_EVENTS; i++) {	
								memset(&client->events[i], 0, sizeof(client->events[i]));
							}
							// sleep(3);
							switch (_recvBuffer[0]) {
								case KEY_EVENT:
									client->events[0].type = EV_KEY;
									client->events[0].code = atoi(_recvBuffer+3); 
									client->events[0].value = (_recvBuffer[2] == '0') ? 0 : 1;

									switch (_recvBuffer[1]) {
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
								//	printf(">> %c\n", _recvBuffer[1]);
									handleEvents(_eventFd, client->events, 1);
									break;
								case REL_EVENT:
									client->events[0].type = EV_REL;
									client->events[0].code = REL_X;
									client->events[1].type = EV_REL;
									client->events[1].code = REL_Y;
									
									_tokenP = strtok(_recvBuffer+1, ",");
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

									_tokenP = strtok(_recvBuffer+1, ",");
									client->events[0].value = atof(_tokenP);
									_tokenP = strtok(NULL, ",");
									client->events[1].value = atof(_tokenP);

									handleEvents(client->gpFd, client->events, 2);
									break;
								case SCROLL_EVENT:
									client->events[0].type = EV_REL;
									client->events[0].code = REL_WHEEL;
									client->events[0].value = atoi(_recvBuffer+1);
									handleEvents(client->mFd, client->events, 1);
									break;
								case COMMAND_EVENT:
								/*	if (fork() == 0) {
										execv("/usr/bin/vlc", args);
									}*/
									break;
							};
						}else{ // connection closed
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
}
