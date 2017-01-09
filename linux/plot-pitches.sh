#!/bin/sh

SR=62000

../cmake-build-debug/media_tests/media_decodetest -gnuplot -replotmilis 500 -type pitches -samplerate ${SR} | feedgnuplot --stream trigger --xlen 2000 --with impulses --ymin 0 --ymax 10000