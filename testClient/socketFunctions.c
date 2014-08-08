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

#include "socketFunctions.h"

int send_t(int sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, socklen_t tolen)
{
	int result;
	int tempResult, cLen, nextLen;
	
	result = cLen = 0;
	nextLen = (len < UDP_PACKET_SIZE) ? len : UDP_PACKET_SIZE;
	
	while(cLen < len)
	{
		usleep(1000);
		tempResult = send(sockfd, msg + cLen, nextLen, flags);
		
		if (tempResult < 0)
		{
			return tempResult;
		}
		
		cLen += nextLen;
		nextLen = (len - cLen < nextLen) ? len - cLen : nextLen;
		result += tempResult;
		
		printf("sent: %d of %d\n", cLen, len);
	}

	return result;
}

int recv_t(int sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from, int *fromlen)
{
	int result;
	int tempResult, cLen, nextLen;
	
	result = cLen = 0;
	nextLen = (len < UDP_PACKET_SIZE) ? len : UDP_PACKET_SIZE;
	
	while(cLen < len)
	{
		tempResult = recv(sockfd, buf + cLen, nextLen, flags);
		
		if (tempResult < 0)
		{
			return tempResult;
		}
		
		cLen += nextLen;
		nextLen = (len - cLen < nextLen) ? len - cLen : nextLen;
		result += tempResult;
		
		printf("downloaded: %d of %d\n", cLen, len);
	}
	
	return result;
}
