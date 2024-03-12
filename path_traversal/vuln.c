#include "../util.h"

#include <string.h>

int main(int argc, char *argv[]) {
  _cleanup_free_ char *cwd = NULL, *web_root = NULL, *file_path = NULL;
  int r;

  if (argc < 2)
    die("Usage: %s <file>\n", argv[0]);

  cwd = get_current_dir_name();
  if (!cwd)
    die("Failed to get current directory: %m\n");

  if (asprintf(&web_root, "%s/web_root", cwd) < 0)
    die("Failed to get format web root: %m\n");

  if (asprintf(&file_path, "%s/%s", web_root, argv[1]) < 0)
    die("Failed to get format file path: %m\n");

  r = cat_file(file_path);
  if (r < 0)
    die("Failed to cat file %s: %s\n", file_path, strerror(-r));

  return EXIT_SUCCESS;
}
