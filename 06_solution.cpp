#include <fstream>
#include <iostream>

template<size_t L=4>
int find_start_marker(std::string packet){
    std::string stuffs(L,0);
    for(int i=0;i<packet.size();i++){
        bool start = true;
        stuffs[i%L] = packet[i];
        for(int j=0; j<L; j++){
            if(!stuffs[j]){
                start=false;
                break;
            }
            for(int k=0; k<j; k++){
                if (stuffs[k]==stuffs[j])
                    start = false;
            }
        }
        if(start)
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