# alias aocfast='function __aocfast() { clear; ./$1.out $1_input.txt; unset -f __aocfast;}; __aocfast';

SUFFIX="_input.txt"


if [ $# == 1 ]
then
    end=$1
else
    end=$2
fi

# clear;
for i in $(seq -f "%02g" $1 $2)
do
    echo $i $i${SUFFIX};
    ./$i.out $i$SUFFIX;
done