set -x
set -e

COVERITY_PATH=coverity
COMMIT_USER="Coverity Scan Builder"
COMMIT_EMAIL="travis@travis-ci.org"

git config user.name "${COMMIT_USER}"
git config user.email "${COMMIT_EMAIL}"

# Get a clean version of the repo.
rm -rf ${COVERITY_PATH}
mkdir -p ${COVERITY_PATH}
git clone -b coverity_scan "${REPO_PATH}" --single-branch ${COVERITY_PATH}


cd ${COVERITY_PATH}
git pull --no-edit origin master
git push origin coverity_scan
