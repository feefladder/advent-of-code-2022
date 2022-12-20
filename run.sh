# alias aocfast='function __aocfast() { clear; ./$1.out $1_input.txt; unset -f __aocfast;}; __aocfast';

SUFFIX="_input.txt"

clear;
for i in $(seq -f "%02g" $1 $1)
do
    echo $i $i${SUFFIX};
    ./$i.out $i$SUFFIX;
done