set -x
set -e

curl -s https://scan.coverity.com/scripts/travisci_build_coverity_scan.sh | COVERITY_SCAN_PROJECT_NAME="Ambrosys/gpcxx" COVERITY_SCAN_NOTIFICATION_EMAIL="karsten.ahnert@gmx.de" COVERITY_SCAN_BUILD_COMMAND="make" COVERITY_SCAN_BUILD_COMMAND_PREPEND="mkdir coverity_build && cd coverity_build && cmake .. -DGPCXX_BUILD_DOCS=OFF" COVERITY_SCAN_BRANCH_PATTERN=master bash

cd $GPCXX_ROOT/coverity_build
cat cov-int/scm_log.txt 



# curl -s https://scan.coverity.com/scripts/travisci_build_coverity_scan.sh | COVERITY_SCAN_PROJECT_NAME="$TRAVIS_REPO_SLUG" COVERITY_SCAN_NOTIFICATION_EMAIL="karsten.ahnert@gmx.de" COVERITY_SCAN_BUILD_COMMAND="make" COVERITY_SCAN_BUILD_COMMAND_PREPEND="cd build && cmake .. -DGPCXX_BUILD_DOCS=OFF" COVERITY_SCAN_BRANCH_PATTERN=coverity_scan bash



# old stuff
# COVERITY_PATH=coverity
# COMMIT_USER="Coverity Scan Builder"
# COMMIT_EMAIL="travis@travis-ci.org"

# Get a clean version of the repo.
# rm -rf ${COVERITY_PATH}
# mkdir -p ${COVERITY_PATH}
# git clone -b coverity_scan "${REPO_PATH}" --single-branch ${COVERITY_PATH}


# cd ${COVERITY_PATH}
# git config user.name "${COMMIT_USER}"
# git config user.email "${COMMIT_EMAIL}"

# git pull --no-edit origin master
# git push origin coverity_scan
