SUFFIX="_input.txt" #"_sample.txt"
SOLUTION="_solution.cpp"

if [ $# == 1 ]
then
    end=$1
else
    end=$2
fi

# clear;
for i in $(seq -f "%02g" $1 $end)
do
    echo $i $i${SUFFIX};
    g++ $i${SOLUTION} -o $i.out && ./$i.out $i${SUFFIX};
done
