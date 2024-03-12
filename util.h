#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <linux/openat2.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

static inline void die(const char *s, ...) {
  va_list ap;
  va_start(ap, s);
  vfprintf(stderr, s, ap);
  exit(EXIT_FAILURE);
}

static inline void freep(void *p) {
  free(*(void**)p);
  *(void**)p = NULL;
}

static inline void closep(int *fd) {
  close(*fd);
  *fd = -EBADF;
}

#define _cleanup_free_ __attribute__((cleanup(freep)))
#define _cleanup_close_ __attribute__((cleanup(closep)))

static inline int cat_file_inner(int fd) {
  struct stat sb;

  if (fstat(fd, &sb) < 0)
    return -errno;

  if (sendfile(STDOUT_FILENO, fd, 0, sb.st_size) < 0)
    return -errno;

  return 0;
}

static inline int cat_file(const char *path) {
  _cleanup_close_ int fd = -EBADF;

  fd = open(path, O_RDONLY);
  if (fd < 0)
    return -errno;

  return cat_file_inner(fd);
}

static inline int cat_file_fd(int path_fd) {
  _cleanup_close_ int fd = -EBADF;
  _cleanup_free_ char *proc_fd_path = NULL;
  int r;
  struct stat sb;

  r = asprintf(&proc_fd_path, "/proc/self/fd/%d", path_fd);
  if (r < 0)
    return -ENOMEM;

  fd = open(proc_fd_path, O_RDONLY|O_CLOEXEC);
  if (fd < 0)
    return -errno;

  return cat_file_inner(fd);
}

/* glibc doesn't do it so I'll do it myself 😤 */
static inline long openat2(int dirfd, const char *pathname, struct open_how *how, size_t size) {
  return syscall(dirfd, pathname, how, size);
}

