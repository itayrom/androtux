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

#include "bthandler.h"

sdp_session_t* register_service() {
	int err;
	sdp_session_t *session;
	uint8_t service_uuid_int[] = {0x07,0x29,0x3d,0xb4,0xa3, 0x23, 0x4e, 0x07,0x8b, 0x8b,0x25,0x0b,0x34,0x0e, 0x42, 0xa4};
	//uint8_t service_uuid_int[] = {0x73, 0x35, 0xb5, 0xf2, 0xec, 0x43, 0x4e, 0x53, 0x84, 0x19, 0x07, 0x65, 0x7a, 0x9a, 0x8d, 0x52};
	uint8_t rfcomm_channel = SVC_CHANNEL;
	uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid;

	sdp_list_t *l2cap_list = 0,
		*rfcomm_list = 0,
		*root_list = 0,
		*proto_list = 0,
		*access_proto_list = 0,
		*class_id_list;

	sdp_data_t *channel = 0, *psm = 0;

	sdp_record_t *record = sdp_record_alloc();

	sdp_uuid128_create(&svc_uuid, &service_uuid_int);
	sdp_set_service_id(record, svc_uuid);

	sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
	root_list = sdp_list_append(0, &root_uuid);
	sdp_set_browse_groups(record, root_list);

	sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
	l2cap_list = sdp_list_append(0, &l2cap_uuid);
	proto_list = sdp_list_append(0, l2cap_list);

	sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
	channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
	rfcomm_list = sdp_list_append(0, &rfcomm_uuid);
	sdp_list_append(rfcomm_list, channel);
	sdp_list_append(proto_list, rfcomm_list);

	access_proto_list = sdp_list_append(0, proto_list);
	sdp_set_access_protos(record, access_proto_list);

	class_id_list = sdp_list_append(0, &svc_uuid);
	sdp_set_service_classes(record, class_id_list);

	sdp_set_info_attr(record, SVC_NAME, SVC_PROV, SVC_DESC);
	
	err = 0;
	session = NULL;

	session = sdp_connect(BDADDR_ANY, BDADDR_LOCAL, SDP_RETRY_IF_BUSY);
	err = sdp_record_register(session, record, 0);

	if (err != 0) {
		perror("register bluetooth record");
	}

	sdp_data_free(channel);
	sdp_list_free(l2cap_list, 0);
	sdp_list_free(rfcomm_list, 0);
	sdp_list_free(root_list, 0);
	sdp_list_free(access_proto_list, 0);

	return session;
}
