#include "../util.h"

#include <string.h>

int main(int argc, char *argv[]) {
  _cleanup_free_ char *cwd = NULL, *web_root = NULL;
  _cleanup_close_ int web_root_dirfd = -EBADF, file_fd = -EBADF;
  int r;

  if (argc < 2)
    die("Usage: %s <file>\n", argv[0]);

  cwd = get_current_dir_name();
  if (!cwd)
    die("Failed to get current directory: %m\n");

  if (asprintf(&web_root, "%s/web_root", cwd) < 0)
    die("Failed to get format web root: %m\n");

  web_root_dirfd = open(web_root, O_DIRECTORY|O_RDONLY);
  if (web_root_dirfd < 0)
    die("Failed to open web root directory: %m\n");

  struct open_how safe_open_how = { .flags = O_RDONLY, .resolve = RESOLVE_IN_ROOT };
  file_fd = (int) openat2(web_root_dirfd, argv[1], &safe_open_how, sizeof(struct open_how));
  if (file_fd < 0)
    die("Failed to open %s: %m %d\n", argv[1], errno);

  r = cat_file_inner(file_fd);
  if (r < 0)
    die("Failed to cat file %s: %s\n", argv[1], strerror(-r));

  return EXIT_SUCCESS;
}
