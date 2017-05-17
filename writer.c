#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int main(int argc, char *argv[]) {
	char buf[4096];
	int ret, len, i;
	int fd;

	if (argc != 2)
		return -EINVAL;

	
	fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC);
	if (fd < 0)
		return -errno;

	while(1) {
		ret = read(STDIN_FILENO, buf, sizeof(buf));
		if (ret < 0)
			return -errno;
		else if (ret == 0)
			break;

		for (i = 0, len = ret; i < len; ) {
			ret = write(fd, buf + i, len - i);
			if (ret < 0)
				return -errno;
			i += ret;
		}
	}
}

