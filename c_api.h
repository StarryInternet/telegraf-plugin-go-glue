// Copyright 2019. Starry, Inc. All Rights Reserved.
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Software written by Preston Carpenter <pcarpenter@starry.com>
#include <assert.h>
#include <stdint.h>

struct go_value {
    enum {
        TYPE_INT
    } type_;
    union {
        int int_;
    } value;
};

struct tag {
    char *key, *value;
};

struct field {
    char *key;
    struct go_value value;
};

static int get_go_value_int(struct go_value value) {
    assert(value.type_ == TYPE_INT);
    return value.value.int_;
}

// Gives a sample configuration for this plugin.
char *sample_config(void);

// Gives a description for what this plugin does.
char *description(void);

// Called by Go at a regular interval to collect data.
void gather(void);

extern void add_field(char *measurment,
        struct tag *tags, int tags_size,
        struct field *fields, int fields_size,
        int64_t unix_sec, int64_t unix_nsec);

extern void add_gauge(char *measurment,
        struct tag *tags, int tags_size,
        struct field *fields, int fields_size,
        int64_t unix_sec, int64_t unix_nsec);
