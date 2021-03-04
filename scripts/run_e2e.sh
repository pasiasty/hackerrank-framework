#!/usr/bin/env bash

set -e

cmake -B build .
make -C build end_to_end_tests
build/end_to_end_tests
