/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the humanfriendly project.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include "hf.h"

#define TIME_UNITS_MAX 7

static void nala_hf_null_last(char *buf_p, size_t size)
{
    buf_p[size - 1] = '\0';
}

static int char_in_string(char c, const char *str_p)
{
    while (*str_p != '\0') {
        if (c == *str_p) {
            return (1);
        }

        str_p++;
    }

    return (0);
}

char *nala_hf_get_username(char *buf_p, size_t size, const char *default_p)
{
    char *res_p;
    struct passwd *passwd_p;

    res_p = buf_p;
    passwd_p = getpwuid(geteuid());

    if (passwd_p == NULL) {
        if (default_p == NULL) {
            res_p = NULL;
        } else {
            strncpy(buf_p, default_p, size);
        }
    } else {
        strncpy(buf_p, passwd_p->pw_name, size);

        if (size > 0) {
            nala_hf_null_last(buf_p, size);
        }
    }

    nala_hf_null_last(buf_p, size);

    return (res_p);
}

char *nala_hf_get_hostname(char *buf_p, size_t size, const char *default_p)
{
    int res;
    char *res_p;

    res_p = buf_p;
    res = gethostname(buf_p, size);

    if (res != 0) {
        if (default_p == NULL) {
            res_p = NULL;
        } else {
            strncpy(buf_p, default_p, size);
        }
    }

    nala_hf_null_last(buf_p, size);

    return (res_p);
}

/* Common time units, used for formatting of time spans. */
struct time_unit_t {
    unsigned long divider;
    const char *unit_p;
};

static struct time_unit_t time_units[TIME_UNITS_MAX] = {
    {
        .divider = 60 * 60 * 24 * 7 * 52 * 1000ul,
        .unit_p = "y"
    },
    {
        .divider = 60 * 60 * 24 * 7 * 1000ul,
        .unit_p = "w"
    },
    {
        .divider = 60 * 60 * 24 * 1000ul,
        .unit_p = "d"
    },
    {
        .divider = 60 * 60 * 1000ul,
        .unit_p = "h"
    },
    {
        .divider = 60 * 1000ul,
        .unit_p = "m"
    },
    {
        .divider = 1000ul,
        .unit_p = "s"
    },
    {
        .divider = 1ul,
        .unit_p = "ms"
    }
};

static const char *get_delimiter(bool is_first, bool is_last)
{
    if (is_first) {
        return ("");
    } else if (is_last) {
        return (" and ");
    } else {
        return (", ");
    }
}

char *nala_hf_format_timespan(char *buf_p,
                         size_t size,
                         unsigned long long timespan_ms)
{
    int i;
    int res;
    unsigned long long count;
    size_t offset;

    strncpy(buf_p, "", size);
    offset = 0;

    for (i = 0; i < TIME_UNITS_MAX; i++) {
        count = (timespan_ms / time_units[i].divider);
        timespan_ms -= (count * time_units[i].divider);

        if (count == 0) {
            continue;
        }

        res = snprintf(&buf_p[offset],
                       size - offset,
                       "%s%llu%s",
                       get_delimiter(strlen(buf_p) == 0, timespan_ms == 0),
                       count,
                       time_units[i].unit_p);
        nala_hf_null_last(buf_p, size);

        if (res > 0) {
            offset += (size_t)res;
        }
    }

    if (strlen(buf_p) == 0) {
        strncpy(buf_p, "0s", size);
        nala_hf_null_last(buf_p, size);
    }

    return (buf_p);
}

long nala_hf_string_to_long(const char *string_p,
                       long minimum,
                       long maximum,
                       long default_value,
                       int base)
{
    long value;
    char *end_p;

    errno = 0;
    value = strtol(string_p, &end_p, base);

    if ((errno != 0) && (value == 0)) {
        value = default_value;
    } else if (end_p == string_p) {
        value = default_value;
    } else if (*end_p != '\0') {
        value = default_value;
    }

    if (value < minimum) {
        value = minimum;
    }

    if (value > maximum) {
        value = maximum;
    }

    return (value);
}

char *nala_hf_buffer_to_string(char *dst_p,
                          size_t dst_size,
                          const void *src_p,
                          size_t src_size)
{
    if (src_size > 0) {
        if (src_size > (dst_size - 1)) {
            src_size = (dst_size - 1);
        }

        memcpy(dst_p, src_p, src_size);
    }

    dst_p[src_size] = '\0';

    return (dst_p);
}

char *nala_hf_strip(char *str_p, const char *strip_p)
{
    char *begin_p;
    size_t length;

    /* Strip whitespace characters by default. */
    if (strip_p == NULL) {
        strip_p = "\t\n\x0b\x0c\r ";
    }

    /* String leading characters. */
    while ((*str_p != '\0') && char_in_string(*str_p, strip_p)) {
        str_p++;
    }

    begin_p = str_p;

    /* Strip training characters. */
    length = strlen(str_p);
    str_p += (length - 1);

    while ((str_p >= begin_p) && char_in_string(*str_p, strip_p)) {
        *str_p = '\0';
        str_p--;
    }

    return (begin_p);
}

void *nala_hf_file_read_all(const char *path_p, size_t *size_p)
{
    FILE *file_p;
    void *buf_p;
    long file_size;

    file_p = fopen(path_p, "rb");

    if (file_p == NULL) {
        return (NULL);
    }

    if (fseek(file_p, 0, SEEK_END) != 0) {
        goto out1;
    }

    file_size = ftell(file_p);

    if (file_size == -1) {
        goto out1;
    }

    if (size_p != NULL) {
        *size_p = (size_t)file_size;
    }

    if (file_size > 0) {
        buf_p = malloc((size_t)file_size);

        if (buf_p == NULL) {
            goto out1;
        }

        if (fseek(file_p, 0, SEEK_SET) != 0) {
            goto out2;
        }

        if (fread(buf_p, (size_t)file_size, 1, file_p) != 1) {
            goto out2;
        }
    } else {
        buf_p = malloc(1);
    }

    fclose(file_p);

    return (buf_p);

 out2:
    free(buf_p);

 out1:
    fclose(file_p);

    return (NULL);
}
