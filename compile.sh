# alias aocgo='function __aocgo() {  unset -f __aocgo;}; __aocgo';

clear; g++ $1_solution.cpp -o $1.out && ./$1.out $1_sample.txt;