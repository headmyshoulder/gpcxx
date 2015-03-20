set -x
set -e

wget http://sourceforge.net/projects/boost/files/boost/1.57.0/boost_1_57_0.tar.bz2/download -O /tmp/boost.tar.bz2
wget https://googlemock.googlecode.com/files/gmock-1.7.0.zip -O /tmp/gmock.zip
tar jxf /tmp/boost.tar.bz2
unzip -q /tmp/gmock.zip
mv boost_1_57_0 $GPCXX_ROOT/third_party/boost
mv gmock-1.7.0 $GPCXX_ROOT/third_party/gmock
export BOOST_ROOT="$GPCXX_ROOT/third_party/boost"
cd $BOOST_ROOT
./bootstrap.sh
./b2 --with-thread --with-system -d0
