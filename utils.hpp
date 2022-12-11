#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <cmath>

namespace utils{
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

template <typename T> inline int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

template<typename T>
class Point{
public:
  T i,j;

  Point(){}
  Point(T ni, T nj): i(ni),j(nj){}

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
  //Using uint32_t for i-addressing allows for rasters of ~65535^2. These
  //dimensions fit easily within an int32_t xy-address.
  typedef int32_t  xy_t;            ///< xy-addressing data type
  typedef uint32_t i_t;             ///< i-addressing data type

  static const i_t NO_I = std::numeric_limits<i_t>::max(); //TODO: What is this?

  xy_t width;
  xy_t height; //can be changed

  
public:
  std::vector<T> _data;
  Array2D(){}
  Array2D(xy_t nwidth) : width(nwidth){
    _data.reserve(nwidth);
  }
  Array2D(xy_t nwidth, xy_t nheight) : width(nwidth),height(nheight){
    assert(nwidth>0);
    assert(nheight>0);
    _data = std::vector<T>(nwidth*nheight,0);
  }
private:
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
public:
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

  void print(){
    std::cout<<"printing raster of size: "<<width<<','<<height<<std::endl;
    for(int j=0;j<height;j++){
      for(int i=0;i<width;i++){
        std::cout<<(_data[i+j*width]?'#':'.');
      }
      std::cout<<std::endl;
    }
  }
};

}