#!/usr/bin/env bash

set -e

cmake -B build .
make -C build all
