# alias aocfast='function __aocfast() { clear; ./$1.out $1_input.txt; unset -f __aocfast;}; __aocfast';

if [ $# == 1 ]
then
    end=$1
else
    end=$2
fi

SUFFIX="_sample.txt"

# clear;
for i in $(seq -f "%02g" $1 $end)
do
    echo $i $i${SUFFIX};
    valgrind --leak-check=yes ./$i.out $i${SUFFIX};
done