#!/bin/bash
set -euo pipefail
# Array should not be empty on macOS (old Bash expands empty array to unset variable and errs)
COMMAND=("$@" ./tic-tac-toe)

echo "Run command: ${COMMAND[@]}"

FAIL=0
# Warning: use \033 instead of \e for compatibility with old Bash on macOS.

function test_ok {
    rm -f test-data/"$1".out
    INITIAL_VIEW=$(basename "$1" | awk -F- '{ print $3 }')
    {
        "${COMMAND[@]}" view-$INITIAL_VIEW <test-data/"$1".in >test-data/"$1".out &&
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
