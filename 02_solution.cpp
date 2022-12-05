#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

inline const unsigned int mod(int a, int b){
    // https://stackoverflow.com/a/23214219/14681457
    return ((a %= b) < 0) ? a+b : a;
}

int win(char opp, char you){
    // returns 0 for draw, 1 for win and 2 for lose
    const int diff = 'X'-'A';
    int win = you-opp-diff;
    return mod(win, 3);
}

int score(char opp, char you){
    /*
    opp:
        A->rock
        B->paper
        C->scissors
    you:
        X->rock (+1)
        Y->paper (+2)
        Z->scissors(+3)
    match:
        loss (+0)
        draw (+3)
        win (+6)
    logic:
        paper>rock>scissors>paper <- something with difference and modulus
        'X'
    */
    int score = 0;
    
    score += you =='X'? 1 : you == 'Y' ? 2 : you == 'Z' ? 3 :-1000; //last should not happen
    int result = win(opp,you);
    score += result != 0 ? (result == 1 ? 6 : 0) : 3;

    return score;
}

int score2(char opp, char you){
    /*
    which:
        X: lose (2) -> 0 //aka -1
        Y: draw (0) -> 3
        Z: win  (1) -> 6
    bla:
        A:Y->draw->A
        B:X->lose->A (B-1)
        C:Z->win->A ()
    */
    int score = 0;
    score += you == 'X' ? 0 : you == 'Y' ? 3 : you == 'Z' ? 6 : -1000;
    int dchoice = (you == 'X' ? -1 : you == 'Y' ? 0 : you == 'Z' ? 1 : -1000);
    char choice = 'A' + mod(opp+dchoice-'A',3);
    score += choice == 'A' ? 1 : choice == 'B' ? 2 : choice == 'C' ? 3 : -1000;
    return score;
}

int main(int argc, char *argv[]){
    std::ifstream input(argv[1]);
    std::string line;
    std::vector<char> opp, you;

    while(getline(input, line)){
        opp.push_back(line[0]);
        you.push_back(line[2]);
    }

    int total_score = 0, total_score2=0;
    for(int i=0;i<opp.size();i++){
        total_score += score(opp[i],you[i]);
        total_score2 += score2(opp[i],you[i]);
    }
    std::cout<<"total score: "<<total_score<<std::endl;
    std::cout<<"total score2: "<<total_score2<<std::endl;
}