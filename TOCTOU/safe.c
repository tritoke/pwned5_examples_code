#include "../util.h"

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>

int owned_by_root(int fd) {
  struct stat sb;
  int r;

  r = fstat(fd, &sb);
  if (r < 0)
    return -errno;

  return sb.st_uid == 0;
}

int main(int argc, char *argv[]) {
  _cleanup_close_ int fd = -EBADF;
  int r;

  if (argc < 2)
    die("Usage: %s <file>\n", argv[0]);

  fd = open(argv[1], O_PATH|O_CLOEXEC);
  if (fd < 0)
    die("Failed to open %s\n", argv[1]);

  r = owned_by_root(fd);
  if (r < 0)
    die("Error checking file ownership: %s\n", strerror(-r));
  if (r > 0)
    die("Refusing to read %s owned by root.\n", argv[1]);

  /* for ease of example exploitation */
  sleep(5);

  r = cat_file_fd(fd);
  if (r < 0)
    die("Failed to cat file: %s\n", strerror(-r));

  return EXIT_SUCCESS;
}
