#include <stdio.h>
#include <stdint.h>

#include "read.h"
#include "compare.h"

int main (int argc, char *argv[]) {
  if (argc != 3) {
    printf("Invalid number of arguments\n");
    return 1;
  }

  int32_t *a = NULL;
  int32_t *b = NULL;
  size_t a_len, b_len;

  read_fp(&a, &a_len, argv[1]);
  read_fp(&b, &b_len, argv[2]);

  if (a != NULL && b != NULL) {
    float score = match_fingerprints(a, a_len, b, b_len, 80);
    printf("%f\n", score);
  } else {
    printf("Unable to read input files\n");
    return 1;
  }

  return 0;
}
