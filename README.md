# Telegraf Go Glue

This repo has Go glue that enables other languages to be added as plugins to
telegraf. It does this by exposing the functions that are normally exposed using
the Go plugin feature as C functions that can be statically linked to.

The Go code can be fetched using `go get`, but it is suggested you use a
submodule instead. You'll need a submodule in order to get the latest header
file that will be used to link to the code.
