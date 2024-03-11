#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../util.h"

int owned_by_root(const char *path) {
  struct stat sb;
  int r;

  r = stat(path, &sb);
  if (r < 0)
    return -errno;

  return sb.st_uid == 0;
}

int cat_file(const char *path) {
  _cleanup_close_ int fd = -EBADF;
  int r;
  struct stat sb;

  fd = open(path, O_RDONLY);
  if (fd < 0)
    return -errno;

  r = fstat(fd, &sb);
  if (r < 0)
    return -errno;

  r = sendfile(STDOUT_FILENO, fd, 0, sb.st_size);
  if (r < 0)
    return -errno;
  
  return 0;
}

int main(int argc, char *argv[]) {
  int r;

  if (argc < 2)
    die("Usage: %s <file>\n", argv[0]);

  r = owned_by_root(argv[1]);
  if (r < 0)
    die("Error checking file ownership: %s\n", strerror(-r));
  if (r > 0)
    die("Refusing to read %s owned by root.\n", argv[1]);

  /* for ease of example exploitation */
  sleep(5);

  r = cat_file(argv[1]);
  if (r < 0)
    die("Failed to cat file %s\n", argv[1]);

  return EXIT_SUCCESS;
}
