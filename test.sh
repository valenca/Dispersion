for i in {1..2000}; do
    python gen.py -n 20 -d 2 -k 2 -l 0 10 > tests/curr_test
    bf=$(pypy python/bruteforce.py < tests/curr_test | cut -d ',' -f 1 | cut -c 2-)
    dc=$(./C/efftrav < tests/curr_test | cut -d ',' -f 1 | cut -c 2-)
    cat tests/curr_test > tests/last_test
    if [ "$bf" == "$dc" ]; then
	echo "Test $i: True"
    else
	echo "Test $i: False"
	echo "$bf"
	echo "$dc"
	cat tests/curr_test > tests/test$i
    fi
done
