set -x
set -e

if  [ ${COVERITY_SCAN_BRANCH} == 1 ]; 
then
    exit 0;
fi

cd build
cmake .. -DGPCXX_BUILD_DOCS=OFF -DCMAKE_BUILD_TYPE=$BUILD_TYPE
make
ctest -V

if [ "$TRAVIS_OS_NAME" != "linux" ];
then
  exit 0;
fi

VALGRIND_CMD="valgrind --leak-check=full --show-reachable=yes --error-exitcode=1 "
$VALGRIND_CMD test/util/util_tests
$VALGRIND_CMD test/tree/tree_tests
$VALGRIND_CMD test/io/io_tests
$VALGRIND_CMD test/operator/operator_tests
$VALGRIND_CMD test/generate/generate_tests
$VALGRIND_CMD test/stat/stat_tests
$VALGRIND_CMD test/eval/eval_tests