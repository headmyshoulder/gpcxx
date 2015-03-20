set -x
set -e

cd $BOOST_ROOT/tools/quickbook
../../b2 dist-bin
export $QUICKBOOK_ROOT=$BOOST_ROOT/dist/bin
export $BOOSTBOOK_ROOT=$BOOST_ROOT/tools/boostbook
export $DOCBOOK_ROOT=$GPCXX_ROOT/third_party/doc_book

mkdir $DOCBOOK_ROOT
cd $DOCBOOK_ROOT

mkdir docbook-xml
cd docbook-xml
wget http://www.oasis-open.org/docbook/xml/4.2/docbook-xml-4.2.zip
unzip docbook-xml-4.2.zip
rm docbook-xml-4.2.zip
cd ..

wget http://sourceforge.net/projects/docbook/files/docbook-xsl/1.78.1/docbook-xsl-1.78.1.tar.bz2/download -O docbook-xsl-1.78.1.tar.bz2
tar xvfj docbook-xsl-1.78.1.tar.bz2
ln -s docbook-xsl-1.78.1 docbook-xsl
rm docbook-xsl-1.78.1.tar.bz2

cd $GPCXX_ROOT
cd build
cmake .. -DGPCXX_BUILD_DOCS
make documentation

