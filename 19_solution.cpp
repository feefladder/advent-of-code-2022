// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "utils.hpp"

class Robot{
public:
  uint8_t type;
  std::map<uint8_t, uint> costs;

  Robot(uint8_t type, std::string& cost_description, std::map<std::string, uint8_t>& mineral_ids);
  bool operator==(uint8_t const& type) const;
};

class State;

class Blueprint{
  std::string name;
public:
  uint max_minutes =0, n_minerals=0;
  std::map<std::string, uint8_t> mineral_ids;

  std::vector<std::string> mineral_names;
  std::vector<Robot> robots;

  std::vector<State> states;
  Blueprint(Blueprint const& b);
  Blueprint(std::string n, std::string line, uint max_mins);
  void print() const;
  uint travel();
  void run();
};

class State{
  friend class Blueprint;
  uint8_t curr_mins;
  std::vector<uint> n_minerals;
  std::vector<uint> n_robots;
  const Blueprint* my_blueprint;
public:
  State(Blueprint* b);
  State(State const& s);
  State(State const& s, Robot& r);
  uint upper_bound() const;
  uint lower_bound() const;
  void print() const;
  bool const operator<(State const& s ) const;
  bool operator==(State const& s) const{
    return curr_mins == s.curr_mins and n_minerals == s.n_minerals and n_robots == s.n_robots;
  }
  uint req_mins(Robot const& r){
    uint req_mins = 1;
    for(auto& c: r.costs){
      if(n_robots[c.first] ==0)
        return 0;
      uint r_mins = (c.second-n_minerals[c.first])/n_robots[c.first];
      if (r_mins>req_mins)
        req_mins = r_mins;
    }
    return req_mins;
  }
};

State::State(State const& s):
  n_minerals(s.n_minerals),
  n_robots(s.n_robots),
  curr_mins(s.curr_mins)
{
    // std::cout<<"copying "<<std::endl;
    s.print();
    my_blueprint = s.my_blueprint;
    // std::cout<<my_blueprint;
}

Robot::Robot(uint8_t t, std::string& c, std::map<std::string, uint8_t>& mineral_ids):
  type(t)
{
  // c=c.substr(0,c.size()-1);
  // std::cout<<c<<std::endl;
  //3 ore and 19 clay
  for(std::string cost: utils::explode(c," and ")){
    //3 ore
    int pos = cost.find(' ');
    uint8_t key = mineral_ids[cost.substr(pos+1)];
    costs[key] = std::stoi(cost.substr(0,pos));
  }
}

bool Robot::operator==(uint8_t const & t) const{
  return t==type;
}

Blueprint::Blueprint(std::string n, std::string line, uint max_mins): name(n), max_minutes(max_mins) {
  for(std::string& r_desc : utils::explode(
    line,
    " Each "
  )){
    //obsidian robot costs 3 ore and 19 clay
    //        |pos
    int pos = r_desc.find(" robot");

    std::string r_name = r_desc.substr(0,pos);
    // std::cout<<r_name<<std::endl;
    mineral_ids[r_name]=n_minerals;
    mineral_names.push_back(r_name);

    std::string r_costs = r_desc.substr(pos+13, r_desc.size()-pos-14);//" robot costs".size()=13
    robots.emplace_back(n_minerals, r_costs, mineral_ids);
    n_minerals++;
  }
  //create the first state
  states.emplace_back(this);
}

Blueprint::Blueprint(Blueprint const& b):
  name(b.name),
  max_minutes(b.max_minutes),
  n_minerals(b.n_minerals),
  mineral_ids(b.mineral_ids),
  mineral_names(b.mineral_names),
  robots(b.robots),
  states(b.states)
{
  for(int i=0;i<states.size();i++){
    // std::cout<<"bla "<<this<<std::endl<<states[i].my_blueprint<<std::endl;
    states[i].my_blueprint = this;
    // std::cout<<"bla "<<this<<std::endl<<states[i].my_blueprint<<std::endl;
  }
}

void Blueprint::print() const {
  std::cout<<name<<std::endl;
  // for(auto& m: mineral_ids){
  //   std::cout<<"mineral: "<<m.first<<std::endl;
  // }
  for(auto& r: robots){
    std::cout<<mineral_names[r.type]<<" robot:\n costs  ";
    for(auto& c: r.costs){
      std::cout<<c.second<<" "<<mineral_names[c.first]<<std::endl;
    }
  }
}

void State::print() const{
  std::cout<<"printing state at:"<<int(curr_mins)<<"\nwith robots:\n";
  for(int i=0; i<n_minerals.size(); i++){
    std::cout<<"    "<<my_blueprint->mineral_names[i]<<": "<<n_minerals[i]<<" and "<<n_robots[i]<<" robots\n";
  }
  std::cout<<std::endl;
}

State::State(Blueprint* b): 
  curr_mins(0),
  n_minerals(b->n_minerals,0),
  n_robots(b->n_minerals+1,0),
  my_blueprint(b)
{
  n_robots[0]=1;
}

