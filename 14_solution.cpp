// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "utils.hpp"

// /*
//  o
//  ##
// |o <- 1
//  ##
// |oo
//  ##
//  o
// |oo
//  ##
// |o <- 3
// |oo
//  ##
// |o
// |ooo
//  ###
// |oo
// |ooo
//  ###
//  o
// |oo
// |ooo
//  ###
// |o  <-6
// |oo
// |ooo
//  ###<-10<-15<-...
// */
// class Pyramid(){
//   // this class simulates a possibly infinite pyramid.
//   int height, grains;
//   Point<int32_t> pour\

//   /*
//     o     <- 1
//   ######
//    oo
//   ######
//    oo|    <- 2
//   ######
//     o
//    oo|
//   #######
//     o
//   ooo|
//   #######
//    oo
//   ooo|
//   #######
//    oo|     <- 5 (start pouring right, aka)
//   ooo|
//   #######
//    oo|o
//   ooo|oo
//   #######
//     o     <- 3 (1+3+5=9th grain)
//    ooo
//   ooooo
//  ooooooo
// ######### <- 4 (1+3+5+7=16th grain)
//   */
// };

template<typename T>
class Cave: public utils::Array2D<T>{
public:
  uint32_t left, right;
  utils::Point<int32_t> pp;

  template<typename U>
  Cave(std::vector<std::vector<utils::Point<U>>> const& lines){
    
    U maxi=500, maxj=0, mini=maxi, minj=maxj;
    for(auto& l: lines){
        for(auto& p: l){
        maxi = p.i>maxi ? p.i : maxi;
        mini = p.i<mini ? p.i : mini;
        maxj = p.j>maxj ? p.j : maxj;
        minj = p.j<minj ? p.j : minj;
        }
    }
    
    this->x_off = mini;
    this->y_off = minj;
    this->width = maxi-mini+1;
    this->height = maxj-minj+3;
    if(this->width < this->height*2){
        this->x_off = 500-this->height;
        this->width = this->height*2;
    }
    this->_data = std::vector<T>(this->width*this->height,0);
    for(auto& l:lines){
      for(int i=1;i<l.size();i++){
        utils::Point<U> start=l[i-1], next=l[i];
        if((start-next).i==0){
          // vertical line
          if(start.j>next.j)
            std::swap(start, next);
          for(U j=start.j;j<=next.j;j++){
            this->_data[this->xyToI(start.i-this->x_off,j-this->y_off)]='#';
          }
        } else {
          // horizontal line
          if(start.i>next.i)
            std::swap(start, next);
          for(U i=start.i; i<=next.i;i++){
            this->_data[this->xyToI(i-this->x_off,start.j-this->y_off)]='#';
          }
        }
      }
    }
    for(int i=0;i<this->width;i++){
      this->_data[this->xyToI(i,this->height-1)]='-';//add bottom
    }
    pp = utils::Point<int32_t>(500-this->x_off,0);
    // pp = utils::Point<int32_t>(500-mini,0);
    this->_data[this->pointToI(pp)]='+';
  }

  utils::Point<int32_t> fall(utils::Point<int32_t> start);
  int pour();
};

template<typename T>
utils::Point<int32_t> Cave<T>::fall(utils::Point<int32_t> start){
  while(!this->_data[this->pointToI(start)]){start.j++;}

  /*
   ?
  /|\
  213
  */
  std::vector<int32_t> dx = {0, -1, 1};
  utils::Point<int32_t> check = utils::Point<int32_t>(start.i,start.j+1);
  while(this->in_grid(check.i, check.j)){
    int32_t dx_def=2;
    for(auto& x : {0, -1, 1}){
      if(this->in_grid(check.i+x,check.j)){
        if(this->_data[this->xyToI(check.i+x,check.j)]){
          continue;
        } else {
          dx_def=x;
          break;
        }
      } else {
        check.i +=x; //outside grid
        return check;
      }
    }
    if(dx_def != 2){
      check.i += dx_def;
      check.j += 1;
    } else {
      check.j -=1;
      return check; //sticky
    }
  }
  return check;
}

template<typename T>
int Cave<T>::pour(){
    bool completed1=false;
  std::cout<<fall(pp)<<std::endl;
  // grid(fall(grid,start))='o';
  int n=0;
  while(true){
    utils::Point<int32_t> end = fall(pp);
    if(this->in_grid(end.i,end.j)&&end.j>0){
        if(end.j==this->height-2 && !completed1){
          std::cout<<"first grain fell on the bottom after "<<n<<std::endl;
          completed1=true;
        }
        this->_data[this->pointToI(end)]='o';
        n++;
    } else {
        if(end.i<0){
        std::cout<<end<<" fell off grid to the left!\n";
        } else if (end.i>this->width){
        std::cout<<end<<" fell off grid to the right!\n";
        } else {
          std::cout<<"the pouring hole filled up!\n";
        }
        // this->print();
        return n;
    }
  }
  return 1;
}

int main(int argc, char *argv[]){
  std::ifstream input(argv[1]);
  std::string line;
  std::vector<std::vector<utils::Point<int>>> lines;

  while(std::getline(input, line)){
    std::vector<utils::Point<int>> l;
    for(auto s: utils::explode(line," -> ")){
      l.emplace_back(utils::split(s,','));
    }
    lines.push_back(l);
  }

  Cave<char> grid = Cave<char>(lines);
//   grid.print();
  // std::cout<<"yay!!!!11!!\n";
  // grid.print();
  std::cout<<grid.pour()<<std::endl;
  // grid.print();
}