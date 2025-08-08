#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

my_command="./my_grep"
sys_command="grep"

flags=(
    "i"
    "v"
    "c"
    "l"
    "n"
    "h"
    "s"
    "o"
)

reg=(
    "grep"
    "file"
    "HELLO"
)

regFiles=(
    "test_files\/reg1.txt"
    "test_files\/reg2.txt"
    "test_files\/reg3.txt"
)

tests=(
    "FLAGS REG test_files/test_1_grep.txt"
    "FLAGS REG test_files/test_2_grep.txt"
    "FLAGS REG test_files/test_3_grep.txt"
    "FLAGS REG test_files/test_1_grep.txt test_files/test_2_grep.txt"
    "FLAGS REG test_files/test_1_grep.txt test_files/test_2_grep.txt test_files/test_3_grep.txt"
)

run_test() {
    param=$(echo "$2" | sed "s/FLAGS/$1/" | sed "s/REG/$3/")
    "${my_command[@]}" $param > "${my_command[@]}".log
    "${sys_command[@]}" $param > "${sys_command[@]}".log
    DIFF="$(diff -s "${my_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    if [ "$DIFF" == "Files "${my_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        let "SUCCESS++"
        echo "$COUNTER - \033[32mSuccess $param"
    else
        let "FAIL++"
        echo "$COUNTER - \033[31mFail $param"
    fi
    rm -f "${my_command[@]}".log "${sys_command[@]}".log
}

echo "=============================================="
echo "1 PARAMETER"
echo "=============================================="
printf "\n"

for flag in "${flags[@]}"
do
    for test in "${tests[@]}"
    do
        for reg in "${reg[@]}"
        do
            run_test "-$flag" "$test" "$reg"
        done
    done
done
printf "\n"

echo "=============================================="
echo "2 PARAMETERS"
echo "=============================================="
printf "\n"

for flag1 in "${flags[@]}"
do
    for flag2 in "${flags[@]}"
    do
        if [ $flag1 != $flag2 ]
        then
            for test in "${tests[@]}"
            do
                for reg in "${reg[@]}"
                do
                    run_test "-$flag1$flag2" "$test" "$reg"
                done
            done
        fi
    done
done
printf "\n"

echo "====================================================================="
echo "2 -e flag"
echo "====================================================================="
printf "\n"

for reg1 in "${reg[@]}"
do
    for reg2 in "${reg[@]}"
    do
        if [ $reg1 != $reg2 ]
        then
            for test in "${tests[@]}"
            do
                run_test "-e $reg1 -e $reg2" "$test" ""
            done
        fi
    done
done
printf "\n"

echo "=============================================="
echo "-e flag with another flag"
echo "=============================================="
printf "\n"

for reg in "${reg[@]}"
do
    for flag in "${flags[@]}"
    do
        for test in "${tests[@]}"
        do
            run_test "-e $reg -$flag" "$test" ""
        done
    done
done
printf "\n"

echo "=============================================="
echo "2 -f flag"
echo "=============================================="
printf "\n"

for regFiles1 in "${regFiles[@]}"
do
    for regFiles2 in "${regFiles[@]}"
    do
        if [ $regFiles1 != $regFiles2 ]
        then
            for test in "${tests[@]}"
            do
                run_test "-f $regFiles1 -f $regFiles2" "$test" ""
            done
        fi
    done
done
printf "\n"

echo "=============================================="
echo "-f flag with another flag"
echo "=============================================="
printf "\n"

for regFiles in "${regFiles[@]}"
do
    for flag in "${flags[@]}"
    do
        for test in "${tests[@]}"
        do
            run_test "-f $regFiles -$flag" "$test" ""
        done
    done
done
printf "\n"

echo "=============================================="
echo "-e and -f flags"
echo "=============================================="
printf "\n"

for regFiles in "${regFiles[@]}"
do
    for reg in "${reg[@]}"
    do
        for test in "${tests[@]}"
        do
            run_test "-f $regFiles -e $reg" "$test" ""
        done
    done
done
printf "\n"

echo "\033[31mFAILED: $FAIL"
echo "\033[32mSUCCESSFUL: $SUCCESS"
echo "\033[34mALL: $COUNTER"
printf "\n"
