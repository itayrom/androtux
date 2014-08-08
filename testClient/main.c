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

#include "client.h"
 
int main(int argc, char* argv[])
{
	printf("AndroTux Server Test Client\n");
	printf("---------------------------\n");
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if ((rv = getaddrinfo("127.0.0.1", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		
		break;
	}
	
	if (p == NULL) {
		fprintf(stderr, "client: failed to bind\n");
		return 2;
	}
	
	printf("connecting to %s\n", "127.0.0.1");
	
	freeaddrinfo(servinfo);
	
	while(!_exitFlag)
	{
		memset(_sendBuffer, 0, BUFFER_SIZE);
		memset(_recvBuffer, 0, BUFFER_SIZE);
				
		printf(">");
		fgets(_sendBuffer, BUFFER_SIZE-1 , stdin);
		
		if (strncmp(_sendBuffer, "exit", 4) == 0) {
			command_exit();
		} else {
		
			*(strchr(_sendBuffer, '\n')) = '\0'; // removes the newline char from the input
		
			if (send_t(sockfd, _sendBuffer, strlen(_sendBuffer), 0, p->ai_addr, p->ai_addrlen) != -1) {
				/*if ((numbytes = recv_t(sockfd, _recvBuffer, BUFFER_SIZE, 0, p->ai_addr, &_fromlen)) != -1) {
					printf("%s\n", _recvBuffer);	
				} else {
					perror("recv");
				}*/
			} else {
				perror("ls");	
			}

			//while(strtok(NULL, " ") != NULL) {}
		}
	}
	
	close(sockfd);
	
	return 0;	
}

void* get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int readInt(int min, int max) {
	char input[80];
	int value;
	value = min - 1;
	
	while (value < min || value > max) {
		printf("> ");
		fgets(input, sizeof(input)-1 , stdin);
		sscanf(input, "%d", &value);
		
		if (value < min || value > max) {
			printf("Invalid input. must be a number between %d and %d.\n", min, max);
		}
	}
	
	return value;
}

void command_exit() {
	printf("Goodbye!\n");
	_exitFlag = 1;	
}
