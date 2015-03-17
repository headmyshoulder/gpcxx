set -x
set -e

lcov --directory build/tests --base-directory include --capture --output-file coverage.info
lcov --remove coverage.info '/usr*' '*/cl.hpp' -o coverage.info
coveralls-lcov build/coverage.info
