#include <fcntl.h>  /* O_RDWR */
#include <string.h> /* memset(), memcpy() */
#include <stdio.h> /* perror(), printf(), fprintf() */
#include <stdlib.h> /* exit(), malloc(), free() */
#include <sys/ioctl.h> /* ioctl() */
#include <unistd.h> /* read(), close() */
/* includes for struct ifreq, etc */
#include <sys/types.h>
#include <linux/if.h>
#include <linux/if_tun.h>

void disp_arp_request(char* buf,int len)
{
	for(int i=0;i<len;i++)
	{
		unsigned char content = buf[i];
		printf("%02X ",content);
		if(i!=0&&i%15==0)
		{
			printf("\n");
		}
	}
}

int tap_open(char *devname)
{
  struct ifreq ifr;
  int fd, err;

  if ( (fd = open("/dev/net/tun", O_RDWR)) == -1 ) {
       perror("open /dev/net/tun");exit(1);
  }
  memset(&ifr, 0, sizeof(ifr));
  ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
  strncpy(ifr.ifr_name, devname, IFNAMSIZ); // devname = "tap0"

  /* ioctl will use ifr.if_name as the name of TAP 
   * interface to open: "tap0", etc. */
  if ( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) == -1 ) {
    perror("ioctl TUNSETIFF");close(fd);exit(1);
  }

  /* After the ioctl call the fd is "connected" to tap device specified
   * by devname ("tap0", "tap1", etc)*/

  return fd;
}


int main()
{
	int tap_fd, nbytes;
	char buffer[4096];
	unsigned char my_mac[]={0x00,0x00,0x00,0x00,0x00,0x02};
	tap_fd = tap_open("tap0");
	if (tap_fd < 0) {
		perror("Allocating interface");
		exit(1);
	}
	printf("Opened tap0 for reading\n");
	while (1) {
		/* First, check for ARP request and reply */
		/* to register device with Address */
		nbytes = read(tap_fd, buffer, sizeof(buffer));
		if (nbytes < 0) {
		    perror("Reading from interface");
		    close(tap_fd);
		    exit(1);
		}
		/* Trying to check if it is an ARP request */
		/* Find the sequence 08 06 in hex, which indicates */
		/* an ARP request */
		int arp_flag=0;
		unsigned char arp_start=0x08;
		unsigned char arp_end=0x06;
		int arp_index=-1;
		for(int i=0;i<nbytes-1;i++)
		{
			if(buffer[i]==arp_start&&buffer[i+1]==arp_end)
			{
				arp_flag=1;
				arp_index=i+2;
				break;
			}
		}
		if(arp_flag==1)
		{
			printf("Received an ARP request.\n");
			/* Optional display of contents of ARP request */
			/*
			printf("Displaying contents of ARP request.\n");
			disp_arp_request(&buffer[0],nbytes);
			printf("\n");
			*/
			unsigned char ip[4];
			unsigned char src_mac[6];
			/* Now, reply to the ARP request */
			/* Set MAC address and reverse source destination */
			/* ip addresses as well as source destination */
			/* MAC addresses. */
			/* From arp_index, at an offset of 8, lies */
			/* the source mac address, to be filled by us*/
			/* From arp_index, at an offset of 18, lies */
			/* the destination mac address, to be replaced */
			/* by source mac address. */
			/* The sender ip address is at an offset of 14 */
			/* with a size of 4 bytes while the destination */
			/* ip address is at an offset of 24 bytes, with a */
			/* size of 4 bytes. */
			/* Change the opcode as well to 0x02, indicating a reply */
			/* opcode is at an offset of 6-7 bytes */
			/* Change arp_index+7 to 0x02 */
			/* Change source mac address to my_mac and change */
			/* destination mac address to src_mac */
			
			memcpy(ip, &buffer[arp_index+14], 4);
			memcpy(&buffer[arp_index+14], &buffer[arp_index+24], 4);
			memcpy(&buffer[arp_index+24], ip, 4);
			memcpy(&src_mac[0],&buffer[arp_index+8],6);
			memcpy(&buffer[arp_index+8],&my_mac[0],6);
			memcpy(&buffer[arp_index+18],&src_mac[0],6);
			buffer[arp_index+7] = 2;
			/* Setting the destination to src_mac address*/
			memcpy(&buffer[0],&src_mac[0],6);
			/* Setting the source to my_mac address*/
			memcpy(&buffer[6],&my_mac[0],6);
			nbytes = write(tap_fd, buffer, nbytes);
			printf("Wrote %d bytes to tap0 device\n",nbytes);
		}
	}
	return 0;
}
