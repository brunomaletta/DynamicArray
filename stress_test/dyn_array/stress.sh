for ((i = 1; ; i++)) do
	./$3 $i > in
	./$1 < in > out_a.txt
	./$2 < in > out_b.txt
	if (! cmp -s out_a.txt out_b.txt) then
		echo "--> input:"
		cat in
		echo "--> output a:"
		cat out_a.txt
		echo "--> output b:"
		cat out_b.txt
		break;
	fi
	echo $i
done
