// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include "utils.hpp"

typedef uint8_t m_id;
static const int t_max = 23;


class State{
public:
  int t=0;
  std::vector<int> n_minerals, n_robots;
  State(int n):n_minerals(n,0),n_robots(n,0){
    n_robots[0]++;
  }
  State(State const& prev, int n_steps, std::map<m_id, int> const & r_costs, m_id const& r):
    n_minerals(prev.n_minerals),
    n_robots(prev.n_robots)
  {
    // if(n_steps != 1){
    //   std::cout<<"making state from "<<prev.t<<'+'<<n_steps<<std::endl;
    //   std::cout<<"  making robot "<<int(r)<<':'<<std::endl;
    //   for(auto& c: r_costs){
    //   std::cout<<"    "<<n_minerals[c.first]<<'+'<<n_steps<<'*'<<n_robots[c.first]<<','<<c.second<<std::endl;
    //   }
    // }
    for(auto& c: r_costs){
      n_minerals[c.first]-=c.second;
    }
    for(int i=0;i<n_robots.size();i++){
      n_minerals[i]+=n_robots[i]*n_steps;
      if(n_minerals[i]<0){
        std::cout<<"big error!!"<<n_steps<<'*'<<n_robots[i]<<std::endl;
      }
    }
    // if(r==n_robots.size()-1){
    //   int geode_steps = t_max-prev.t-n_steps-3;
    //   // std::cout<<"making geode bot!"<<geode_steps<<std::endl;
    //   n_minerals.back() += utils::triangle_nr(geode_steps);
    // } else{
      n_robots[r]++;
    // }
    t = prev.t+n_steps;
  }
  bool operator==(State const& s){
    return t==s.t and n_minerals==s.n_minerals and n_robots==s.n_robots;
  }
};

class Blueprint{
public:
  std::string name;
  std::map<std::string,m_id> r_types;
  std::vector<std::string> r_names;
  std::vector<std::map<m_id,int>> r_costs;
  std::vector<uint> max_costs;
  std::vector<State> states;

  void print_state(State s) const {
    if(s.n_minerals.back()==0)
      return;
    std::cout<<"  state at: "<<s.t<<std::endl;
    for(int i=0;i<s.n_minerals.size();i++){
      std::cout<<"    "<<r_names[i]<<": "<<s.n_minerals[i]<<" and "<<s.n_robots[i]<<" robots\n";
    }
  }

  void print(bool p_staets) const {
    std::cout<<name<<":\n";
    for(int i=0;i<r_types.size();i++){
      std::cout<<"  "<<r_names[i]<<" robot costs";
      for(auto& c: r_costs[i]){
        std::cout<<' '<<c.second<<' '<<r_names[c.first];
      }
      std::cout<<" max is: "<<max_costs[i]<<std::endl;
    }
    if (not p_staets)
      return;
    for(State const& state: states){
      print_state(state);
    }
  }

  inline int num_steps(State const& s, m_id r) const {
    int n_max=1;
    for(auto& c: r_costs[r]){
      if(s.n_robots[c.first] ==0)
        return 0;
      int too_little = c.second-s.n_minerals[c.first];
      int n = too_little/s.n_robots[c.first];
      if(too_little<0){
        return 0;
        std::cout<<"calced "<<n<<" steps to make "<<too_little<<" at "<<s.n_robots[c.first]<<"/step"<<std::endl;
        print_state(s);
      }
      n+= too_little%s.n_robots[c.first] != 0;
      if(n>n_max)
        n_max=n;
    }
    // std::cout<<"going "<<n_max<<"steps\n";
    return n_max;
  }

  void calc_max_costs(){
    max_costs = std::vector<uint>(r_names.size(),0);
    for(auto& m: r_costs){
      for(auto& c: m){
        if(max_costs[c.first]<c.second)
          max_costs[c.first] = c.second;
      }
    }
    // we want infinite geode bots;
    max_costs.back()=-1;
  }

  int dfs(){
    int best = 0;
    std::queue<State> s_queue;
    for(auto& s: states){
      s_queue.push(s);
    }
    while(!s_queue.empty()){
      State s = s_queue.front();
      s_queue.pop();
      for(int r=0;r<r_names.size();r++){
        if(s.n_robots[r]==max_costs[r])
          continue;
        int n = num_steps(s,r);
        if(n<=0)
          continue;
        if(s.t+n>=t_max){
          // std::cout<<"some robot finished!\n";
          int score = s.n_minerals.back()+(t_max-s.t-1)*s.n_robots.back();
          if (score>best)
            best = score;
          continue;
        }
        // we are not at the end and can add it to the queue
        State s_new = State(s, n, r_costs[r], r);
        if(std::find(states.begin(),states.end(), s_new) == states.end()){
          //add it to the queue
          s_queue.push(s_new);
          states.push_back(s_new);
          // print(true);
        } else {
          // already visited: continue
          continue;
        }
        
      }
      // return best;
    }
    return best;
  }
};

int main(int argc, char *argv[]){
  std::ifstream input(argv[1]);
  std::vector<Blueprint> baes;
  std::string line;
  while(std::getline(input, line)){
    /*
    """Blueprint 1:\
    Each ore robot costs 3 ore.\
    Each clay robot costs 3 ore.\
    Each obsidian robot costs 3 ore and 19 clay.\
    Each geode robot costs 3 ore and 17 obsidian.""";
    */
    Blueprint b;
    int pos = line.find(':');
    b.name = line.substr(0,pos);
    for(std::string& r_desc: utils::explode(line.substr(pos+7)," Each ")){
    /**
    """ore robot costs 3 ore.

    clay robot costs 3 ore.

    obsidian robot costs 3 ore and 19 clay.

    geode robot costs 3 ore and 17 obsidian.""";
    */
      pos = r_desc.find(" robot costs ");
      std::string r_name = r_desc.substr(0,pos);
      std::string r_c = r_desc.substr(pos+13,r_desc.size()-pos-14);
      b.r_types[r_name] = b.r_names.size();
      b.r_names.push_back(r_name);
      std::map<m_id, int> c_map;
      for(std::string& cost: utils::explode(r_c," and ")){
        /*
      """ 3 ore

          3 ore

          3 ore\
          19 clay

          3 ore\
          17 obsidian""";
          */
        pos = cost.find(' ');
        std::string type = cost.substr(pos+1);
        c_map[b.r_types[type]] = std::stoi(cost.substr(0,pos));
      }
      b.r_costs.push_back(c_map);
    }
    b.states.emplace_back(b.r_costs.size());
    b.calc_max_costs();
    baes.push_back(b);
  }
  for(auto& b: baes){
    std::cout<<b.dfs()<<std::endl;
    // b.print(true);
  }
}