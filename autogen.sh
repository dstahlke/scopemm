#!/bin/sh

libtoolize
autoheader
aclocal
autoconf
automake --add-missing
