// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

template<typename T>
class Point3D{
public:
  T x,y,z;

  Point3D(){}
  Point3D(Point3D const& p): x(p.x),y(p.y),z(p.z) {}
  Point3D(T nx, T ny, T nz): x(nx),y(ny),z(nz) {}

  friend std::ostream &operator<<( std::ostream &output, const Point3D &p ) { 
     output <<'('<<p.x<<','<<p.y<<','<<p.z<<')';
     return output;
  }
};

template<typename T>
class Array3D{

    //Using uint32_t for i-addressing allows for rasters of ~65535^2. These
  //dimensions fit easily within an int32_t xy-address.
  typedef int32_t  xyz_t;            ///< xy-addressing data type
  typedef uint32_t i_t;             ///< i-addressing data type

  xyz_t length=0;
  xyz_t width=0;
  xyz_t height=0; //can be changed

//   const signed char enclosed='#';
  signed char empty, enclosed=0;
  /**
  height
  z  y-length
  | /-----
  |/-----
  o----x-width
  */
  template<typename U>
  inline i_t xyz_to_i(U x, U y, U z) const {
    return (i_t)z*(i_t)length*(i_t)width+(i_t)y*(i_t)width+x;
  }

  // inline Point3D<xyz_t> i_to_xyz(i_t i) const {
  //   return Point3D<xyz_t>(i%width%length,i/width%length,i/width/length);
  // }

public:
  std::vector<T> _data;
  xyz_t x_off, y_off, z_off;

  Array3D(){}

  void print(){
    std::cout<<"printing cube of size: "<<width<<','<<length<<','<<height<<std::endl;
    for(int z=0;z<height;z++){
      std::cout<<"printing layer "<<z<<std::endl;
      for(int y=0;y<length;y++){
        for(int x=0;x<width;x++){
          std::cout<<_data[xyz_to_i(x,y,z)];
        }
        std::cout<<std::endl;
      }
    }
  }

  template<typename U>
  Array3D(std::vector<Point3D<U>> points, char nempty):empty(nempty),enclosed(nempty){
    Point3D<U> min=points[0], max=points[0];
    for (auto& p: points){
      if (p.x<min.x)
        min.x=p.x;
      if (p.y<min.y)
        min.y=p.y;
      if (p.z<min.z)
        min.z=p.z;

      if (p.x>max.x)
        max.x=p.x;
      if (p.y>max.y)
        max.y=p.y;
      if (p.z>max.z)
        max.z=p.z;
    }
    x_off=min.x;
    y_off=min.y;
    z_off=min.z;
    width=max.x-min.x+3;
    length=max.y-min.y+3;
    height=max.z-min.z+3;
    _data = std::vector<T>(width*length*height,empty);
    for(auto& p: points){
      _data[xyz_to_i(p.x-x_off+1,p.y-y_off+1,p.z-z_off+1)] = 1;
    }
  }
public:
  /**
   x
   y
   z
  */
  int calc_sides(){
    int total_sides = 0;
    for(xyz_t x=0;x<width;x++){
      for(xyz_t y=0;y<length;y++){
        for(xyz_t z=0;z<height;z++){
          if(_data[xyz_to_i(x,y,z)]==empty||_data[xyz_to_i(x,y,z)]==enclosed){
            continue;
          }
          T sides = '0';
          if(_data[xyz_to_i(x-1,y,z)]==empty)
            sides++;
          if(_data[xyz_to_i(x+1,y,z)]==empty)
            sides++;
          if(_data[xyz_to_i(x,y-1,z)]==empty)
            sides++;
          if(_data[xyz_to_i(x,y+1,z)]==empty)
            sides++;
          if(_data[xyz_to_i(x,y,z-1)]==empty)
            sides++;
          if(_data[xyz_to_i(x,y,z+1)]==empty)
            sides++;
          _data[xyz_to_i(x,y,z)]=sides;
          total_sides+= sides-'0';
        }
      }
    }
    return total_sides;
  }
  template<typename U>
  inline bool in_grid(U x, U y, U z){
    return x>=0&&x<width&&y>=0&&y<length&&z>=0&&z<height;
  }
  template<typename U>
  inline bool on_edge(U x, U y, U z){
    return x==0||x==width-1||y==0||y==length-1||z==0||z==height-1;
  }
  template<typename U>
  inline bool next_to_edge(U x, U y, U z){
    return x==1||x==width-2||y==1||y==length-2||z==1||z==height-2;
  }
  void outside(char const& outside){
    std::queue<Point3D<xyz_t>> q;
    for(xyz_t x=0;x<width;x++){
        for(xyz_t y=0;y<length;y++){
            for(xyz_t z=0;z<height;z++){
                if(on_edge(x,y,z)){
                    _data[xyz_to_i(x,y,z)]=outside;
                }
                if(next_to_edge(x,y,z)&&_data[xyz_to_i(x,y,z)]==empty){
                    q.emplace(x,y,z);
                }
            }
        }
    }

    while(!q.empty()){
        auto p = q.front();
        q.pop();
        if(_data[xyz_to_i(p.x,p.y,p.z)]==outside)
            continue;
        _data[xyz_to_i(p.x,p.y,p.z)]=outside;
        if(_data[xyz_to_i(p.x-1,p.y,p.z)]==empty){
          q.emplace(p.x-1,p.y,p.z);
          // _data[xyz_to_i(p.x-1,p.y,p.z)]=outside;
          }
        if(_data[xyz_to_i(p.x+1,p.y,p.z)]==empty){
          q.emplace(p.x+1,p.y,p.z);
          // _data[xyz_to_i(p.x+1,p.y,p.z)]=outside;
          }
        if(_data[xyz_to_i(p.x,p.y-1,p.z)]==empty){
          q.emplace(p.x,p.y-1,p.z);
          // _data[xyz_to_i(p.x,p.y-1,p.z)]=outside;
          }
        if(_data[xyz_to_i(p.x,p.y+1,p.z)]==empty){
          q.emplace(p.x,p.y+1,p.z);
          // _data[xyz_to_i(p.x,p.y+1,p.z)]=outside;
          }
        if(_data[xyz_to_i(p.x,p.y,p.z-1)]==empty){
          q.emplace(p.x,p.y,p.z-1);
          // _data[xyz_to_i(p.x,p.y,p.z-1)]=outside;
          }
        if(_data[xyz_to_i(p.x,p.y,p.z+1)]==empty){
          q.emplace(p.x,p.y,p.z+1);
          // _data[xyz_to_i(p.x,p.y,p.z+1)]=outside;
          }
    }
    empty=outside;
  }

  
};

int main(int argc, char *argv[]){
  std::ifstream input(argv[1]);
  std::string line;
  std::vector<Point3D<uint32_t>> points;
  while(std::getline(input, line)){
    int pos1 = line.find(',');
    int pos2 = line.find(',',pos1+1);
    points.emplace_back(
      std::stoi(line.substr(0,pos1)),
      std::stoi(line.substr(pos1+1,pos2-pos1-1)),
      std::stoi(line.substr(pos2+1))
    );
  }
  Array3D<char> lava=Array3D<char>(points, '~');
  std::cout<<int(lava.calc_sides())<<std::endl;
  // lava.print();
  lava.outside('.');
  
  std::cout<<int(lava.calc_sides())<<std::endl;
//   lava.print();
}