#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "utils.hpp"

size_t path(utils::Array2D<char> const& grid, std::vector<utils::Point<int32_t>>& starts, utils::Point<int32_t>& end){
  size_t min=-1;

  /*
  321
  4 0
  567
  */
  const int dx[] = {1, 1, 0,-1,-1,-1, 0, 1};
  const int dy[] = {0, 1, 1, 1, 0,-1,-1,-1};
  /*
   1
  2 0
   3
  */
  const int d4x[] = {1,0,-1,0};
  const int d4y[] = {0,1,0,-1};
  std::queue<utils::Point<int32_t>> q;
  for(auto& start: starts){
    utils::Array2D<int> out = utils::Array2D<int>(grid.get_width(), grid.get_height());
    q.push(start);
    while(!q.empty()){
      utils::Point p = q.front();
      q.pop();

      for(int i=0;i<4;i++){
        int nx = p.i+d4x[i],ny=p.j+d4y[i];
        if(nx==start.i&&ny==start.j)
          continue;
        if(grid.in_grid(nx,ny)){
          if(grid(nx,ny)-1>grid(p.i,p.j))
            continue; //skip points that are too high
          if(grid(nx,ny)+out(p.i,p.j)<out(nx,ny)||out(nx,ny)==0){
            out(nx,ny)=out(p.i,p.j)+1;
            q.emplace(nx,ny);
          }
        }
      }
    }
    if(min>out(end.i,end.j)&&out(end.i,end.j)!=0)
      min=out(end.i,end.j);
  }
  std::cout<<"best path: "<<min<<std::endl;
  return min;
}



int main(int argc, char *argv[]){
  std::ifstream input(argv[1]);
  std::string line;
  utils::Array2D<char> grid;
  while(std::getline(input, line)){
    if(!grid.get_width()){
      grid=utils::Array2D<char>(line.size());
    }
    // grid._data.push_back(line);
    grid.add_row(line.begin(), line.end());
  }

  std::vector<utils::Point<int32_t>> start, ass;
  utils::Point<int32_t> end;
  for(int i=0;i<grid.get_width();i++){
    for(int j=0;j<grid.get_height();j++){
      if(grid(i,j)=='S'){
        start.emplace_back(i,j);
        grid(i,j)='~';
      }
      if(grid(i,j)=='E'){
        end = utils::Point<int32_t>(i,j);
        grid(i,j)='z'+1;//'{'
      }
      if(grid(i,j)=='a')
        ass.emplace_back(i,j);
    }
  }

  std::cout<<"S: "<<int('S')<<" a: "<<int('a')<<std::endl;
  // grid.print();
  path(grid, start, end);
  path(grid, ass, end);
}