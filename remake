#!/bin/bash
for dir in Examples *; do autoreconf -vif $dir; done
autoreconf -vif
CFLAGS='' ./configure
CFLAGS='' make