uint State::upper_bound() const {
  /**
  1.
  2..
  3...
  4....
  5~....
  6~~.... build the best robot every minute
  7#######that robot will produce what we need every minute
  8########
  9######### tetrahedral number yay
  */
  uint asdf = n_robots[n_robots.size()-1];
  uint left_mins = my_blueprint->max_minutes - curr_mins;
  // std::cout<<"already built "<<asdf<<"robots\n"<<"still "<<left_mins<<"minutes to go\n";
  return asdf+left_mins*(left_mins+1)*(left_mins+2)/6;
}

uint State::lower_bound() const {
  /**
  only save for building the most expensive robot
  */
  if(n_robots[n_robots.size()-2] == 0)
    return 0;

  auto geodes = n_robots[n_robots.size()-1];
  uint curr = curr_mins, next = curr;
  //create two vectors with amounts and costs for till the end
  std::vector<uint> n_m, r_costs, n_r;
  auto costs = my_blueprint->robots[my_blueprint->robots.size()-1].costs;
  for (auto& c: costs){
    // std::cout<<my_blueprint->mineral_names[c.first]<<':'<<c.second<<std::endl;
    n_m.push_back(n_minerals[c.first]);
    n_r.push_back(n_robots[c.first]);
    r_costs.push_back(c.second);
  }
  do {
    next = next+1;
    // for(auto& c:costs){
    //   uint n = ()
    // }
    for(int i=0;i<n_m.size();i++ ){
      auto n = (r_costs[i] - n_m[i])/n_r[i];
      if(curr+n>next){
        next = next+n;
      }
    }
    uint step = next-curr, steps_left=my_blueprint->max_minutes-next;
    for(int i=0;i<n_m.size();i++ ){
      n_m[i] += step*n_r[i]-r_costs[i];
    }
    geodes += steps_left*(steps_left+1)/2;
    curr=next;
  } while(next<my_blueprint->max_minutes);
  return geodes;
}

State::State(State const& s, Robot& r):
  curr_mins(s.curr_mins),
  n_minerals(s.n_minerals),
  n_robots(s.n_robots),
  my_blueprint(s.my_blueprint)
{
  // std::cout<<"making robot: "<<my_blueprint->mineral_names[r.type]<<std::endl;
  uint n=1;
  for(auto& c: r.costs){
    // (r_costs[i] - n_m[i])/n_r[i]
    // std::map<uint8_t, uint> costs;
    uint t = (c.second-n_minerals[c.first])/n_robots[c.first];
    // std::cout<<"t: "<<t<<std::endl;
    if (t>n) 
      n=t;
    // already subtract the costs
    n_minerals[c.first] -= c.second;
  }
  std::cout<<"n: "<<n<<','<<int(s.curr_mins)<<std::endl;
  // make minerals
  for(int i=0;i<n_minerals.size();i++){
    n_minerals[i]+=n*n_robots[i];
  }
  //build the robot
  n_robots[r.type]++;
  curr_mins += n;
  print();
}

bool const State::operator<(State const & s) const {
  return s.upper_bound() < upper_bound();
}

uint Blueprint::travel(){
  uint best = 0;
  std::vector<State> visited;
  while(true){
    //see if we can buy a robot
    State s = states[0];
    std::cout<<states.size()<<std::endl;
    pop_heap(states.begin(), states.end());
    states.pop_back();
    std::cout<<states.size()<<std::endl;
    if(s.curr_mins>=max_minutes){
      if(s.lower_bound()>best)
        best = s.n_robots.back();
      break;
    }
    std::cout<<int(s.curr_mins)<<std::endl;
    // try to build all robots and make them branches
    for(auto& r: robots){
      uint req_mins = s.req_mins(r);
      if(req_mins == 0 or req_mins > max_minutes-s.curr_mins){
        continue;
      } else {
        std::cout<<"we can buy a "<<mineral_names[r.type]<<" robot! at min:"<<int(s.curr_mins)<<"\n";
        State new_s = State(s,r);
        if(std::find(visited.begin(),visited.end(),new_s)==visited.end()){
          std::cout<<"appending to visited!\n";
          visited.push_back(s);
        } else {
          std::cout<<"not appending to visited!\n";
          continue;
        }
        states.emplace_back(s, r);
        push_heap(states.begin(),states.end());
      }
    }
    
    // return 1;//s.lower_bound();
  }
  return best;
}

int main(int argc, char *argv[]){
  uint run_for = 24;
  std::ifstream input(argv[1]);
  std::string line;
  std::vector<Blueprint> blues;
  blues.reserve(20);
  while(std::getline(input, line)){
    int pos = line.find(':');
    // Blueprint blue = Blueprint;
    blues.emplace_back(line.substr(0,pos), line.substr(pos+7), run_for);
  }
  for(auto& blue: blues){
    std::cout<<blue.travel()<<std::endl;
  }
}