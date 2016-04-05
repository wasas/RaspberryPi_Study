#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>

struct gpio_config
{
	int button_num;
	int led_num;
};

int main(int argc, char **argv)
{
	int fd;
	struct gpio_config config;
	config.button_num = 2;
	config.led_num = 3;

	fd = open("/dev/Demo", O_RDWR);
	if (fd < 0) {
		perror("/dev/Demo");
		exit(0);
	}

	ioctl(fd,0,&config);

	while(1)
	{
	   sleep(2);
	}

	close(fd);
	return 0;
}
