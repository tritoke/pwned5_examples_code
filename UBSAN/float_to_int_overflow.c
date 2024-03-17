#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  double y = 31425e69;
  int x = (int) y;

  printf("%f\n", y);
  printf("%d\n", x);
  
  return EXIT_SUCCESS;
}
