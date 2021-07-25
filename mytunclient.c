#include <fcntl.h>  /* O_RDWR */
#include <string.h> /* memset(), memcpy() */
#include <stdio.h> /* perror(), printf(), fprintf() */
#include <stdlib.h> /* exit(), malloc(), free() */
#include <sys/ioctl.h> /* ioctl() */
#include <unistd.h> /* read(), close() */
/* includes for struct ifreq, etc */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_tun.h>

int tun_open(char *devname)
{
  struct ifreq ifr;
  int fd, err;

  if ( (fd = open("/dev/net/tun", O_RDWR)) == -1 ) {
       perror("open /dev/net/tun");exit(1);
  }
  memset(&ifr, 0, sizeof(ifr));
  ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
  strncpy(ifr.ifr_name, devname, IFNAMSIZ); // devname = "tun0"

  /* ioctl will use ifr.if_name as the name of TUN 
   * interface to open: "tun0", etc. */
  if ( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) == -1 ) {
    perror("ioctl TUNSETIFF");close(fd);exit(1);
  }

  /* After the ioctl call the fd is "connected" to tun device specified
   * by devname ("tun0", "tun1", etc)*/

  return fd;
}

int main()
{
	int tun_fd, nbytes;
	char buffer[4096];
	tun_fd = tun_open("tun0");

	if (tun_fd < 0) {
		perror("Allocating interface");
		exit(1);
	}

	printf("Opened tun0 for reading\n");
	while (1) {
		unsigned char ip[4];
		nbytes = read(tun_fd, buffer, sizeof(buffer));
		if (nbytes < 0) {
		    perror("Reading from interface");
		    close(tun_fd);
		    exit(1);
		}
		printf("Read %d bytes from tun0 device\n", nbytes);
		/* Use Wireshark to see packet contents */
		memcpy(ip, &buffer[12], 4);
		memcpy(&buffer[12], &buffer[16], 4);
		memcpy(&buffer[16], ip, 4);
		buffer[20] = 0;
		*((unsigned short *)&buffer[22]) += 8;
		nbytes = write(tun_fd, buffer, nbytes);
		printf("Wrote %d bytes to tun0 device\n",nbytes);
	}
	return 0;
}
