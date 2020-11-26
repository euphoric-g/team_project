#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define BUF_SIZE 1024
#define TARGET_ADDR "07:B4:EC:20:A5:81"

int main(int argc, char **argv) {
	struct sockaddr_rc addr = { 0 };
	int s, status;
	char dest[18] = TARGET_ADDR;
	char buf[BUF_SIZE] = {0};

	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = 4;
	str2ba(dest, &addr.rc_bdaddr);
	status = connect(s, (struct sockaddr*)&addr, sizeof(addr));
	while(1) {
		recv(s, buf, BUF_SIZE, 0);
		if(status < 0) {
			perror("receive error");
			close(s);
			return -1;
		}
		printf("Data received : %s\n", buf);
	}
	close(s);
	return 0;
}`
