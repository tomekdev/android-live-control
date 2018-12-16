#include <stdio.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <lzma.h>

int main()
{
	int fb_fd = open("/dev/graphics/fb0", O_RDONLY);
	if(fb_fd < 0)
		printf("Failed to open /dev/graphics/fb0: %s\n", strerror(errno));
	
	struct fb_var_screeninfo vinfo;
	if(ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) != 0)
	{
		printf("Failed to get fb info: %s\n", strerror(errno));
		close(fb_fd);
		return -1;
	}
	int sz = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	printf("Framebuffer size: %i\n", sz);
	if(fbp == MAP_FAILED)
	{
		printf("Failed to map framebuffer to memory: %s\n", strerror(errno));
		close(fb_fd);
		return -1;
	}
	//lzma_stream stream = LZMA_STREAM_INIT;
	//lzma_ret ret = lzma_easy_encoder(

	int outfd = open("a.bin", O_WRONLY | O_CREAT, 0666);
	if(outfd < 0)
	{
		printf("Failed to create a.bin: %s\n", strerror(errno));
	}
	else
	{
		write(outfd, fbp, sz);
		close(outfd);
	}	

	munmap(fbp, sz);
	close(fb_fd);

return 0;
}