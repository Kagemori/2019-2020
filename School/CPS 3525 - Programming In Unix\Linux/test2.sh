#!/usr/bin/sh

if $#argv == 0 then
	echo Usage: $0 num1 [num2 num3]
	exit 1
endif

@ sum = 0
foreach number($argv)
	@ sum += $number
end

echo The sum is : $sum

@ average = $sum / $#argv
@ remainder = $sum % $#argv
echo The average is: $averagge\($remainder\)
