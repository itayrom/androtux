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

int main(int argc, char* argv[]) {
	printf("AndroTux Wifi Server\n");
	printf("--------------------\n");

	
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
	
	_clientList = createClientList();
	
	while(1) {
		read_fds = master;
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) != -1) {
			for (i = 0; i <= fdmax; i++) {
				if (FD_ISSET(i, &read_fds)) {
					if (i == _sockfd) { // new connection
						_addrLength = sizeof _remoteAddr;
						_newfd = accept(_sockfd, (struct sockaddr*)&_remoteAddr, &_addrLength);
						if (_newfd != -1) {
							newClient = createClient(_newfd, NULL);
							addClient(_clientList, newClient);
							FD_SET(_newfd, &master);
							fdmax = (_newfd > fdmax) ? _newfd : fdmax;
							
							inet_ntop(_remoteAddr.ss_family, get_in_addr((struct sockaddr*)&_remoteAddr), _remoteIP, INET6_ADDRSTRLEN);
							printf("new connection from %s on socket %d\n", _remoteIP, _newfd);
							
							if (setup_uinput_device(newClient) < 0) {
								perror("device setup");
								newClient->uinpFd = -1;	
								goto close_conn;
							}
						}else{
							perror("accept");
						}
					}else{ // data from client
						client = getClient(_clientList, i);
						memset(_recvBuffer, '\0', BUFFER_SIZE);
						
						if ((_numbytes = recv(i, _recvBuffer, BUFFER_SIZE, 0)) > 0) { // data arrived from client
							printf("From: %d | Messaged: %s\n", client->devId, _recvBuffer);
							sleep(3);
							switch (_recvBuffer[0]) {
								case 'k':
									_keyCode = atoi(_recvBuffer+1);	
									press_key(client, _keyCode);
									break;
								case 'm':
									_tokenP = strtok(_recvBuffer+1, ",");
									_x = atoi(_tokenP);
									_tokenP = strtok(NULL, ",");
									_y = atoi(_tokenP);
									mouse_move(client, _x, _y);
									break;
								case 'a':
									_tokenP = strtok(_recvBuffer+1, ",");
									_x = atoi(_tokenP);
									_tokenP = strtok(NULL, ",");
									_y = atoi(_tokenP);
									mouse_move(client, _x, _y);
									break;
								case 's':
									_scroll = atoi(_recvBuffer+1);
									mouse_scroll(client, _scroll);
									break;
							};
						}else{ // connection closed
							close_conn:
							if (_numbytes == 0) { // initiated by client
								printf("socket %d closed the connection\n", i);
							}else{ // due to error
								perror("recv");
							}
							
							if (client->uinpFd > -1)
								close_uinput_device(client);

							removeClient(_clientList, i);
							
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
