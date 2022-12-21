// This project is libre, and licenced under the terms of the
// DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 4.0,
// as published by dtf, July 2019. See the COPYING file or
// https://github.com/dtf0/wtfpl for more details.

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

class Monkey;

static std::vector<Monkey> monkeys;
static std::map<std::string, size_t> monkey_names;
static size_t n_monkeys = 0;

static void connect_parents();
/*
adds the numbers of two monkeys indexed by U
*/
template <typename T, typename U>
static const T monkey_add(U a_index,U b_index) {
  return monkeys[a_index].calc()+monkeys[b_index].calc();
}

template <typename T, typename U>
static const T monkey_sub(U a_index,U b_index) {
  return monkeys[a_index].calc()-monkeys[b_index].calc();
}

template <typename T, typename U>
static const T monkey_mul(U a_index,U b_index) {
  return monkeys[a_index].calc()*monkeys[b_index].calc();
}

template <typename T, typename U>
static const T monkey_div(U a_index,U b_index) {
  return monkeys[a_index].calc()/monkeys[b_index].calc();
}

template <typename T, typename U>
static const T monkey_opp(U f_index, U s_index, char const& opp){
  switch (opp){
    case '+':
      return monkey_add<T>(f_index, s_index);
      break;
    case '-':
      return monkey_sub<T>(f_index, s_index);
      break;
    case '*':
      return monkey_mul<T>(f_index, s_index);
      break;
    case '/':
      return monkey_div<T>(f_index, s_index);
      break;
    default:
      std::cout<<"err! could not make anything from '"<<opp<<"'"<<std::endl;
      return -1;
  }
}

// template <typename T, typename U>
// static const T monkey_shank(U f_index, U s_index, char const& opp){

// }

class Monkey{
  int64_t value=0;
  size_t first=-1, second=-1, humn_deps=-1;
  // const int64_t (*calc_pointer)(size_t, size_t);
  char opp='#';
  std::string name;
public:
  size_t parent=-1;
  void print(){
    // std::cout<<"I am a monkey and I'm called "<<name<<std::endl;
    if(first != -1 and second != -1){
      std::cout<<"  I like to say "<<monkeys[first].name<<opp<<monkeys[second].name<<std::endl;
    } else {
      std::cout<<"  I do like the number "<<value<<std::endl;
    }
  }

  Monkey(): name("noname"){};
  Monkey(std::string n): name(n){};
  Monkey(int64_t val, std::string n): value(val), name(n){}
  Monkey(size_t monkey_f, size_t monkey_s, char monkey_opp, std::string n): 
    first(monkey_f),
    second(monkey_s),
    opp(monkey_opp),
    name(n)
  {}

  int64_t calc(){
    if(value != 0){
      return value;
    } else {
      int64_t val = monkey_opp<int64_t>(first, second, opp);
      // std::cout<<"value at: "<<name<<": "<<val<<std::endl;
      return val;//monkey_opp<int64_t>(first, second, opp);
    }
  }

  int64_t shank(int64_t val){
    if(value != 0){
      std::cout<<"I think I have to say "<<val<<std::endl;
      return val;
    }
    size_t gang = first == humn_deps ? second : first;
    int64_t shank_who=monkeys[gang].calc(), how;
    switch (opp){
      case '+':
      // m=h+s
      // h=m-s
        how = val-shank_who;
        // std::cout<<val<<"="<<how+shank_who<<"="<<how<<"+"<<shank_who<<std::endl;
        break;
      case '-':
        if(humn_deps == first){
          // m=h-s
          // h=m+s
          how = val+shank_who;
          // std::cout<<val<<"="<<how-shank_who<<"="<<how<<"-"<<shank_who<<std::endl;
        } else {
          // m=f-h
          // h=f-m
          how = shank_who-val;
          // std::cout<<val<<"="<<shank_who-how<<"="<<how<<"+"<<shank_who<<std::endl;
        }
        break;
      case '*':
          // m=h*s
          // h=m/s
          how = val/shank_who;
          // std::cout<<val<<"="<<how*shank_who<<"="<<how<<"*"<<shank_who<<std::endl;
        break;
      case '/':
        if(humn_deps == first){
          // m=h/s
          // h=m*s
          how = val*shank_who;
          // std::cout<<val<<"="<<how/shank_who<<"="<<how<<"/"<<shank_who<<std::endl;
        } else {
          // m=f/h
          // h=f/m
          how = shank_who/val;
          // std::cout<<val<<"="<<shank_who/how<<"="<<shank_who<<"/"<<how<<std::endl;
        }
        break;
      default:
        std::cout<<"cannot make anythign from '"<<opp<<"'"<<std::endl;
        return val;
        break;
    }
    // std::cout<<"If "<<name<<" were to say "<<val<<", then "<<monkeys[humn_deps].name<<" would have to say "<<how<<" cuz "<<shank_who<<std::endl;
    // print();
    // std::cout<<how<<std::endl;
    return monkeys[humn_deps].shank(how);
  }

