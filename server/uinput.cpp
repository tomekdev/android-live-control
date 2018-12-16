#include <linux/uinput.h>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "include/types.h"

void send_key(int fd, int type, int code, int value)
{
	struct input_event ev;
	ev.type = type;
	ev.code = code;
	ev.value = value;
	/* Ignore timestamp so set to 0 */
	ev.time.tv_usec = 0;
	ev.time.tv_sec = 0;
	write(fd, &ev, sizeof(ev));
}

int initialize_uinput()
{
	int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	
	if(fd < 0)
	{
		printf("Failed to open /dev/uinput: %s\n", strerror(errno));
		return -1;
	}

	/* Get uinput version */
	int version = 0;
	if(ioctl(fd, UI_GET_VERSION, &version) != 0)
	{
		printf("server/uinput.cpp:%i: ioctl failed: %s\n", __LINE__ - 2, strerror(errno));
		printf("Assuming legacy version of uinput\n");
		version = 1;
	}
		
	
	/* Enable keyboard device to pass SPACE key */
	if(ioctl(fd, UI_SET_EVBIT, EV_KEY) != 0)
		printf("line %i: ioctl failed: %s\n", __LINE__ - 1, strerror(errno));
	if(ioctl(fd, UI_SET_KEYBIT, KEY_A) != 0)
		printf("line %i: ioctl failed: %s\n", __LINE__ - 1, strerror(errno));

	if(version >= 5)
	{
		printf("Using uinput version %i\n", version);
		struct uinput_setup devsetup;	
		memset(&devsetup, 0, sizeof(devsetup));
		devsetup.id.bustype = BUS_USB;
		devsetup.id.vendor = 0x0000;
		devsetup.id.product = 0x0000;
		strcpy(devsetup.name, "Example keyboard emulated device");

		if(ioctl(fd, UI_DEV_SETUP, &devsetup) != 0)
		{
			printf("line %i: ioctl failed: %s\n", __LINE__ - 2, strerror(errno));
			close(fd);
			return -1;
		}
		if(ioctl(fd, UI_DEV_CREATE) != 0)
		{
			printf("line %i: ioctl failed: %s\n", __LINE__ - 2, strerror(errno));
			close(fd);
			return -1;
		}
	}
	else
	{
		/* Old uinput interface */
		struct uinput_user_dev devsetup;
		devsetup.id.bustype = BUS_USB;
		devsetup.id.vendor = 0x0000;
		devsetup.id.product = 0x0000;
		strcpy(devsetup.name, "Example uinput emulated keyboard device");

		write(fd, &devsetup, sizeof(devsetup));
		if(ioctl(fd, UI_DEV_CREATE) != 0)
		{
			printf("line %i: ioctl failed: %s\n", __LINE__ - 2, strerror(errno));
			close(fd);
			return -1;
		}
	}
	return fd;
}
int parse_uinput_packet(uinput_packet &p, int uinput_fd)
{
	send_key(uinput_fd, 
}