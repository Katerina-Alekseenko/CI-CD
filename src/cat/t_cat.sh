#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
TEST_FILE="bytes.txt"
echo "" > log.txt

# Проверка существования файлов
[ ! -f "$TEST_FILE" ] && echo "Test file not found!" && exit 1
[ ! -f "./my_cat" ] && echo "my_cat executable not found!" && exit 1

run_test() {
    local test_cmd="$1"
    echo "$test_cmd"
    
    ./my_cat $test_cmd > my_cat.txt 2>/dev/null
    cat $test_cmd > cat.txt 2>/dev/null
    
    if diff -q my_cat.txt cat.txt >/dev/null 2>&1; then
        ((COUNTER_SUCCESS++))
    else
        echo "$test_cmd" >> log.txt
        ((COUNTER_FAIL++))
        echo "======= DIFF =======" >> log.txt
        diff -u my_cat.txt cat.txt >> log.txt
        echo "===================" >> log.txt
    fi
    
    rm -f my_cat.txt cat.txt
}

# Тестируем одиночные флаги
flags=(-b -e -n -s -t -v)
for var in "${flags[@]}"; do
    run_test "$var $TEST_FILE"
done

# Тестируем парные комбинации
for var1 in "${flags[@]}"; do
    for var2 in "${flags[@]}"; do
        if [ "$var1" != "$var2" ]; then
            run_test "$var1 $var2 $TEST_FILE"
        fi
    done
done

# Тестируем тройные комбинации
for var1 in "${flags[@]}"; do
    for var2 in "${flags[@]}"; do
        for var3 in "${flags[@]}"; do
            if [ "$var1" != "$var2" ] && [ "$var2" != "$var3" ] && [ "$var1" != "$var3" ]; then
                run_test "$var1 $var2 $var3 $TEST_FILE"
            fi
        done
    done
done

# Тестируем четверные комбинации
for var1 in "${flags[@]}"; do
    for var2 in "${flags[@]}"; do
        for var3 in "${flags[@]}"; do
            for var4 in "${flags[@]}"; do
                if [ "$var1" != "$var2" ] && [ "$var2" != "$var3" ] && [ "$var1" != "$var3" ] &&
                   [ "$var1" != "$var4" ] && [ "$var2" != "$var4" ] && [ "$var3" != "$var4" ]; then
                    run_test "$var1 $var2 $var3 $var4 $TEST_FILE"
                fi
            done
        done
    done
done

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"

[ "$COUNTER_FAIL" -eq 0 ] && exit 0 || exit 1