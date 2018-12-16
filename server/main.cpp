#include <stdio.h>
#include <stdlib.h>
#include "include/socket.h"
#include "include/types.h"
#include "include/uinput.h"
#include <linux/uinput.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define VERSION "1.0.0"

int main(int argc, char *argv[])
{
	int port = 8080;

	printf("Starting LIVE control server for Android devices\n");
	printf("Version: %s (%s)\n", VERSION, __DATE__);
	
	if(argc > 1)
	{
		if(argc > 2)
		{
			printf("%s: error: too many arguments\n", argv[0]);
			printf("Usage: %s PORT_NUMBER\n", argv[0]);
			printf("PORT_NUMBER is optional. If not specified server will start on port 8080\n");
			return -1;
		}
		else
		{
			port = strtol(argv[1], NULL, 10);
		}
	}
		
	printf("Initializing uinput virtual input devices (currently virtual keyboard only)...");
	int uinput_fd = initialize_uinput();
	
	if(uinput_fd == -1)
		return -1;
	
	printf("Initializing graphics capturing...(not yet)\n");

	printf("Initializing server on port %i...", port);
	struct socket_union sockets = initialize_sockets(port);
	if(sockets.opened_socket == -1)
		return -1;

	printf("Session ended. Exiting...\n");
	if(ioctl(uinput_fd, UI_DEV_DESTROY) < 0)
	{
		printf("server/main.cpp:%i: ioctl failed: %s", __LINE__ - 2, strerror(errno));
	}
	close(uinput_fd);
	close(sockets.opened_socket);
	close(sockets.primary_socket);
return 0;
}