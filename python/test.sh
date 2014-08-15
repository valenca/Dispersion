for i in {1..2000}; do
	python gen.py -n 20 -d 2 -k 2 -l 0 10 > test
	bf=$(pypy bruteforce.py < test | cut -d ',' -f 1 | cut -c 2-)
	dc=$(pypy efftrav.py < test | cut -d ',' -f 1 | cut -c 2-)
	cat test > last_test
	if [ "$bf" == "$dc" ]; then
		echo "Test $i: True"
	else
		echo "Test $i: False"
		echo "$bf"
		echo "$dc"
		cat test > tests/test$i
	fi
done
