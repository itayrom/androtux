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

#ifndef __BT_SERVER_H__
#define __BT_SERVER_H__

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

#include "server.h"

#define SVC_CHANNEL 1
#define SVC_NAME "AndroTux"
#define SVC_DESC "Androtux bluetooth server"
#define SVC_PROV "Itay Rom, Tal Sabo"

sdp_session_t* register_service();

int _btSockFd;
struct sockaddr_rc _btLocAddr, _btRemoteAddr;
socklen_t _opt;
sdp_session_t *sdp_session;

#endif
