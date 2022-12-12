# alias aocgo='function __aocgo() {  unset -f __aocgo;}; __aocgo';

SUFFIX="_input.txt"
SOLUTION="_solution.cpp"

clear;
for i in $(seq -f "%02g" 1 $1)
do
    echo $i $i${SUFFIX};
    g++ $i${SOLUTION} -o $i.out && ./$i.out $i${SUFFIX};
done