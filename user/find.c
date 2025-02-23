#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(const char *dir, const char *target) {
	char path[512], *p;
	int fd;
	struct dirent de;
	struct stat st;

	if ((fd = open(dir, 0)) < 0) {
		printf("find: cannot open %s\n", dir);
		return;
	}

	if (fstat(fd, &st) < 0) {
		printf("find: cannot stat %s\n", dir);
		close(fd);
		return;
	}

	if (st.type != T_DIR) {
		close(fd);
		return;
	}

	strcpy(path, dir);
	p = path + strlen(path);
	*p++ = '/';

	while (read(fd, &de, sizeof(de)) == sizeof(de)) {
		if (de.inum == 0) continue;

		if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

		memmove(p, de.name, strlen(de.name) + 1);

		if (stat(path, &st) < 0) {
			printf("find: cannot stat %s\n", path);
			continue;
		}
		if (st.type == T_FILE && strcmp(de.name, target) == 0) {
			printf("%s\n", path);
		}
		if (st.type == T_DIR) {
			find(path, target);
		}
	}
	close(fd);
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: find <directory> <filename>\n");
		exit(1);
	}
	find(argv[1], argv[2]);
	exit(0);
}
