#!/bin/bash
set -euo pipefail
if [ "$#" -ge 1 ]; then
    COMMAND=("$@")
else
    COMMAND=("./mytest_main")
fi

if ! [ "${COMPILATION_COMMAND+x}" ]; then
    COMPILATION_COMMAND=$(./detect-compiler.sh)
fi
if [[ $COMPILATION_COMMAND =~ "^cl " ]]; then
    COMPILATION_COMMAND+=" /I../../"
else
    COMPILATION_COMMAND+=" -I../../"
fi

echo "Compilation command: ${COMPILATION_COMMAND[@]}"
echo "Run command: ${COMMAND[@]}"

FAIL=0
# Warning: use \033 instead of \e for compatibility with old Bash on macOS.

function test_ok {
    pushd test-data/$tn &>/dev/null
    rm *.out &>/dev/null || true
    SOURCES="*.cpp mytest.cpp"
    if ! [ -f main.cpp ]; then
        SOURCES+=" mytest_main.cpp"
    fi
    SOURCES=$(echo $SOURCES | tr ' ' '\n' | sort | sed 's~^mytest~../../mytest~g')

    {
        # Prevent msys on Windows from converting Visual Studio's flags like /W4 to file paths
        # Redirect stderr to stdout so logs are not interleaved
        MSYS_NO_PATHCONV=1 MSYS2_ARG_CONV_EXCL="*" ${COMPILATION_COMMAND[@]} $SOURCES 2>&1 &&
        { "${COMMAND[@]}" >stdout.out 2>stderr.out; echo $? >exit.out; } &&
        diff /dev/null stdout.out &&
        diff stderr.{sol,out} &&
        diff -w exit.{sol,out} &&
        echo PASS
    } || { echo -e "\033[31;1mFAIL\033[0m" && FAIL=1; }
    popd &>/dev/null
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
