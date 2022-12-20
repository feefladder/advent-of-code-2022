// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

class int_list{
public:
  int n=-1;
  std::vector<int_list> list;

  bool operator<(int_list const& l){
    if(n!=-1&&l.n!=-1)
      return n<l.n;
    if(n==-1&&l.n!=-1) //take first element of list
      return list[0]<l.n;
    if(n!=-1&&l.n==-1)
      return n<l.list[0];
    return list[0]<l.list[0];
  }

  bool operator<(int const& i){
    if(n!=-1)
      return n<i;
    return list[0]<i;
  }

  void print(){
    if(list.size()){
      std::cout<<'[';
      for(auto& l:list){
        l.print();
      }
      std::cout<<']';
    } else {
      std::cout<<n<<',';
    }
  }
}

template<typename T>
class Packet{
public:
  int_list left,right;

}

int main(int argc, char *argv[]){
  std::ifstream input(argv[1]);
  std::string line;
  while(std::getline(input, line)){
  
  }
}