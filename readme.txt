Build steps linux:

mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
cmake --build . --target run_whistle_tests -- -j 8
./whistle_tests/run_whistle_tests


