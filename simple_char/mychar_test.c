#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define DEVICE "/dev/mychar"

int main(void)
{
  int fd;
  char buf[] = "here is what you want";
  char buf2[60] = {0};
  int len;

  fd = open(DEVICE, O_RDWR);
  if(fd < 0)
  {
    perror("Open device failed\n");
    return -1;
  }

  len = write(fd, buf, sizeof(buf));
  printf("buf: %s %d\n", buf, len);

  len = read(fd, buf2, 10);
  printf("buf2: %s %d\n", buf2, len);

  close(fd);
  return 0;
}
