for i in {1..200}; do
	python gen.py -n 20 -d 2 -k 4 -l 0 10 > test
	bf=$(pypy bruteforce.py < test | cut -d ',' -f 1 | cut -c 2-)
	dc=$(pypy try4.py < test | cut -d ',' -f 1 | cut -c 2-)
	cat test > last_test
	if [ "$bf" == "$dc" ]; then
		echo "Test $i: True"
	else
		echo "Test $i: False"
		echo "$bf"
		echo "$dc"
	fi
done
