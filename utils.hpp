// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

#ifndef _utils_hpp_
#define _utils_hpp_

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <cmath>

namespace utils{

template<typename T>
inline T triangle_nr(T const& n){
  return n*(n+1)/2;
}

template<typename T, typename U>
inline const unsigned int mod(T a, U const b){
    // https://stackoverflow.com/a/23214219/14681457
    int r = a%b;
    return (r < 0) ? (r+b)%b : r;
}

std::string intersection(std::string a, std::string b){
  std::string is;
  for(auto i: a){
    if(b.find(i)!=-1)
      is.push_back(i);
  }
  return is;
}

std::pair<int,int> split(std::string str, char delimiter){
  int pos = str.find(delimiter);
  return std::pair<int,int>(std::stoi(str.substr(0,pos)),std::stoi(str.substr(pos+1)));
}

template<typename T>
std::vector<T> explode(std::string str, char delimiter)
{
  std::vector<T> output;
  std::stringstream ss(str);
  std::string token;

  while (getline(ss, token, delimiter)) {
    if (token.size() > 0) {
      output.push_back(std::stoi(token));
    }
  }
  return output;
}

template<typename T>
std::vector<T> explode(std::string str, std::string delimiter)
{
  std::vector<T> output;
  std::string token;
  int pos=0,pos2=0;
  //   0       1       2
  // blabla->blabla->blabla
  // pos   pos2
  //       pos     pos2
  //               pos     pos2
  pos2 = str.find(delimiter, pos);
  do {
    if(pos==0 && pos2!=0){
      output.push_back(std::stoi(str.substr(0, pos2)));
    } else if(pos2-pos){
      output.push_back(std::stoi(str.substr(pos+delimiter.size(), pos2-pos-delimiter.size())));
    }
    pos=pos2;
    pos2=str.find(delimiter,pos2+1);
    std::cout<<"output: "<<output.back()<<"pos: "<<pos<<','<<pos2<<std::endl;
  } while(pos != -1);
  return output;
}


std::vector<std::string> explode(std::string str, std::string delimiter)
{
  std::vector<std::string> output;
  std::string token;
  int pos=0,pos2=0;
  //   0       1       2
  // blabla->blabla->blabla
  // pos   pos2
  //       pos     pos2
  //               pos     pos2
  pos2 = str.find(delimiter, pos);
  do {
    if(pos==0 && pos2!=0){
      output.push_back(str.substr(0, pos2));
    } else if(pos2-pos){
      output.push_back(str.substr(pos+delimiter.size(), pos2-pos-delimiter.size()));
    }
    pos=pos2;
    pos2=str.find(delimiter,pos2+1);
  } while(pos != -1);
  return output;
}

template <typename T> inline int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

template<typename T>
class Point{
public:
  T i,j;

  Point(){}
  Point(Point const& p){i=p.i;j=p.j;}
  Point(T ni, T nj): i(ni),j(nj){}
  Point(std::pair<T,T> p): i(p.first),j(p.second){}

  bool operator==(Point const&p){
    return p.i==i&&p.j==j;
  }

  Point operator+(Point const& p){
    return Point(p.i+i,p.j+j);
  }

  Point<T> operator+=(Point const& p){
    i+=p.i;
    j+=p.j;
    return *this;
  }

  Point operator-(Point const& p){
    return Point(i-p.i,j-p.j);
  }

  bool operator<(Point const& p){
    if (i==p.i){
      return j<p.j;
    } else {
      return i<p.i;
    }
  }

  T magnitude(){
    return std::sqrt(i*i+j*j);
  }

