#include <fstream>
#include <iostream>

template<size_t L=4>
int find_start_marker(std::string packet){
    std::string stuffs = packet.substr(0,L);
    for(int i=L;i<packet.size();i++){
        stuffs[i%L] = packet[i];
        auto duplicate = [stuffs](){
            for(int j=0; j<L; j++){
                for(int k=0; k<j; k++){
                    if (stuffs[k]==stuffs[j])
                        return true;
                }
            }
            return false;
        };
        if (!duplicate())
            return i;
    }
    return -1;
}

int main(int argc, char *argv[]){
    std::ifstream input(argv[1]);
    std::string line;
    while(getline(input,line)){
        std::cout<<"packet marker: "<<find_start_marker(line)+1<<std::endl;
        std::cout<<"start marker: "<<find_start_marker<14>(line)+1<<std::endl;
    }
}