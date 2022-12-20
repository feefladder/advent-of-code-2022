// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "utils.hpp"

template<typename T>
std::vector<const T*> make_vec(std::vector<T>const & v){
  std::vector<const T*> l;
  for(int64_t i=0;i<v.size();i++){
    l.push_back(&v[i]);
  }
  return l;
}

// template<typename T, typename U, typename V>

// template<typename T>
void move(std::vector<const int64_t*>& l, int64_t from, int64_t to){
  if(from<to){
    /*
      f   t
    1,2,3,4,5,6
    temp f
         <-
    1,3,4,4,5,6
          f
    1,3,4,2,5,6
    */
    
    const int64_t* temp = l[from];
    // for(auto& i:l){
    //   std::cout<<*i<<';';
    // }
    // std::cout<<*temp<<std::endl;
    for(int64_t i=from;i<to;i++){
      l[i]=l[i+1];
    }
    l[to]=temp;
  } else if(from>to){
    /*
      t   f
    1,2,3,4,5,6
    temp f
      ->
    1,2,2,3,5,6
    1,4,2,3,5,6
    t     f
    1,2,3,4
    ->
    1,1,2,3
    4,1,2,3
    */
    
    const int64_t* temp = l[from];
    // for(auto& i:l){
    //   std::cout<<*i<<';';
    // }
    // std::cout<<*temp<<std::endl;
    for(int64_t i=from;i>to;i--){
      l[i]=l[i-1];
    }
    l[to]=temp;
  }
}

inline void coords(std::vector<int64_t> const& vec, std::vector<const int64_t*> const& l){
  const int64_t* a0 = &*std::find(vec.begin(), vec.end(), 0);
  int64_t index = std::find(l.begin(),l.end(), a0) - l.begin();
  int64_t coords = 0;
  for(int64_t i=1;i<=3;i++){
    int64_t j=int64_t(i*1000+index)%l.size();
    // std::cout<<*l[j]<<std::endl;
    coords += *l[j];
  }
  std::cout<<coords<<std::endl;
}

int main(int argc, char *argv[]){
  std::ifstream input(argv[1]);
  std::string line;
  std::vector<int64_t> vec;
  while(std::getline(input, line)){
    vec.push_back(std::stoi(line));
  }
  std::vector<const int64_t*> l = make_vec(vec);
  for(int i=0;i<vec.size();i++){
    if(vec[i]==0)
      continue;
    int64_t index = std::find(l.begin(),l.end(),&vec[i]) - l.begin();
    int64_t new_index = utils::mod(int64_t(index+vec[i]),int64_t(l.size()-1));
    move(l, index, new_index);
  }
  coords(vec, l);
  const int64_t key = 811589153;
  for(auto& i: vec){
    i*= key;
  }
  l = make_vec(vec);
  for(int _i=0;_i<10;_i++){
    for(int i=0;i<vec.size();i++){
      if(vec[i]==0)
        continue;
      int64_t index = std::find(l.begin(),l.end(),&vec[i]) - l.begin();
      int64_t new_index = utils::mod(int64_t(index+vec[i]),int64_t(l.size()-1));
      move(l, index, new_index);
    }
  }
  coords(vec, l);
}