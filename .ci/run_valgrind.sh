if [ "$TRAVIS_OS_NAME" != "linux" ];
then
  exit 0;
fi

VALGRIND_CMD="valgrind --leak-check=full --show-reachable=yes "

cd build
$VALGRIND_CMD test/util/util_tests || exit $?
$VALGRIND_CMD test/tree/tree_tests || exit $?
$VALGRIND_CMD test/io/io_tests || exit $?
$VALGRIND_CMD test/operator/operator_tests || exit $?
$VALGRIND_CMD test/generate/generate_tests || exit $?
$VALGRIND_CMD test/stat/stat_tests || exit $?
$VALGRIND_CMD test/eval/eval_tests || exit $?
