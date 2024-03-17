#include <stdlib.h>
#include <stdio.h>

int use_after_free(void) {
  int *x = malloc(10 * sizeof(int));
  free(x);
  return x[4]; /* BAD BAD BAD */
}

int main(int argc, char *argv[]) {
  printf("%d\n", use_after_free());
  
  return EXIT_SUCCESS;
}
