// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "utils.hpp"

typedef std::pair<int,int> irange;

bool fully_contains(irange a, irange b){
    bool a_c_b, b_c_a;
    b_c_a = (a.first <= b.first && a.second >= b.second);
    a_c_b = (b.first <= a.first && b.second >= a.second);
    return b_c_a || a_c_b;
}

std::vector<int> intersection(irange a, irange b){
    std::vector<int> is;
    for(int i=a.first;i<=a.second;i++){
        if(i>=b.first && i<=b.second)
            is.push_back(i);
    }
    return is;
}

int main(int argc, char *argv[]){
    std::ifstream input(argv[1]);
    std::string line;
    unsigned long int fcs=0,pcs=0;
    while(getline(input, line)){
        int pos = line.find(',');
        irange a = utils::split(line.substr(0,pos),'-');
        irange b = utils::split(line.substr(pos+1),'-');
        fcs += fully_contains(a,b);
        pcs += intersection(a,b).size() > 0;
    }
    std::cout<<"there are: "<<fcs<<" fully contained thingies and "<<pcs<<" partially contained ones\n";
}