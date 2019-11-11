#include "diff.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../utils.h"

/*
 * Diff matrix initialization
 */

static void initialize_diff_matrix(NalaDiffMatrix *diff_matrix, size_t rows, size_t columns)
{
    diff_matrix->rows = rows;
    diff_matrix->columns = columns;
    diff_matrix->content = malloc(rows * columns * sizeof(int));
}

NalaDiffMatrix *nala_new_diff_matrix(size_t rows, size_t columns)
{
    NalaDiffMatrix *diff_matrix = malloc(sizeof(NalaDiffMatrix));
    initialize_diff_matrix(diff_matrix, rows, columns);

    return diff_matrix;
}

/*
 * Diff matrix operations
 */

NalaDiffMatrix *nala_new_diff_matrix_from_lengths(size_t original_length,
                                                        size_t modified_length)
{
    NalaDiffMatrix *diff_matrix =
        nala_new_diff_matrix(modified_length + 1, original_length + 1);

    for (size_t i = 0; i < diff_matrix->rows; i++)
    {
        nala_diff_matrix_set(diff_matrix, i, 0, (int)i);
    }

    for (size_t j = 0; j < diff_matrix->columns; j++)
    {
        nala_diff_matrix_set(diff_matrix, 0, j, (int)j);
    }

    return diff_matrix;
}

static void fill_different(NalaDiffMatrix *diff_matrix, size_t i, size_t j)
{
    nala_diff_matrix_set(
        diff_matrix,
        i,
        j,
        nala_min_int(nala_diff_matrix_get(diff_matrix, i - 1, j - 1),
                        nala_min_int(nala_diff_matrix_get(diff_matrix, i - 1, j),
                                        nala_diff_matrix_get(diff_matrix, i, j - 1))) +
            1);
}

static void fill_equal(NalaDiffMatrix *diff_matrix, size_t i, size_t j)
{
    nala_diff_matrix_set(diff_matrix, i, j, nala_diff_matrix_get(diff_matrix, i - 1, j - 1));
}

void nala_diff_matrix_fill_from_strings(NalaDiffMatrix *diff_matrix,
                                           const char *original,
                                           const char *modified)
{
    for (size_t i = 1; i < diff_matrix->rows; i++)
    {
        for (size_t j = 1; j < diff_matrix->columns; j++)
        {
            if (original[j - 1] == modified[i - 1])
            {
                fill_equal(diff_matrix, i, j);
            }
            else
            {
                fill_different(diff_matrix, i, j);
            }
        }
    }
}

void nala_diff_matrix_fill_from_lines(NalaDiffMatrix *diff_matrix,
                                         const char *original,
                                         const char *modified)
{
    const char *modified_pos;
    const char *modified_line = modified;

    for (size_t i = 1; i < diff_matrix->rows; i++)
    {
        modified_pos = nala_next_line(modified_line);
        size_t modified_line_length = (size_t)(modified_pos - modified_line);

        const char *original_pos;
        const char *original_line = original;

        for (size_t j = 1; j < diff_matrix->columns; j++)
        {
            original_pos = nala_next_line(original_line);
            size_t original_line_length = (size_t)(original_pos - original_line);

            if (original_line_length == modified_line_length &&
                strncmp(original_line, modified_line, original_line_length) == 0)
            {
                fill_equal(diff_matrix, i, j);
            }
            else
            {
                fill_different(diff_matrix, i, j);
            }

            original_line = original_pos + 1;
        }

        modified_line = modified_pos + 1;
    }
}

