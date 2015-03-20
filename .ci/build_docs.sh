set -x
set -e

cd $BOOST_ROOT/tools/quickbook
../../b2 dist-bin -d0
export QUICKBOOK_ROOT="$BOOST_ROOT/dist/bin"
export BOOSTBOOK_ROOT="$BOOST_ROOT/tools/boostbook"
export DOCBOOK_ROOT="$GPCXX_ROOT/third_party/docbook"

sudo apt-get install -qq doxygen
sudo apt-get install -qq xsltproc

mkdir $DOCBOOK_ROOT
cd $DOCBOOK_ROOT

mkdir docbook-dtd
cd docbook-dtd
wget http://www.oasis-open.org/docbook/xml/4.2/docbook-xml-4.2.zip
unzip -q docbook-xml-4.2.zip
rm docbook-xml-4.2.zip
cd ..

wget http://sourceforge.net/projects/docbook/files/docbook-xsl/1.78.1/docbook-xsl-1.78.1.tar.bz2/download -O docbook-xsl-1.78.1.tar.bz2
tar xfj docbook-xsl-1.78.1.tar.bz2
ln -s docbook-xsl-1.78.1 docbook-xsl
rm docbook-xsl-1.78.1.tar.bz2

cd $GPCXX_ROOT
cd build
ls $QUICKBOOK_ROOT
ls $BOOSTBOOK_ROOT
ls $DOCBOOK_ROOT
cmake .. -DGPCXX_BUILD_DOCS=1
make documentation


GH_PAGES_PATH="gh-pages"
COMMIT_USER="Documentation Builder"
COMMIT_EMAIL="travis@travis-ci.org"


# Get a clean version of the repo.
rm -rf ${GH_PAGES_PATH}
mkdir -p ${GH_PAGES_PATH}
git clone -b gh-pages "${REPO_PATH}" --single-branch ${GH_PAGES_PATH}


cd ${GH_PAGES_PATH}/doc
rm -rf *
cp -rf ../../doc/html/* .
git add -A
git config user.name "${COMMIT_USER}"
git config user.email "${COMMIT_EMAIL}"
git commit -m "adding current documentation version"
git push origin gh-pages

