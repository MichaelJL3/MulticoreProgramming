
#pragma once

/*************************************\

 filesys.hpp
 Author: Michael Laucella
 Last Modified: 4/22/17

 basic functions for thread safe file
 manipulation

 includes:
    writing
    reading
    deleting

\*************************************/

#include <fstream>
#include <string>
#include <pthread.h>

class ThreadSafeFiles{
    pthread_rwlock_t lock;
    const std::string PATH;
    std::ofstream fd;
    ThreadSafeFiles();
public:
    ThreadSafeFiles(const std::string path);
    //write to a file
    bool wrFile(std::string filename, std::string content);
    //read the contents of a file
    bool rdFile(std::string filename, std::string &content);
    //delete a file
    bool dlFile(std::string filename);
};
