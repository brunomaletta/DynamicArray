for ((i = 1; i <= 5; i++)) do
	echo =========== 10^$i ===========
	> input

	# number of arays
	let val=1
	for ((k = 1; k <= $i; k++)) do
		let val=10*val
	done
	echo $val >> input

	# number of operations
	let val=1000000
	echo $val >> input

	for file in dyn_array
	do
		> output
		# average across 5 runs
		for ((j = 1; j <= 10; j++)) do
			let seed=10*$i+$j
			mem=$( (/usr/bin/time -f "%M" ./$file $seed < input >> output) 2>&1)
			echo "$(($mem/1024))" >> output
		done
		./average 5 < output
		echo ""
	done
done
make clean
