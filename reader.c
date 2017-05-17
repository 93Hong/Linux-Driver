#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int
main(int argc, char *argv[]) {
	char buf[4096];
	int ret, fd;

	if (argc != 2)
		return -EINVAL;

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return -errno;

	while(1) {
		ret = read(fd, buf, sizeof(buf));
		if (ret < 0)
			return -errno;
		else if (ret == 0)
			break;

		ret = write(STDOUT_FILENO, buf, ret);
		if (ret < 0)
			return -errno;
	}

	close(fd);
	return 0;
}
