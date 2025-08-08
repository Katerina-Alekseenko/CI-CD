#!/bin/bash

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

SUCCESS=0
FAIL=0
COUNTER=0

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
    "test_files/reg1.txt"
    "test_files/reg2.txt"
    "test_files/reg3.txt"
)

tests=(
    "FLAGS REG test_files/test_1_grep.txt"
    "FLAGS REG test_files/test_2_grep.txt"
    "FLAGS REG test_files/test_3_grep.txt"
    "FLAGS REG test_files/test_1_grep.txt test_files/test_2_grep.txt"
    "FLAGS REG test_files/test_1_grep.txt test_files/test_2_grep.txt test_files/test_3_grep.txt"
)

cleanup() {
    rm -f my_grep.log grep.log
}

print_result() {
    local test_num=$1
    local status=$2
    local message=$3
    
    if [ "$status" == "success" ]; then
        echo -e "${GREEN}$test_num - Success: $message${NC}"
    else
        echo -e "${RED}$test_num - Fail: $message${NC}"
    fi
}

run_test() {
    local flags=$1
    local test_case=$2
    local pattern=$3
    
    # Подставляем флаги и шаблон
    local command=$(echo "$test_case" | sed "s/FLAGS/$flags/" | sed "s/REG/$pattern/")
    
    # Выполняем команды
    $my_command $command > my_grep.log 2>&1
    $sys_command $command > grep.log 2>&1
    
    ((COUNTER++))
    
    if diff -q my_grep.log grep.log >/dev/null; then
        ((SUCCESS++))
        print_result "$COUNTER" "success" "$command"
    else
        ((FAIL++))
        print_result "$COUNTER" "fail" "$command"
        echo -e "${YELLOW}Diff output (first 10 lines):${NC}"
        diff -u my_grep.log grep.log | head -n 10
        echo ""
    fi
    
    cleanup
}

echo -e "${YELLOW}==============================================${NC}"
echo -e "${YELLOW}1 PARAMETER${NC}"
echo -e "${YELLOW}==============================================${NC}"
echo ""

for flag in "${flags[@]}"; do
    for test in "${tests[@]}"; do
        for pattern in "${reg[@]}"; do
            run_test "-$flag" "$test" "$pattern"
        done
    done
done

echo -e "${YELLOW}==============================================${NC}"
echo -e "${YELLOW}2 PARAMETERS${NC}"
echo -e "${YELLOW}==============================================${NC}"
echo ""

for flag1 in "${flags[@]}"; do
    for flag2 in "${flags[@]}"; do
        if [ "$flag1" != "$flag2" ]; then
            for test in "${tests[@]}"; do
                for pattern in "${reg[@]}"; do
                    run_test "-$flag1$flag2" "$test" "$pattern"
                done
            done
        fi
    done
done

echo -e "${YELLOW}=====================================================================${NC}"
echo -e "${YELLOW}2 -e flag${NC}"
echo -e "${YELLOW}=====================================================================${NC}"
echo ""

for reg1 in "${reg[@]}"; do
    for reg2 in "${reg[@]}"; do
        if [ "$reg1" != "$reg2" ]; then
            for test in "${tests[@]}"; do
                run_test "-e $reg1 -e $reg2" "$test" ""
            done
        fi
    done
done

echo -e "${YELLOW}==============================================${NC}"
echo -e "${YELLOW}-e flag with another flag${NC}"
echo -e "${YELLOW}==============================================${NC}"
echo ""

for pattern in "${reg[@]}"; do
    for flag in "${flags[@]}"; do
        for test in "${tests[@]}"; do
            run_test "-e $pattern -$flag" "$test" ""
        done
    done
done

echo -e "${YELLOW}==============================================${NC}"
echo -e "${YELLOW}2 -f flag${NC}"
echo -e "${YELLOW}==============================================${NC}"
echo ""

for regFiles1 in "${regFiles[@]}"; do
    for regFiles2 in "${regFiles[@]}"; do
        if [ "$regFiles1" != "$regFiles2" ]; then
            for test in "${tests[@]}"; do
                run_test "-f $regFiles1 -f $regFiles2" "$test" ""
            done
        fi
    done
done

echo -e "${YELLOW}==============================================${NC}"
echo -e "${YELLOW}-f flag with another flag${NC}"
echo -e "${YELLOW}==============================================${NC}"
echo ""

for regFile in "${regFiles[@]}"; do
    for flag in "${flags[@]}"; do
        for test in "${tests[@]}"; do
            run_test "-f $regFile -$flag" "$test" ""
        done
    done
done

echo -e "${YELLOW}==============================================${NC}"
echo -e "${YELLOW}-e and -f flags${NC}"
echo -e "${YELLOW}==============================================${NC}"
echo ""

for regFile in "${regFiles[@]}"; do
    for pattern in "${reg[@]}"; do
        for test in "${tests[@]}"; do
            run_test "-f $regFile -e $pattern" "$test" ""
        done
    done
done

echo -e "${YELLOW}==============================================${NC}"
echo -e "${RED}FAILED: $FAIL${NC}"
echo -e "${GREEN}SUCCESSFUL: $SUCCESS${NC}"
echo -e "${YELLOW}ALL: $COUNTER${NC}"
echo -e "${YELLOW}==============================================${NC}"

cleanup
exit $FAIL