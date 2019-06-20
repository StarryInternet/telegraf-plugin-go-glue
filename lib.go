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
package main

/*
#cgo CFLAGS: -g -Wall -Wpedantic -Werror -Wno-unused-function -Wno-unused-variable
#cgo LDFLAGS: -L. -l:plugin.a -ldl
#include "c_api.h"
*/
import "C"

import (
	"github.com/influxdata/telegraf"
	"log"
	"time"
	"unsafe"
)

type RustPlugin struct{}

var Plugin telegraf.Input = &RustPlugin{}
var acc *telegraf.Accumulator = nil

func (plugin *RustPlugin) SampleConfig() string {
	return C.GoString(C.sample_config())
}

func (plugin *RustPlugin) Description() string {
	return C.GoString(C.description())
}

func (plugin *RustPlugin) Gather(acc_ telegraf.Accumulator) error {
	acc = &acc_
	C.gather()
	acc = nil
	return nil
}

type add_func func(measurement string,
	fields map[string]interface{},
	tags map[string]string,
	t ...time.Time)

func add_generic(measurement_ *C.char,
	tags_ *C.struct_tag, tags_size C.int,
	fields_ *C.struct_field, fields_size C.int,
	unix_sec, unix_nsec C.int64_t) (
	measurement string, fields map[string]interface{},
	tags map[string]string, timestamp *time.Time) {
	if acc == nil {
		log.Fatal("No accumulator was active, only call this from Gather")
	}
	measurement = C.GoString(measurement_)
	tag_list := (*[1 << 32]C.struct_tag)(unsafe.Pointer(tags_))[:tags_size]
	field_list := (*[1 << 32]C.struct_field)(unsafe.Pointer(fields_))[:fields_size]

	tags = make(map[string]string)
	for _, tag := range tag_list {
		key := C.GoString(tag.key)
		value := C.GoString(tag.value)
		tags[key] = value
	}

	fields = make(map[string]interface{})
	for _, field := range field_list {
		key := C.GoString(field.key)
		var value interface{}
		switch field.value.type_ {
		case C.TYPE_INT:
			value = int(C.get_go_value_int(field.value))
		}
		fields[key] = value
	}
	timestamp = nil
	if unix_sec != 0 && unix_nsec != 0 {
		timestamp_ := time.Unix(int64(unix_sec), int64(unix_nsec))
		timestamp = &timestamp_
	}
	return
}

//export add_field
func add_field(measurement_ *C.char,
	tags_ *C.struct_tag, tags_size C.int,
	fields_ *C.struct_field, fields_size C.int,
	unix_sec, unix_nsec C.int64_t) {
	measurement, fields, tags, t := add_generic(measurement_, tags_, tags_size, fields_, fields_size, unix_sec, unix_nsec)
	if t != nil {
		(*acc).AddFields(measurement, fields, tags, *t)
	} else {
		(*acc).AddFields(measurement, fields, tags)
	}
}

//export add_gauge
func add_gauge(measurement_ *C.char,
	tags_ *C.struct_tag, tags_size C.int,
	fields_ *C.struct_field, fields_size C.int,
	unix_sec, unix_nsec C.int64_t) {
	measurement, fields, tags, t := add_generic(measurement_, tags_, tags_size, fields_, fields_size, unix_sec, unix_nsec)
	if t != nil {
		(*acc).AddGauge(measurement, fields, tags, *t)
	} else {
		(*acc).AddGauge(measurement, fields, tags)
	}
}

//export add_counter
func add_counter(measurement_ *C.char,
	tags_ *C.struct_tag, tags_size C.int,
	fields_ *C.struct_field, fields_size C.int,
	unix_sec, unix_nsec C.int64_t) {
	measurement, fields, tags, t := add_generic(measurement_, tags_, tags_size, fields_, fields_size, unix_sec, unix_nsec)
	if t != nil {
		(*acc).AddCounter(measurement, fields, tags, *t)
	} else {
		(*acc).AddCounter(measurement, fields, tags)
	}
}
