#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
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

std::vector<int> explode(std::string str, char delimiter)
{
    std::vector<int> output;
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

}