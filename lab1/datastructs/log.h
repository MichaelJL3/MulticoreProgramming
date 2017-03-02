
#ifndef LOG_H
#define LOG_H

#include <pthread.h>
#include <cstdio>
#include <list>

/*************************************************************\

Class: ThreadSafeLog
Author: Michael Laucella
Last Modified: 2/16/2017

!!OS specific requires posix libraries

creates a thread safe log

for specific instance there will be more writes then reads
so no need for reader/writer lock

\*************************************************************/

class ThreadSafeLog{
	std::list<std::string> log;
	pthread_mutex_t lock;
public:
	ThreadSafeLog();				//constructor
	~ThreadSafeLog();				//destructor
	int insert(std::string in);		//inserts into the log
	int toString(std::string &str);	//return the log as a formatted string
	int printLog();					//print the log
};

/*
constructor

initializes the mutex locks
*/
ThreadSafeLog::ThreadSafeLog(){
	pthread_mutex_init(&lock, NULL);
}

/*
destructor

destroys the mutex locks
*/
ThreadSafeLog::~ThreadSafeLog(){
	pthread_mutex_destroy(&lock);
}

/*
insert(string)

locks and inserts a string into the log

if memory error occurs return -1
else return 0
*/
int ThreadSafeLog::insert(std::string in){
	if(pthread_mutex_lock(&lock))
		return -1;
	try{
		log.push_back(in);
		if(pthread_mutex_unlock(&lock))
			return -1;
		return 0;
	} catch (std::bad_alloc& e){
		if(pthread_mutex_unlock(&lock))
			return -1;
		return -1;
	}
}

/*
toString()

locks then iterates through the log creating a formatted string

returns the formatted string
*/
int ThreadSafeLog::toString(std::string &str){
	std::string logged="";

	if(pthread_mutex_lock(&lock))
		return -1;
	for(auto it=log.begin(); it!=log.end(); it++){
		logged+=*it+"\n";
	}
	str=logged;
	if(pthread_mutex_unlock(&lock))
		return -1;

	return 0;
}

/*
printLog()

locks the print grabs the string then prints it
*/
int ThreadSafeLog::printLog(){
	std::string logString;
	if(toString(logString))
		return -1;

	if(pthread_mutex_lock(&lock))
		return -1;
	printf("%s\n", logString.c_str());
	if(pthread_mutex_unlock(&lock))
		return -1;

	return 0;
}

#endif