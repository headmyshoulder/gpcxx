set -x
set -e

sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -qq

if [ "$CXX" == "g++" ];
then
    sudo apt-get install -qq g++-${GCC_VERSION}
fi

if [ "$CXX" == "clang++" ];
then
    # export LIBCXX_REPO="http://llvm.org/svn/llvm-project/libcxx/trunk"
    export LIBCXX_REPO="http://llvm.org/svn/llvm-project/libcxx/tags/RELEASE_342/final"
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

# install valgrind
if [ "$TRAVIS_OS_NAME" = "linux" ];
then
    sudo apt-get -qq install valgrind
fi
