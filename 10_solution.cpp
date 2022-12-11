#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "utils.hpp"

class Processor{
public:
  int c=0, sstrength=1, out=0;
  utils::Array2D<bool> crt= utils::Array2D<bool>(40,6);
  Processor(){}

  void cycle(){
    // std::cout<<"cycle: "<<c<<" X: "<<sstrength<<"CRT pos: "<<(c%40)<<std::endl;
    if(std::abs(sstrength-(c%40))<=1){
      // std::cout<<"draw cycle: "<<c<<" X: "<<sstrength<<std::endl;
      crt._data[c] = true;
    }
    c++;
    if(c%40==20){
      // std::cout<<"plus: "<<c<<','<<sstrength<<'='<<c*sstrength<<std::endl;
      out+= c*sstrength;
    }
    return;
  }

  void noop(){
    cycle();
  }

  void addx(int x){
    cycle();
    sstrength+= x;
    cycle();
  }

  void print(){
    crt.print();
  }
};

int main(int argc, char *argv[]){
    std::ifstream input(argv[1]);
    std::string line;
    Processor P;
    P.cycle();
    while(std::getline(input, line)){
        if(line[0]=='n'){
            P.noop();
        } else {
            P.addx(std::stoi(line.substr(5)));
        }
    }
    std::cout<<P.out<<std::endl;
    P.print();
}