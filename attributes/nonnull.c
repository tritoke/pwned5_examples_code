#include <stdlib.h>

__attribute__((nonnull(2)))
void does_a_thing(char *can_be_null, char *cannot_be_null) {
  *cannot_be_null = 'a';
}

int main(int argc, char *argv[]) {
  does_a_thing(NULL, NULL);

  return EXIT_SUCCESS;
}
