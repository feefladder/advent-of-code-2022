// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

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
inline void move(Instruction ins, utils::Point<T>& h, utils::Point<T>& t, std::vector<utils::Point<T>>& visited){
  // start at 0,0
  //   1   U
  // -1 1 L R
  //  -1   D
  int di=0,dj=0;
  switch (ins.dir){
    case 'U':
      dj=1;//utils::Point<T>(0,1);
      break;
    case 'L':
      di=-1;//utils::Point<T>(-1,0);
      break;
    case 'R':
      di=1;//utils::Point<T>(1,0);
      break;
    case 'D':
      dj=-1;//utils::Point<T>(0,-1);
      break;
  }
  for(int i=0;i<ins.dist;i++){
    h.i+=di;
    h.j+=dj;
    follow(h, t);
    visited.emplace_back(t);
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
        std::cout<<'#';//it-snek.begin();
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
  // utils::Point<T> d;
  int di=0,dj=0;
  switch (ins.dir){
    case 'U':
      dj=1;//utils::Point<T>(0,1);
      break;
    case 'L':
      di=-1;//utils::Point<T>(-1,0);
      break;
    case 'R':
      di=1;//utils::Point<T>(1,0);
      break;
    case 'D':
      dj=-1;//utils::Point<T>(0,-1);
      break;
  }
  for(int i=0;i<ins.dist;i++){
    snek[0].i+=di;
    snek[0].j+=dj;
    for(int i=1;i<snek.size();i++){
      follow(snek[i-1], snek[i]);
    }
    visited.emplace_back(snek[snek.size()-1]);
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
  vis2.reserve(100000); visited.reserve(1000000);
  utils::Point<int> head = utils::Point(0,0), tail=utils::Point(0,0);
  for(int i=0;i<10;i++){
    snek.push_back(utils::Point<int>(0,0));
  }
  visited.push_back(tail);
  for(auto& d: dists){
    move(d, head, tail, visited);
    move_snek(d, snek, vis2);
  }
  utils::remove_duplicates(visited);
  utils::remove_duplicates(vis2);
//   print_snek(visited);
//   print_snek(vis2);
  std::cout<<visited.size()<<std::endl;
  std::cout<<vis2.size()<<std::endl;
}
