#ifndef NALA_HEXDUMP_H
#define NALA_HEXDUMP_H

#include <stdint.h>
#include <stdlib.h>

char *nala_hexdump(const uint8_t *buffer, size_t size, size_t bytes_per_row);
size_t nala_optimal_bytes_per_row(size_t element_size, size_t target, size_t range);

#endif
