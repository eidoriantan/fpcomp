#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "read.h"

/**
 * Reads Acoustid raw fingerprint from binary file
 */
void read_fp (int32_t **a, size_t *length, const char *filename) {
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    char* error;
    sprintf(error, "Unable to open file \"%s\"\n", filename);
    perror(error);
    return;
  }

  *a = malloc(sizeof(int32_t) * 2048);
  *length = 0;
  while (fread(*a + *length, sizeof(int32_t), 1, fp) == 1) (*length)++;

  fclose(fp);
}
