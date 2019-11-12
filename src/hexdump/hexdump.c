#include "hexdump.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

char *nala_hexdump(const uint8_t *buffer, size_t size, size_t bytes_per_row)
{
    size_t dump_size;
    char *dump;
    FILE *stream = open_memstream(&dump, &dump_size);

    size_t offset = 0;

    while (offset < size) {
        fprintf(stream, "%06lX  ", offset);

        for (size_t i = 0; i < bytes_per_row; i++) {
            if (offset + i < size)             {
                fprintf(stream, "%02X ", buffer[offset + i]);
            } else {
                fprintf(stream, "-- ");
            }
        }

        fprintf(stream, " ");

        for (size_t i = 0; i < bytes_per_row; i++) {
            if (offset + i < size) {
                uint8_t byte = buffer[offset + i];
                fprintf(stream, "%c", isprint(byte) ? byte : '.');
            } else {
                fprintf(stream, " ");
            }
        }

        offset += bytes_per_row;

        if (offset < size) {
            fprintf(stream, "\n");
        }
    }

    fclose(stream);

    return dump;
}
