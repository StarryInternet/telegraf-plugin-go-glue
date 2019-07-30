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
#include <stdbool.h>
#include <stdint.h>

struct go_value {
    enum {
        TYPE_INT,
        TYPE_UINT,
        TYPE_FLOAT,
        TYPE_BOOL,
        TYPE_STRING,
    } type_;
    union {
        int64_t int_;
        uint64_t uint_;
        double double_;
        bool bool_;
        char *string_;
    } value;
};

struct tag {
    char *key, *value;
};

struct field {
    char *key;
    struct go_value value;
};

static int64_t get_go_value_int(struct go_value go_value) {
    assert(go_value.type_ == TYPE_INT);
    return go_value.value.int_;
}

static uint64_t get_go_value_uint(struct go_value go_value) {
    assert(go_value.type_ == TYPE_UINT);
    return go_value.value.uint_;
}

static double get_go_value_double(struct go_value go_value) {
    assert(go_value.type_ == TYPE_FLOAT);
    return go_value.value.double_;
}

static bool get_go_value_bool(struct go_value go_value) {
    assert(go_value.type_ == TYPE_BOOL);
    return go_value.value.bool_;
}

static char *get_go_value_string(struct go_value go_value) {
    assert(go_value.type_ == TYPE_STRING);
    return go_value.value.string_;
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

extern void add_counter(char *measurment,
        struct tag *tags, int tags_size,
        struct field *fields, int fields_size,
        int64_t unix_sec, int64_t unix_nsec);

extern void add_summary(char *measurment,
        struct tag *tags, int tags_size,
        struct field *fields, int fields_size,
        int64_t unix_sec, int64_t unix_nsec);

extern void add_histogram(char *measurment,
        struct tag *tags, int tags_size,
        struct field *fields, int fields_size,
        int64_t unix_sec, int64_t unix_nsec);
