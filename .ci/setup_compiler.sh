set -x
set -e

sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -qq

if [ "$CXX" == "g++" ];
then
    sudo apt-get install -qq g++-${GCC_VERSION}

    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 90
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 90
    sudo update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-4.9 90
fi

if [ "$CXX" == "clang++" ];
then
    export LIBCXX_REPO="http://llvm.org/svn/llvm-project/libcxx/trunk"
    # export LIBCXX_REPO="http://llvm.org/svn/llvm-project/libcxx/tags/RELEASE_33/final"
    export LIBCXXABI_REPO="http://llvm.org/svn/llvm-project/libcxxabi/trunk"
    # export LIBCXXABI_REPO="http://llvm.org/svn/llvm-project/libcxxabi/tags/RELEASE_351/final"

    # Install libc++
    svn co --quiet $LIBCXX_REPO libcxx
    cd libcxx/lib && bash buildit
    sudo cp ./libc++.so.1.0 /usr/lib/
    sudo mkdir /usr/include/c++/v1
    cd .. && sudo cp -r include/* /usr/include/c++/v1/
    cd /usr/lib && sudo ln -sf libc++.so.1.0 libc++.so
    sudo ln -sf libc++.so.1.0 libc++.so.1 && cd $cwd
  
    # Install libc++abi
    svn co --quiet $LIBCXXABI_REPO libcxxabi
    cd libcxxabi/lib && bash buildit
    sudo cp ./libc++abi.so.1.0 /usr/lib/
    cd .. && sudo cp -r include/* /usr/include/c++/v1/
    cd /usr/lib && sudo ln -sf libc++abi.so.1.0 libc++abi.so
    sudo ln -sf libc++abi.so.1.0 libc++abi.so.1 && cd $cwd
fi

if [ -n "$MASTER_BUILD" ];
then
    # gcc 4.9 does not work with lcov 1.10, we need to install lcov 1.11
    wget -O lcov.tar.gz http://downloads.sourceforge.net/ltp/lcov-1.11.tar.gz
    mkdir lcov
    tar xzf lcov.tar.gz -C ./lcov --strip-components=1
    cd lcov
    sudo make install
    cd ..
    rm -Rf lcov lcov.tar.gz
    # sudo apt-get install -qq lcov

    sudo apt-get install -qq python-yaml
    gem install coveralls-lcov
fi


# install valgrind
if [ "$TRAVIS_OS_NAME" = "linux" ];
then
    sudo apt-get -qq install valgrind
fi
