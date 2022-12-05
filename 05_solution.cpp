#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>

void move(std::vector<std::list<char>>& stacks, int& n, int& from, int& to){
    for(int i=0; i<n; i++){
        stacks[to-1].splice(stacks[to-1].end(), stacks[from-1], --stacks[from-1].end());
    }
}

void move2(std::vector<std::list<char>>& stacks, int& n, int& from, int& to){
    std::list<char>::iterator it = stacks[from-1].end();
    std::advance(it,-n);
    stacks[to-1].splice(stacks[to-1].end(), stacks[from-1], it, stacks[from-1].end());
}

int main(int argc, char *argv[]){
    std::ifstream input(argv[1]);
    std::string line;
    std::vector<std::list<char>> stacks, stacks2;

    while(getline(input, line)){
        if(line[0] == '[' || line[0] == ' '){
            //initialize the stacks vector
            if (stacks.size()==0){
                for(int i=0; i<=line.size()/4;i++){
                    std::list<char> stack;
                    stacks.push_back(stack);
                }
            }
            // boxes are at 1+4(n-1) (or n if 0-indexed)
            for(int i=0; i<=line.size()/4;i++){
                if (line[1+4*i] != ' '){
                    stacks[i].push_front(line[1+4*i]);
                }
            }
        } else if(!line.size()){
            //copy stacks vector so we can work on both solutions
            stacks2 = std::vector<std::list<char>>(stacks);
        } else if(line[0]=='m'){
            int fpos = line.find("from");
            int tpos = line.find("to");
            int n, from, to;
            n = std::stoi(line.substr(5,fpos-5));
            from = std::stoi(line.substr(fpos+5,tpos-fpos-5));
            to = std::stoi(line.substr(tpos+3));
            move(stacks, n, from, to);
            move2(stacks2, n, from, to);
        }
    }
    std::cout<<"solution: ";
    for(auto& stack: stacks){
        std::cout<<stack.back();
    }
    std::cout<<std::endl<<"solution2: ";
    for(auto& stack: stacks2){
        std::cout<<stack.back();
    }
    std::cout<<std::endl;
}