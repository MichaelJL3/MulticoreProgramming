
#pragma once

#include <fstream>

#define PATH ""

bool wrFile(std::string filename, std::string content){
    std::ofstream fd(PATH+filename, std::ofstream::out);
    
    if(fd.is_open()){
        fd<<content;
        fd.close();
    }else{
        fd.close();
        return false;
    }

    return true;
}

bool rdFile(std::string filename, std::string &content){
    std::ifstream fd(PATH+filename, std::ofstream::out);
    std::string line;

    if(!fd.is_open()){
        fd.close();
        return false;
    }

    content="";
    while(getline(fd,line)){
        content+=line;    
    }

    fd.close();
    return true;
}

bool dlFile(std::string filename){
    return (std::renove(PATH+filename)?false:true);
}