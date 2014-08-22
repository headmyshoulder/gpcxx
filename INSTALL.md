gpcxx is a header-only library. To use it, there is no installation necessary. If you want to build the examples, the docs, or the unit test you can follow this guide.

Requirements
============
 * cmake v2.8 or newer
 * git
 * C++11 capable C++ compiler
 * boost
 * For unit tests you also need
   * gmock
   * gtest
 * To build the documentation you need
   * Doxygen
   * xsltproc
   * boostbook.dtd

Howto install under Ubuntu 13.04
--------------------------------
#optional, needed to enable unittests
export GPCXX_SRC_ROOT="$HOME/src/gpcxx"
mkdir -p "$GPCXX_SRC_ROOT"
git clone https://github.com/Ambrosys/gpcxx.git "$GPCXX_SRC_ROOT"
svn checkout http://googlemock.googlecode.com/svn/tags/release-1.7.0/ "$GPCXX_SRC_ROOT/third_party/gmock"

Build with cmake example
--------------------------------
export GPCXX_SRC_ROOT="$HOME/src/gpcxx"
mkdir -p "$GPCXX_SRC_ROOT/build" #make a build directory
cd "$GPCXX_SRC_ROOT/build" && cmake ..

Build with documentation
--------------------------------
sudo apt-get install doxygen xsltproc libboost1.55-tools-dev

#find path to boostbook.dtd
apt-file search boostbook.dtd

Test build
--------------------------------
cd "<GPCXX_SRC_ROOT>/build" 
make test
