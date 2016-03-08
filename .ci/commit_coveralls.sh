set -x
set -e

cd build
# lcov --zerocounters --directory test
lcov --gcov-tool=$GCOV --directory test --base-directory $GPCXX_ROOT/include/gpcxx --capture --output-file coverage.info
lcov --remove coverage.info '/usr*' '*/third_party*' -o coverage.info
coveralls-lcov coverage.info
