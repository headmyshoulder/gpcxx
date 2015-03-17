set -x
set -e

cd build
make coverage_reset
ctest 
lcov --directory test --base-directory ../include/gpcxx --capture --output-file coverage.info
lcov --remove coverage.info '/usr*' '*/third_party/*' -o coverage.info
coveralls-lcov coverage.info
