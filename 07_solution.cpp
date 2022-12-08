#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

class File{
public:
    int s;
    std::string name;
    File(int size, std::string nname) : s(size), name(nname){}
};

class Dir{
    std::string m_name;
    std::vector<File> m_files;
    std::vector<Dir*> m_subdirs;
    Dir* m_parent;
public:
    Dir(std::string name): m_name(name) {}

    Dir(Dir* parent, std::string name){
        m_parent = parent;
        m_name = name;
    }

    ~Dir(){
        for(auto& dir: m_subdirs){
            delete dir;
        }
    }

    void add(std::string what){
        if(what.substr(0,3)=="dir"){
            Dir* newdir = new Dir(this, what.substr(4));
            // std::cout<<"adding folder "<<what.substr(3)<<" to "<<m_name<<std::endl;
            m_subdirs.push_back(newdir);

        } else { //file
            int pos = what.find(' ');
            // std::cout<<"adding file "<<what.substr(pos)<<" to "<<m_name<<std::endl;
            m_files.push_back(File(std::stoi(what.substr(0,pos)),what.substr(pos+1)));
        }
    }

    Dir* get_parent(){
        return m_parent;
    }

    Dir* get_child(std::string cname){
        for(auto& sd: m_subdirs)
            if(sd->m_name == cname)
                return sd;
        return NULL;
    }

    void print(uint depth){
        for(int i=0;i<depth;i++)
            std::cout<<'|';
        std::cout<<'-'<<m_name<<std::endl;
        for(auto& file: m_files){
            for(int i=0;i<depth+1;i++)
                std::cout<<'|';
            std::cout<<'-'<<file.name<<file.s<<std::endl;
        }
        for(auto& dir: m_subdirs)
            dir->print(depth+1);
    }

    int size(){
        int s = 0;
        for(auto f: m_files)
            s += f.s;
        for(auto d: m_subdirs)
            s += d->size();
        return s;
    }

    template<int LT>
    int sizelt(){
        int slt =0, s=size();
        if(s < LT)
            slt += s;
        for(auto& sd: m_subdirs)
            slt += sd->sizelt<LT>();
        return slt;
    }

    size_t smallest_gt(int GT){
        size_t sizegt = size()>=GT ? size() : -1; //becasuse size_t, -1 is huge;
        for(auto& sd: m_subdirs){
            size_t sdgt = sd->smallest_gt(GT);
            sizegt = sdgt < sizegt ? sdgt : sizegt;
        }
        return sizegt;
    }
};

int main(int argc, char *argv[]){
    std::ifstream input(argv[1]);
    std::string line;
    Dir root=Dir("/");
    Dir* current = &root;
    std::getline(input, line);
    std::getline(input, line);
    while(std::getline(input, line)){
        if(line[0] == '$'){
            if(line.substr(2,2)=="cd"){
                if(line.substr(5,2)==".."){
                    current = current->get_parent();
                } else {
                    current = current->get_child(line.substr(5));
                    if(current == NULL)
                        return -1;
                }
            } //else continue; (ls is not super necessary)
        } else { //listing files
            current->add(line);
        }
    }
    // root.print(0);
    std::cout<<"total size of folders less than 100000: "<<root.sizelt<100000>()<<std::endl;
    int req_space = 30000000, total_space = 70000000, used_space = root.size();
    int free_space = total_space-used_space;
    int to_free=req_space-free_space;
    std::cout<<"size of smallest folder greater than "<<to_free<<": "<<root.smallest_gt(to_free)<<std::endl;
    // hopefully, root will cascade delete everything
}