set -x
set -e

curl -s https://scan.coverity.com/scripts/travisci_build_coverity_scan.sh | COVERITY_SCAN_PROJECT_NAME="Ambrosys/gpcxx" COVERITY_SCAN_NOTIFICATION_EMAIL="karsten.ahnert@gmx.de" COVERITY_SCAN_BUILD_COMMAND="make" COVERITY_SCAN_BUILD_COMMAND_PREPEND="mkdir coverity_build && cd coverity_build && cmake .. -DGPCXX_BUILD_DOCS=OFF" COVERITY_SCAN_BRANCH_PATTERN=master bash

cd $GPCXX_ROOT/coverity_build
cat cov-int/scm_log.txt 