  friend std::ostream &operator<<( std::ostream &output, const Point &p ) { 
     output << p.i<<","<<p.j;
     return output;
  }
};

template<typename T> void remove_duplicates(std::vector<T>& v){
  //remove duplicates
  std::sort(v.begin(), v.end());
  auto last = std::unique(v.begin(), v.end());
  v.erase(last, v.end());
}

template<typename T>
class Array2D{
protected:
  //Using uint32_t for i-addressing allows for rasters of ~65535^2. These
  //dimensions fit easily within an int32_t xy-address.
  typedef int32_t  xy_t;            ///< xy-addressing data type
  typedef uint32_t i_t;             ///< i-addressing data type

  static const i_t NO_I = std::numeric_limits<i_t>::max(); //TODO: What is this?

  xy_t width=0;
  xy_t height=0; //can be changed
  std::vector<T> _data;
  xy_t x_off,y_off;
  
public:


  Array2D(){}
  Array2D(xy_t nwidth) : width(nwidth){
    _data.reserve(nwidth);
  }
  Array2D(xy_t nwidth, xy_t nheight) : width(nwidth),height(nheight){
    assert(nwidth>0);
    assert(nheight>0);
    _data = std::vector<T>(nwidth*nheight,0);
  }

  void add_row(std::vector<T>& row){
    assert(row.size()==width);
    _data.insert(_data.end(), row.begin(), row.end());
    height++;
  }

  template<class R>
  void add_row(R begin, R end){
    assert(end-begin==width);
    _data.insert(_data.end(), begin, end);
    height++;
  }

  xy_t get_width() const {return width;}
  xy_t get_height() const {return height;}
protected:
  /**
    @brief Convert from x,y coordinates to index coordinates

    @param[in]  x   X-coordinate to convert
    @param[in]  y   Y-coordinate to convert

    @return Returns the index coordinate i of (x,y)
  */
  inline i_t xyToI(xy_t x, xy_t y) const {
    assert(0<=x && x<width && 0<=y && y<height);
    return (i_t)y*(i_t)width+(i_t)x;
  }

  template<typename U>
  inline i_t pointToI(Point<U> p) const {
    return xyToI(p.i,p.j);
  }
public:
  template<typename U>
  inline bool in_grid(U i, U j) const {
    return i>=0&&i<width&&j>=0&&j<height;
  }

  /**
    @brief Return cell value based on i-coordinate

    @param[in]   i    i-coordinate of cell whose data should be fetched.

    @return The value of the cell identified by 'i'
  */
  T& operator()(i_t i){
    assert(i<(i_t)width*height);
    return &_data[i];
  }
  T operator()(i_t i) const {
    assert(i<(i_t)width*height);
    return _data[i];
  }

  /**
    @brief Return cell value based on x,y coordinates

    @param[in]   x    X-coordinate of cell whose data should be fetched.
    @param[in]   y    Y-coordinate of cell whose data should be fetched.

    @return The value of the cell identified by x,y
  */
  T& operator()(xy_t x, xy_t y){
    assert(x>=0);
    assert(y>=0);
    assert(x<width);
    assert(y<height);
    return _data[xyToI(x,y)];
  }
  T operator()(xy_t x, xy_t y) const {
    assert(x>=0);
    assert(y>=0);
    assert(x<width);
    assert(y<height);
    return _data[xyToI(x,y)];
  }


  T& operator()(Point<xy_t> p) {
    assert(p.i>=0);
    assert(p.j>=0);
    assert(p.i<width);
    assert(p.j<height);
    return _data[xyToI(p.i,p.j)];
  }

  T operator()(Point<xy_t> p) const {
    assert(p.i>=0);
    assert(p.j>=0);
    assert(p.i<width);
    assert(p.j<height);
    return _data[xyToI(p.i,p.j)];
  }

  void print(){
    std::cout<<"printing raster of size: "<<width<<','<<height<<std::endl;
    for(int j=0;j<height;j++){
      for(int i=0;i<width;i++){
        std::cout<<(_data[xyToI(i,j)]?_data[xyToI(i,j)]:'.');
      }
      std::cout<<std::endl;
    }
  }
};

} //namespace utils

#endif //_utils_hpp_
