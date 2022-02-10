#!/bin/sh

SOURCES_DIRS="source"
FILES_TO_CHECK="$(find "$SOURCES_DIRS" | grep -E ".*(\.ino|\.cpp|\.c|\.h|\.hpp|\.hh)$")"

clang-format -i $FILES_TO_CHECK
