cd build
cmake .. -DGPCXX_BUILD_DOCS=OFF -DCMAKE_BUILD_TYPE=$BUILD_TYPE || exit $?
make || exit $?
make test || exit $?
