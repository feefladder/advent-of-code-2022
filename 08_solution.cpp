// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

#include <fstream>
#include <iostream>
#include <vector>

bool check_tree(int i, int j, std::vector<std::vector<uint8_t>> const& forest){
    if (i==0||j==0||i==forest.size()-1||j==forest[i].size()-1)
        return true;
    
    auto h=forest[i][j];
    bool top=true,bot=true,left=true,right=true;
    for(int k=0; k<i; k++){
        if(forest[k][j]>=h)
            top=false;
    }
    for(int k=forest.size()-1; k>i; k--){
        if(forest[k][j]>=h)
            bot=false;
    }
    for(int k=0;k<j;k++){
        if(forest[i][k]>=h)
            left=false;
    }
    for(int k=forest[i].size()-1; k>j; k--){
        if(forest[i][k]>=h)
            right = false;
    }
    return (top||bot||left||right);
}

size_t find_trees(std::vector<std::vector<uint8_t>> const & forest){
    size_t trees=0;
    for(int i=0;i<forest.size();i++){
        for(int j=0;j<forest[i].size();j++){
            if(check_tree(i,j,forest)){
                trees++;
            }
        }
    }
    return trees;
}

int score(int i, int j, std::vector<std::vector<uint8_t>> const & forest){
    int score=1, h=forest[i][j];
    // if (i==0||j==0||i==forest.size()-1||j==forest[i].size()-1)
    //     return 0;
    for(int k=i+1;; k++){
        if(forest[k][j]>=h||k==forest.size()-1){
            score *=k-i;
            break;
        }
    }
    for(int k=i-1;; k--){
        if(forest[k][j]>=h||k==0){
            score *=i-k;
            break;
        }
    }
    for(int k=j+1;;k++){
        if(forest[i][k]>=h||k==forest[i].size()-1){
            score *=k-j;
            break;
        }
    }
    for(int k=j-1;; k--){
        if(forest[i][k]>=h||k==0){
            score *=j-k;
            break;
        }
    }
    return score;
}

size_t best(std::vector<std::vector<uint8_t>> const & forest){
    size_t best =0;
    for(int i=1;i<forest.size()-1;i++){
        for(int j=1;j<forest[i].size()-1;j++){
            size_t s = score(i,j,forest);
            if(s>best)
                best=s;
        }
    }
    return best;
}

int main(int argc, char *argv[]){
    std::ifstream input(argv[1]);
    std::string line;
    std::vector<std::vector<uint8_t>> forest;
    while(std::getline(input,line)){
        std::vector<uint8_t> row;
        for(auto i:line)
            row.push_back(i-'0');
        forest.push_back(row);
    }
    std::cout<<find_trees(forest)<<" best: "<<best(forest)<<std::endl;
}