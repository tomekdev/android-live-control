#include <linux/uinput.h>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>

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

int main()
{
	struct uinput_setup devsetup;
	int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	
	if(fd < 0)
	{
		printf("Failed to open /dev/uinput: %s\n", strerror(errno));
		return errno;
	}
	
	/* Enable keyboard device to pass SPACE key */
	if(ioctl(fd, UI_SET_EVBIT, EV_KEY) != 0)
		printf("line %i: ioctl failed: %s\n", __LINE__ - 1, strerror(errno));
	if(ioctl(fd, UI_SET_KEYBIT, KEY_A) != 0)
		printf("line %i: ioctl failed: %s\n", __LINE__ - 1, strerror(errno));
	
	memset(&devsetup, 0, sizeof(devsetup));
	devsetup.id.bustype = BUS_USB;
	devsetup.id.vendor = 0x1234;
	devsetup.id.product = 0x5678;
	strcpy(devsetup.name, "Example keyboard emulated device");
	
	write(fd, &devsetup, sizeof(devsetup));
	
	if(ioctl(fd, UI_DEV_SETUP, &devsetup) != 0)
		printf("line %i: ioctl failed: %s\n", __LINE__ - 1, strerror(errno));
	if(ioctl(fd, UI_DEV_CREATE) != 0)
		printf("line %i: ioctl failed: %s\n", __LINE__ - 1, strerror(errno));
	
	/* Give time for userspace to recognize device */
	sleep(3);
	send_key(fd, EV_KEY, KEY_A, 1); /* Send key state pressed */
	send_key(fd, EV_SYN, SYN_REPORT, 1); /* Synchronize */
	sleep(1);
	send_key(fd, EV_KEY, KEY_A, 0); /* Send key state released */
	send_key(fd, EV_SYN, SYN_REPORT, 0); /* Synchronize */
	
	/* Give time for userspace to process events before destroying device */
	sleep(2);
	ioctl(fd, UI_DEV_DESTROY);
	close(fd);

return 0;
}