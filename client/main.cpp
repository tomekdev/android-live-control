#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "../server/include/types.h"
#include <string.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>

int main()
{
	/* Initialize socket */
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd == 0)
	{
		printf("client/main.cpp:%i: failed to create socket: %s\n", __LINE__ - 2, strerror(errno));
		return -1;
	}
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);

	if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0)
	{
		printf("client/main.cpp:%i inet_pton() failed: %s\n", __LINE__	- 2, strerror(errno));
		return -1;
	}

	if(connect(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		printf("client/main.cpp:%i Connection failed: %s\n", __LINE__ - 2, strerror(errno));
		return -1;
	}
	uinput_packet sendpkt;
	memset(&sendpkt, 0, sizeof(uinput_packet));
	sendpkt.key_array[3].id = KEY_A;
	sendpkt.key_array[3].state = 1;
	sendpkt.request_quit = 0;
	write(socket_fd, &sendpkt, sizeof(uinput_packet));
	close(socket_fd);
return 0;
}