NalaDiff nala_diff_matrix_get_diff(const NalaDiffMatrix *diff_matrix)
{
    if (diff_matrix->rows == 1 && diff_matrix->columns == 1)
    {
        NalaDiff diff = { .size = 0, .chunks = NULL };
        return diff;
    }

    size_t capacity = 32;
    size_t size = 0;
    NalaDiffChunk *backtrack = malloc(capacity * sizeof(NalaDiffChunk));

    size_t i = diff_matrix->rows - 1;
    size_t j = diff_matrix->columns - 1;

    while (i > 0 || j > 0)
    {
        if (size == capacity)
        {
            capacity *= 2;
            backtrack = realloc(backtrack, capacity * sizeof(NalaDiffChunk));
        }

        NalaDiffChunk *current_chunk = &backtrack[size];
        size++;

        int current = nala_diff_matrix_get(diff_matrix, i, j);

        if (i > 0 && j > 0 && current == nala_diff_matrix_get(diff_matrix, i - 1, j - 1) + 1)
        {
            current_chunk->type = NALA_DIFF_CHUNK_TYPE_REPLACED;
            current_chunk->original_start = j - 1;
            current_chunk->original_end = j;
            current_chunk->modified_start = i - 1;
            current_chunk->modified_end = i;
            i--;
            j--;
        }
        else if (j > 0 && current == nala_diff_matrix_get(diff_matrix, i, j - 1) + 1)
        {
            current_chunk->type = NALA_DIFF_CHUNK_TYPE_DELETED;
            current_chunk->original_start = j - 1;
            current_chunk->original_end = j;
            current_chunk->modified_start = i;
            current_chunk->modified_end = i;
            j--;
        }
        else if (i > 0 && current == nala_diff_matrix_get(diff_matrix, i - 1, j) + 1)
        {
            current_chunk->type = NALA_DIFF_CHUNK_TYPE_ADDED;
            current_chunk->original_start = j;
            current_chunk->original_end = j;
            current_chunk->modified_start = i - 1;
            current_chunk->modified_end = i;
            i--;
        }
        else if (i > 0 && j > 0 && current == nala_diff_matrix_get(diff_matrix, i - 1, j - 1))
        {
            current_chunk->type = NALA_DIFF_CHUNK_TYPE_MATCHED;
            current_chunk->original_start = j - 1;
            current_chunk->original_end = j;
            current_chunk->modified_start = i - 1;
            current_chunk->modified_end = i;
            i--;
            j--;
        }
    }

    NalaDiff diff = { size, malloc(size * sizeof(NalaDiffChunk)) };

    ssize_t backtrack_index = (ssize_t)size - 1;
    size_t chunk_index = 0;

    diff.chunks[chunk_index] = backtrack[backtrack_index];

    for (backtrack_index--; backtrack_index >= 0; backtrack_index--)
    {
        NalaDiffChunk *chunk = &backtrack[backtrack_index];
        NalaDiffChunk *previous_chunk = &diff.chunks[chunk_index];

        if (chunk->type == previous_chunk->type)
        {
            previous_chunk->original_end = chunk->original_end;
            previous_chunk->modified_end = chunk->modified_end;
        }
        else if ((chunk->type == NALA_DIFF_CHUNK_TYPE_REPLACED &&
                  previous_chunk->type != NALA_DIFF_CHUNK_TYPE_MATCHED) ||
                 (chunk->type != NALA_DIFF_CHUNK_TYPE_MATCHED &&
                  previous_chunk->type == NALA_DIFF_CHUNK_TYPE_REPLACED))
        {
            previous_chunk->type = NALA_DIFF_CHUNK_TYPE_REPLACED;
            previous_chunk->original_end = chunk->original_end;
            previous_chunk->modified_end = chunk->modified_end;
        }
        else
        {
            chunk_index++;
            diff.chunks[chunk_index] = *chunk;
        }
    }

    free(backtrack);

    diff.size = chunk_index + 1;
    diff.chunks = realloc(diff.chunks, diff.size * sizeof(NalaDiffChunk));

    return diff;
}

size_t nala_diff_matrix_index(const NalaDiffMatrix *diff_matrix, size_t row, size_t column)
{
    return row * diff_matrix->columns + column;
}

int nala_diff_matrix_get(const NalaDiffMatrix *diff_matrix, size_t row, size_t column)
{
    return diff_matrix->content[nala_diff_matrix_index(diff_matrix, row, column)];
}

void nala_diff_matrix_set(const NalaDiffMatrix *diff_matrix,
                             size_t row,
                             size_t column,
                             int value)
{
    diff_matrix->content[nala_diff_matrix_index(diff_matrix, row, column)] = value;
}

/*
 * Higher-level wrappers
 */

NalaDiff nala_diff_strings_lengths(const char *original,
                                         size_t original_length,
                                         const char *modified,
                                         size_t modified_length)
{
    NalaDiffMatrix *diff_matrix =
        nala_new_diff_matrix_from_lengths(original_length, modified_length);

    nala_diff_matrix_fill_from_strings(diff_matrix, original, modified);

    NalaDiff diff = nala_diff_matrix_get_diff(diff_matrix);

    nala_free_diff_matrix(diff_matrix);

    return diff;
}

NalaDiff nala_diff_strings(const char *original, const char *modified)
{
    return nala_diff_strings_lengths(original, strlen(original), modified, strlen(modified));
}

NalaDiff nala_diff_lines(const char *original, const char *modified)
{
    size_t original_length = nala_count_chars(original, '\n') + 1;
    size_t modified_length = nala_count_chars(modified, '\n') + 1;

    NalaDiffMatrix *diff_matrix =
        nala_new_diff_matrix_from_lengths(original_length, modified_length);

    nala_diff_matrix_fill_from_lines(diff_matrix, original, modified);

    NalaDiff diff = nala_diff_matrix_get_diff(diff_matrix);

    nala_free_diff_matrix(diff_matrix);

    return diff;
}

/*
 * Cleanup
 */

void nala_free_diff_matrix(NalaDiffMatrix *diff_matrix)
{
    free(diff_matrix->content);
    free(diff_matrix);
}
