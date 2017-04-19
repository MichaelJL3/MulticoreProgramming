
#pragma once

/*************************************\

 filesys.hpp
 Author: Michael Laucella
 Last Modified: 4/18/17

 basic functions for thread safe file
 manipulation

 includes:
    writing
    reading
    deleting

\*************************************/

#include <fstream>

//path where server files are stored
const std::string STORAGE_PATH="stored/";

//write to a file
bool wrFile(std::string filename, std::string content){
    std::ofstream fd(STORAGE_PATH+filename, std::ofstream::out);
    
    if(fd.is_open()){
        fd<<content;
        fd.close();
    }else{
        fd.close();
        return false;
    }

    return true;
}

//read the contents of a file
bool rdFile(std::string filename, std::string &content){
    std::ifstream fd(STORAGE_PATH+filename, std::ofstream::out);
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

//delete a file
bool dlFile(std::string filename){
    return (std::renove(STORAGE_PATH+filename)?false:true);
}