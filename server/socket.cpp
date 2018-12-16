#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include "include/types.h"
#include "include/uinput.h"
#include <unistd.h>

struct socket_union initialize_sockets(int port)
{
	struct socket_union failed;
	failed.primary_socket = -1;
	failed.opened_socket = -1;
	
	int socket_fd = 0;
	int opt = 1;
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd == 0)
	{
		printf("server/socket.cpp:%i: failed to create socket: %s\n", __LINE__ - 2, strerror(errno));
		return failed;
	}
	/* Attach socket to given port */
	if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) != 0)
	{
		printf("server/socket.cpp:%i: failed to create socket: %s\n", __LINE__ - 2, strerror(errno));
		return failed;
	}
	
	struct sockaddr_in client_address;
	int client_address_size = sizeof(client_address);
	client_address.sin_family = AF_INET;
	client_address.sin_addr.s_addr = INADDR_ANY;
	client_address.sin_port = htons(port);
	
	/* Bind server to port */
	if(bind(socket_fd, (struct sockaddr *)&client_address, sizeof(client_address)) < 0)
	{
		printf("server/socket.cpp:%i: failed to create socket: %s\n", __LINE__ - 2, strerror(errno));
		close(socket_fd);
		return failed;
	}
	if(listen(socket_fd, 1) < 0)
	{
		printf("server/socket.cpp:%i: failed to create socket: %s\n", __LINE__ - 2, strerror(errno));
		close(socket_fd);
		return failed;
	}
	printf("OK\nListening for incoming connection on port %i\n", port);
	int new_socket = accept(socket_fd, (struct sockaddr *)&client_address, (socklen_t *)&client_address_size);
	if(new_socket < 0)
	{
		printf("server/socket.cpp:%i: failed to create new socket: %s\n", __LINE__ - 2, strerror(errno));
		close(socket_fd);
		return failed;
	}
	struct socket_union return_val;
	return_val.primary_socket = socket_fd;
	return_val.opened_socket = new_socket;

	return return_val;
	/*
	bool request_exit = false;
	while(!request_exit)
	{
		uinput_packet packet;
		memset(&packet, 0, sizeof(uinput_packet));
		int nreadbytes = read(new_socket, &packet, sizeof(uinput_packet));
		if(nreadbytes == 0)
		{
			printf("Connection closed. Exiting...\n");
			close(new_socket);
			close(socket_fd);
			request_exit = true;
		}
		else
		{
			parse_uinput_packet(packet);
		}
	} */
}