#ifndef NALA_HEXDUMP_H
#define NALA_HEXDUMP_H

#include <stdint.h>
#include <stdlib.h>

char *nala_hexdump(const uint8_t *buffer, size_t size, size_t bytes_per_row);

#endif
