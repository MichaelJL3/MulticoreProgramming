
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
#include <string>

//path where server files are stored
const std::string STORAGE_PATH="stored/";

//write to a file
bool wrFile(std::string filename, std::string content);

//read the contents of a file
bool rdFile(std::string filename, std::string &content);

//delete a file
bool dlFile(std::string filename);