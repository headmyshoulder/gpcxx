Requirements
============
 * cmake v2.8 or newer
 * git
 * c++ compiler c++11 capable
   * gmock/gtest (optional, needed to enable unittests)
   * boost
 * Doxgen (optional, only needed to generate documentation)
 * xsltproc (optional, only needed to generate documentation)

Howto install under Ubuntu 13.04
--------------------------------
#optional, needed to enable unittests
export GPCXX_SRC_ROOT="$HOME/src/gpcxx"
sudo apt-get install subversion git
mkdir -p "$GPCXX_SRC_ROOT"
git clone https://github.com/Ambrosys/gpcxx.git "$GPCXX_SRC_ROOT"
svn checkout http://googlemock.googlecode.com/svn/tags/release-1.7.0/ "$GPCXX_SRC_ROOT/third_party/gmock"

Build with cmake exampel
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


