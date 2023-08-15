#!/bin/bash
set -euo pipefail
[ "$#" -ge 1 ] || (echo "Expected at least one argument: command to run" && exit 1)
COMMAND=("$@")

FAIL=0
# Warning: use \033 instead of \e for compatibility with old Bash on macOS.

function test_ok {
    rm -f test-data/"$1".out
    {
        "${COMMAND[@]}" <test-data/"$1".in | head -c 1000000 >test-data/"$1".out &&
        diff test-data/"$1".{sol,out} &&
        echo PASS
    } || { echo -e "\033[31;1mFAIL\033[0m" && FAIL=1; }
}

for tn in $(cat tests.txt); do
    tn=$(echo $tn)  # To remove \r from tests.txt on Windows
    echo ===== $tn =====
    test_ok "$tn"
done
if [[ "$FAIL" == "0" ]]; then
    echo -e "===== \033[32;1mALL PASS\033[0m ====="
else
    echo -e "===== \033[31;1mSOME FAIL\033[0m ====="
fi
exit $FAIL
