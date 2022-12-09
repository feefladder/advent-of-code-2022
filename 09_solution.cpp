#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "utils.hpp"

class Instruction{
public:
    char dir;
    size_t dist;

    Instruction(char ndr, int nds): dir(ndr), dist(nds){}
};

template<typename T>
inline void follow(utils::Point<T>& h, utils::Point<T>& t){
    utils::Point<T> d = h-t;
    if(d.magnitude()<=std::sqrt(2))
        return;
    t+=utils::Point<T>(utils::sgn(d.i),utils::sgn(d.j));
}

template<typename T>
void move(Instruction ins, utils::Point<T>& h, utils::Point<T>& t, std::vector<utils::Point<T>>& visited){
    // start at 0,0
    //   1   U
    // -1 1 L R
    //  -1   D
    for(int i=0;i<ins.dist;i++){
        switch (ins.dir){
            case 'U':
                h+=utils::Point<T>(0,1);
                break;
            case 'L':
                h+=utils::Point<T>(-1,0);
                break;
            case 'R':
                h+=utils::Point<T>(1,0);
                break;
            case 'D':
                h+=utils::Point<T>(0,-1);
                break;
        }
        follow(h, t);
        if(std::find(visited.begin(),visited.end(),t)==visited.end()){
            visited.push_back(t);
        }
    }
}

template<typename T>
void print_snek(std::vector<utils::Point<T>> snek){
    T maxi=snek[0].i, maxj=snek[0].j, mini=maxi, minj=maxj;
    for(auto& s: snek){
        maxi = s.i>maxi ? s.i : maxi;
        mini = s.i<mini ? s.i : mini;
        maxj = s.j>maxj ? s.j : maxj;
        minj = s.j<minj ? s.j : minj;
    }
    for(int j=minj;j<=maxj;j++){
        for(int i=mini;i<=maxi;i++){
            auto it = std::find(snek.begin(),snek.end(), utils::Point<T>(i,j));
            if(it==snek.begin()){
                std::cout<<'H';
            } else if(it!=snek.end()){
                std::cout<<it-snek.begin();
            } else {
                std::cout<<'.';
            }
        }
        std::cout<<std::endl;
    }
}

template<typename T>
void move_snek(Instruction ins, std::vector<utils::Point<T>>& snek, std::vector<utils::Point<T>>& visited){
    // start at 0,0
    //   1   U
    // -1 1 L R
    //  -1   D
    utils::Point<T> d;
    switch (ins.dir){
            case 'U':
                d=utils::Point<T>(0,1);
                break;
            case 'L':
                d=utils::Point<T>(-1,0);
                break;
            case 'R':
                d=utils::Point<T>(1,0);
                break;
            case 'D':
                d=utils::Point<T>(0,-1);
                break;
        }
    for(int i=0;i<ins.dist;i++){
        snek[0]+=d;
        for(int i=1;i<snek.size();i++){
            follow(snek[i-1], snek[i]);
        }
        visited.push_back(snek[snek.size()-1]);
    }
}

int main(int argc, char *argv[]){
    std::ifstream input(argv[1]);
    std::string line;
    std::vector<utils::Point<int>> points;
    std::vector<Instruction> dists;
    while(std::getline(input, line)){
        dists.push_back(Instruction(line[0], std::stoi(line.substr(2))));
    }

    std::vector<utils::Point<int>> visited, vis2, snek;
    utils::Point<int> head = utils::Point(0,0), tail=utils::Point(0,0);
    for(int i=0;i<10;i++){
        snek.push_back(utils::Point<int>(0,0));
    }
    visited.push_back(tail);
    for(auto& d: dists){
        move(d, head, tail, visited);
        move_snek(d, snek, vis2);
        // print_snek(snek);
    }
    utils::remove_duplicates(vis2);
    // for(auto& p: vis2){
    //     std::cout<<p<<std::endl;
    // }

    // print_snek(vis2);
    std::cout<<visited.size()<<std::endl;
    std::cout<<vis2.size()<<std::endl;
}