#include<linux/if_tun.h>
#include<net/if.h>
#include<sys/ioctl.h>

/*
 Tunnel Allocation code taken from class-provided code
 */
int tun_alloc(char *dev, int flags)
{
	struct ifreq ifr;
	int fd, err;
	char *clonedev = (char*)"/dev/net/tun";

	if( (fd = open(clonedev , O_RDWR)) < 0 )
	{
		perror("Opening /dev/net/tun");
		return fd;
	}

	memset(&ifr, 0, sizeof(ifr));

	ifr.ifr_flags = flags;

	if (*dev)
	{
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	}

	if( (err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0 )
	{
		perror("ioctl(TUNSETIFF)");
		close(fd);
		return err;
	}

	strcpy(dev, ifr.ifr_name);
	return fd;
}
