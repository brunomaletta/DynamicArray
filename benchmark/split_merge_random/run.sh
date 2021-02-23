for ((i = 7; i <= 12; i++)) do
	echo =========== 10^$i ===========
	> input

	# maximum value stored in the sets
	let val=1
	for ((k = 1; k <= $i; k++)) do
		let val=10*val
	done
	echo $val >> input

	# number of operations
	echo 1000000 >> input

	for file in sms sms_mo treap splay
	do
		> output
		# average across 10 runs
		for ((j = 1; j <= 10; j++)) do
			mem=$( (/usr/bin/time -f "%M" ./$file $j < input >> output) 2>&1)
			echo "$(($mem/1024))" >> output
		done
		./average 10 < output
		echo ""
	done
done
make clean
