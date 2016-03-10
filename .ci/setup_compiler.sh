set -x
set -e

if [ "$LIBCXX" == "on" ]; then

    cd $THIRD_PARTY_ROOT

    LLVM_URL="https://github.com/llvm-mirror/llvm/archive/master.tar.gz"
    LIBCXX_URL="https://github.com/llvm-mirror/libcxx/archive/master.tar.gz"
    LIBCXXABI_URL="https://github.com/llvm-mirror/libcxxabi/archive/master.tar.gz"
    TAR_FLAGS="-xz"

    mkdir -p llvm llvm/build llvm/projects/libcxx llvm/projects/libcxxabi
    wget --quiet -O - ${LLVM_URL} | tar --strip-components=1 ${TAR_FLAGS} -C llvm
    wget --quiet -O - ${LIBCXX_URL} | tar --strip-components=1 ${TAR_FLAGS} -C llvm/projects/libcxx
    wget --quiet -O - ${LIBCXXABI_URL} | tar --strip-components=1 ${TAR_FLAGS} -C llvm/projects/libcxxabi
    (cd llvm/build && cmake .. -DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_ROOT}/llvm/install -DCMAKE_CXX_COMPILER=${CXX} )
    (cd llvm/build/projects/libcxx && make install -j2)
    (cd llvm/build/projects/libcxxabi && make install -j2)

fi


if [ "$COVERALLS_BUILD" == "on" ];
then

    # gcc 4.9 does not work with lcov 1.10, we need to install lcov 1.11
    cd $THIRD_PARTY_ROOT
    curl http://ftp.uk.debian.org/debian/pool/main/l/lcov/lcov_1.11.orig.tar.gz -O
    tar xfz lcov_1.11.orig.tar.gz
    mkdir -p lcov && make -C lcov-1.11/ install PREFIX=$THIRD_PARTY_ROOT/lcov
    export PATH="${THIRD_PARTY_ROOT}/lcov/usr/bin:${PATH}"
    rm -Rf lcov-1.11/ lcov_1.11.orig.tar.gz
    
    gem install coveralls-lcov

    lcov --version

fi