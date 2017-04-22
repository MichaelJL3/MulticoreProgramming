
#include "filesys.hpp"

/*************************************\

 filesys.cpp
 Author: Michael Laucella
 Last Modified: 4/22/17

 basic functions for thread safe file
 manipulation

 includes:
    writing
    reading
    deleting

\*************************************/

//write to a file
bool ThreadSafeFiles::wrFile(std::string filename, std::string content){
    pthread_rwlock_wrlock(&lock);
    fd=open(STORAGE_PATH+filename, std::ofstream::out);
    
    if(fd.is_open()){
        fd<<content;
        fd.close();
        pthread_rwlock_unlock(&lock);
        return true;
    }

    fd.close();
    pthread_rwlock_unlock(&lock);
    return false;
}

//read the contents of a file
bool ThreadSafeFiles::rdFile(std::string filename, std::string &content){
    std::string line;

    pthread_rwlock_rdlock(&lock);
    std::ifstream fd(STORAGE_PATH+filename, std::ofstream::out);

    if(!fd.is_open()){
        fd.close();
        pthread_rwlock_unlock(&lock);
        return false;
    }

    content="";
    while(getline(fd,line)){
        content+=line;    
    }

    fd.close();
    pthread_rwlock_unlock(&lock);
    return true;
}

//delete a file
bool dlFile(std::string filename){
    pthread_rwlock_wrlock(&lock);
    bool ret=(std::remove(std::string(STORAGE_PATH+filename).c_str())?false:true);
    pthread_rwlock_unlock(&lock);
    return ret;
}