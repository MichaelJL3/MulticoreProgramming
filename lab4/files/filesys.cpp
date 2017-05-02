
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

ThreadSafeFiles::ThreadSafeFiles(const std::string path){
    STORAGE_PATH=path;
    pthread_rwlock_init(&lock, nullptr);
}

ThreadSafeFiles::~ThreadSafeFiles(){
    pthread_rwlock_destroy(&lock);
}

//write to a file
bool ThreadSafeFiles::wrFile(std::string filename, std::string content){
    bool ret=false;

    pthread_rwlock_wrlock(&lock);
    fw.open(STORAGE_PATH+filename, std::ofstream::out);

    ret=fw.is_open();
    if(ret)
        fw<<content;

    fw.close();
    pthread_rwlock_unlock(&lock);
    return ret;
}

//read the contents of a file
bool ThreadSafeFiles::rdFile(std::string filename, std::string &content){
    std::string line;
    bool ret=false;

    pthread_rwlock_rdlock(&lock);
    std::ifstream fd(STORAGE_PATH+filename);

    ret=fd.is_open();
    if(ret){
        content="";
        while(getline(fd,line))
            content+=line;    
    }

    fd.close();
    pthread_rwlock_unlock(&lock);
    return ret;
}

//delete a file
bool ThreadSafeFiles::dlFile(std::string filename){
    pthread_rwlock_wrlock(&lock);
    bool ret=!(remove((STORAGE_PATH+filename).c_str()));
    pthread_rwlock_unlock(&lock);
    return ret;
}
