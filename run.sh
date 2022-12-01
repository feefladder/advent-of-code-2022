alias aocfast='function __aocfast() { clear; ./$1.out $1_input.txt; unset -f __aocfast;}; __aocfast';

aocfast $1;