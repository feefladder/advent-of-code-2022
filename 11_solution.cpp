#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "utils.hpp"

uint64_t add(uint64_t a,uint64_t b){
  return a+b;
}

uint64_t mul(uint64_t a, uint64_t b){
  return a*b;
}

uint64_t sq(uint64_t a, uint64_t b){
  return a*a;
}

class Monkey{
public:
  std::vector<uint64_t> items;
  uint64_t divnum, mtrue, mfalse, old, inspects=0;
  uint64_t* val=&old;
  uint64_t (*opPtr)(uint64_t, uint64_t);
  Monkey(){}
  Monkey(Monkey const& m): items(m.items),divnum(m.divnum),mtrue(m.mtrue),mfalse(m.mfalse),old(m.old),inspects(m.inspects),opPtr(m.opPtr){
    if(m.val==&m.old){
        val=&old;
    } else {
        val=new uint64_t(*m.val); //m.val is deleted on other monkey delete
    }
  }
  ~Monkey(){
    if(val!=&old){
      delete val;
    }
  }
  Monkey(std::vector<uint64_t> its, std::string op, uint64_t dn, uint64_t mt, uint64_t mf):
  items(its), divnum(dn), mtrue(mt), mfalse(mf){
    //only options are +, * and square
    if(op[op.size()-1] == 'd'){
      val = &old;
    } else {
      uint64_t pos = op.rfind(" ");
      val = new uint64_t(std::stoi(op.substr(pos+1)));
    }
    if(op[5] == '+'){
      opPtr = add;
    } else if(op[5] == '*'){
      opPtr = mul;
    }
  }

  void do_thing(std::vector<Monkey>& monkeys){
    for(auto& it: items){
      inspects++;
      old = it;
      it = opPtr(old, *val)/3;
      uint64_t m = it%divnum?mfalse:mtrue;
      monkeys[m].items.emplace_back(it);
    }
    items.clear();
  }

  void do_thing2(std::vector<Monkey>& monkeys, uint64_t& lcm){
    for(auto& it: items){
      it %= lcm;
      inspects++;
      old = it;
      it = opPtr(old, *val)%lcm;
      uint64_t m = it%divnum?mfalse:mtrue;
      monkeys[m].items.emplace_back(it);
    }
    items.clear();
  }

  bool operator<(Monkey const& m){
    return inspects < m.inspects;
  }
};

uint64_t max2(std::vector<Monkey>& monkeys){
  std::vector<uint64_t> inspects;
  for(auto& m:monkeys){
    inspects.push_back(m.inspects);
  }
  std::sort(inspects.begin(), inspects.end());

  uint64_t two_prod = 1;
  for (uint64_t i=inspects.size()-2;i<inspects.size();i++){
      two_prod *= inspects[i];
  }
  return two_prod;
}

int main(int argc, char *argv[]){
  std::ifstream input(argv[1]);
  std::string line;
  std::vector<Monkey> monkeys, m2;

  uint64_t old = 5;
  uint64_t* val = &old;
  while(std::getline(input, line)){
    if(line == "")
      continue;
    std::getline(input, line);
    uint64_t pos = line.find(":");
    std::vector<uint64_t> its = utils::explode<uint64_t>(line.substr(pos+2),',');
    std::getline(input, line);
    std::string op = line.substr(line.find("=")+1);
    std::getline(input, line);
    uint64_t dn = std::stoi(line.substr(line.find("by")+3));
    std::getline(input, line);
    uint64_t tm = line[line.size()-1] - '0';
    std::getline(input, line);
    uint64_t fm = line[line.size()-1] - '0';
    monkeys.emplace_back(its, op, dn, tm, fm);
    m2.emplace_back(its, op, dn, tm, fm);
  }

  for(uint64_t i=1;i<=20;i++){
    for(uint64_t m=0;m<monkeys.size();m++){
      monkeys[m].do_thing(monkeys);
    }
  }

  uint64_t lcm = 1;
  for(auto& m: m2){
    lcm *= m.divnum;
  }
  for(uint64_t i=1;i<=10000;i++){
    for(uint64_t m=0;m<m2.size();m++){
      m2[m].do_thing2(m2, lcm);
    }
  }

  std::cout<<"inspects: "<<max2(monkeys)<<','<<max2(m2)<<std::endl;
}