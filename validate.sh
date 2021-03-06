for i in {1..2000}; do
    python gen.py -n 40 -d 2 -k 30 -l 0 100 > tests/curr_test
    dc=$(./C2/efftrav < tests/curr_test | cut -d ',' -f 1 | cut -c 2-)
    bf=$(pypy python/bruteforce.py < tests/curr_test | cut -d ',' -f 1 | cut -c 2-)
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
