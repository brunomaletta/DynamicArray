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

	for file in sms set treap splay
	do
		> output
		# average across 10 runs
		for ((j = 1; j <= 10; j++)) do
			./$file $j < input >> output
		done
		./average < output
	done
done
make clean
