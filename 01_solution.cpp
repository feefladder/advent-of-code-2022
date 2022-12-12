// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char *argv[]){
    std::ifstream input(argv[1]);
    std::string line;
    std::vector<int> elves;
    int current_elf =0;

    while(getline(input, line)){
        if(line==""){
            elves.push_back(current_elf);
            current_elf = 0;
        } else {
            current_elf += std::stoi(line);
        }
    }
    elves.push_back(current_elf);
    std::sort(elves.begin(), elves.end());


    int three_sum = 0;
    for (int i=elves.size()-3;i<elves.size();i++){
        three_sum += elves[i];
    }
    std::cout <<"best elf: "<<elves[elves.size()-1]<<" sum of three: "<<three_sum<<std::endl;
}