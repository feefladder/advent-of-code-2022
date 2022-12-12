// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

char cintersection(std::string a, std::string b){
    // std::string is;
    for(auto i: a){
        if(b.find(i)!=-1)
            return i;
    }
    return -1;
}

std::string intersection(std::string a, std::string b){
    std::string is;
    for(auto i: a){
        if(b.find(i)!=-1)
            is.push_back(i);
    }
    return is;
}

int prio(std::vector<char> items){
    int p=0;
    for(auto& it: items){
        // note that 'A'=65 and 'a'=97, so check for >'Z'->lowercase;
        p += it > 'Z' ? it-'a'+1:it - 'A' + 27;
    }
    return p;
}

int main(int argc, char *argv[]){
    std::ifstream input(argv[1]);
    std::string line, first, second;
    std::vector<char> comm_items, badges;
    std::vector<std::vector<std::string>> groups;
    int i=0;
    std::vector<std::string> group;
    while(getline(input, line)){
        first = line.substr(0,line.size()/2);
        second = line.substr(line.size()/2);
        comm_items.push_back(cintersection(first, second));
        if ((++i%=3)!=0){
            group.push_back(line);
        } else {
            group.push_back(line);
            groups.push_back(group);
            group.clear();
        }
    }
    std::cout<<prio(comm_items)<<std::endl;
    for(auto& g: groups){
        badges.push_back(cintersection(g[0],intersection(g[1],g[2])));
    }
    std::cout<<prio(badges)<<std::endl;
}