  void bubble_up(size_t const& from){
    humn_deps=from;
    // std::cout<<"bubbling at "<<name<<std::endl;
    // if(from == first){
    //   int64_t val = monkeys[second].calc();
    //   std::cout<<" imagine we want "<<monkeys[first].name<<" to say "<<val<<std::endl;
    //   std::cout<<monkeys[first].shank(val);
    // } else if (from==second){
    //   int64_t val = monkeys[first].calc();
    //   std::cout<<" imagine we want "<<monkeys[second].name<<" to say "<<val<<std::endl;
    //   std::cout<<monkeys[second].shank(val);
    // } else {
    //   std::cout<<"only "<<name<<" after all"<<std::endl;
    // }
    if(parent == -1){
      size_t gang = from==first?second:first;
      // std::cout<<;
      // std::cout<<parent<<std::endl;
      std::cout<<"monkey "+name+" does not have a parent, and we should say "<<monkeys[from].shank(monkeys[gang].calc());
      return;
    }
    return monkeys[parent].bubble_up(monkey_names[name]);
  }

  friend void connect_parents();

};

static size_t maybe_add_monkey(std::string monkey_name){
    if (monkey_names.count(monkey_name) == 0){
        monkey_names[monkey_name]=n_monkeys++;
        monkeys[monkey_names[monkey_name]] = Monkey(monkey_name);
        return monkey_names[monkey_name];
    } else {
        // std::cout<<"monkey "<<monkey_name<<" is at pos: "<<monkey_names[monkey_name]<<std::endl;
        return monkey_names[monkey_name];
    }
}

void connect_parents(){
  for(size_t i=0; i<monkeys.size(); i++){
    if(monkeys[i].first != -1){
      monkeys[monkeys[i].first].parent = i;
    }
    if(monkeys[i].second != -1){
      monkeys[monkeys[i].second].parent = i;
    }
  }
}

int main(int argc, char *argv[]){
  std::ifstream input(argv[1]), f_counter(argv[1]);
  std::string line;
  size_t n_lines = std::count(std::istreambuf_iterator<char>(f_counter), 
             std::istreambuf_iterator<char>(), '\n');
  monkeys.resize(n_lines+1);
  // for(auto& m: monkeys){
  //   m.print();
  // }
  while(std::getline(input, line)){
    std::string name = line.substr(0,4);
    size_t i_name = maybe_add_monkey(name);
    if (line.size() == 17){
        // const char[18]: 
        // "root: pppw + sjmn"
        //        |    | |
        //        6   11 13
        // add an opps monkey
        std::string first = line.substr(6,4), second = line.substr(13);
        char opp = line[11];
        size_t  i_first = maybe_add_monkey(first),
                i_sec = maybe_add_monkey(second);
        monkeys[i_name] = Monkey(i_first, i_sec, opp, name);
        // monkeys[i_name].print();
        // std::cout<<name<<" does "<<first<<opp<<second<<std::endl;
    } else {
        // add a value monkey
        
        int64_t val = std::stoi(line.substr(6));

        monkeys[i_name] = Monkey(val, name);
        // monkeys[i_name].print();
        // std::cout<<name<<" likes "<<line.substr(6)<<std::endl;
    }
    // monkeys[i_name].print();
  }
  connect_parents();
  // for (int i=0; i<monkeys.size();i++){
  //   monkeys[i].print();
  // }
  std::cout<<monkeys[monkey_names["root"]].calc()<<std::endl;
  monkeys[monkey_names["humn"]].bubble_up(-2);
}