#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/sco.h>

#define TARGET_ADDR "XX:XX:XX:XX:XX:XX"

int main(int argc, char **argv) {
	inquiry_info *devices = NULL;
	int max_rsp, num_rsp;
	int adapter_id, sock, len, flags;
	int i;
	char target_addr[19] = TARGET_ADDR;
	char addr[19] = { 0 };
	char name[248] = { 0 };
	adapter_id = hci_get_route(NULL);
	sock = hci_open_dev(adapter_id);
	if(adapter_id < 0 || sock < 0) {
		perror("opening socket");
		exit(1);
	}
	len = 8;
	max_rsp = 255;
	flags = IREQ_CACHE_FLUSH;
	devices = (inquiry_info*) malloc(max_rsp * sizeof(inquiry_info));
	num_rsp = hci_inquiry(adapter_id, len, max_rsp, NULL, &devices, flags);
	if(num_rsp < 0) {
		perror("hci_inquiry");
	}
	for(i=0; i<num_rsp; i++) {
		ba2str(&(devices+i)->bdaddr, addr);
		memset(name, 0, sizeof(name));
		if( 0 != hci_read_remote_name(sock, &(devices+i)->bdaddr, sizeof(name), name, 0)) {
			strcpy(name, "[unknown]");
		}
		printf("%s %s\n", addr, name);
	}
	free(devices);
	close(sock);
	return 0;
}
