#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

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

}