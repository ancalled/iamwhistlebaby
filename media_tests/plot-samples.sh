#!/bin/sh

../cmake-build-debug/media_tests/media_decodetest -gnuplot -replotmilis 100 -type samples | feedgnuplot --stream trigger --xlen 5000 --lines --ymin -5000 --ymax 5000