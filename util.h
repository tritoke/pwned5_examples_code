#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
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
