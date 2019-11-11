#ifndef NALA_DIFF_H
#define NALA_DIFF_H

#include <stdlib.h>

#include "types.h"

struct NalaDiffMatrix
{
    size_t rows;
    size_t columns;
    int *content;
};

enum NalaDiffChunkType
{
    NALA_DIFF_CHUNK_TYPE_MATCHED,
    NALA_DIFF_CHUNK_TYPE_ADDED,
    NALA_DIFF_CHUNK_TYPE_REPLACED,
    NALA_DIFF_CHUNK_TYPE_DELETED
};

struct NalaDiff
{
    size_t size;
    NalaDiffChunk *chunks;
};

struct NalaDiffChunk
{
    NalaDiffChunkType type;
    size_t original_start;
    size_t original_end;
    size_t modified_start;
    size_t modified_end;
};

NalaDiffMatrix *nala_new_diff_matrix(size_t rows, size_t columns);
NalaDiffMatrix *nala_new_diff_matrix_from_lengths(size_t original_length,
                                                        size_t modified_lengths);
void nala_diff_matrix_fill_from_strings(NalaDiffMatrix *diff_matrix,
                                           const char *original,
                                           const char *modified);
void nala_diff_matrix_fill_from_lines(NalaDiffMatrix *diff_matrix,
                                         const char *original,
                                         const char *modified);
NalaDiff nala_diff_matrix_get_diff(const NalaDiffMatrix *diff_matrix);

size_t nala_diff_matrix_index(const NalaDiffMatrix *diff_matrix, size_t row, size_t column);
int nala_diff_matrix_get(const NalaDiffMatrix *diff_matrix, size_t row, size_t column);
void nala_diff_matrix_set(const NalaDiffMatrix *diff_matrix,
                             size_t row,
                             size_t column,
                             int value);

NalaDiff nala_diff_strings_lengths(const char *original,
                                         size_t original_length,
                                         const char *modified,
                                         size_t modified_length);
NalaDiff nala_diff_strings(const char *original, const char *modified);
NalaDiff nala_diff_lines(const char *original, const char *modified);

void nala_free_diff_matrix(NalaDiffMatrix *diff_matrix);

#endif
