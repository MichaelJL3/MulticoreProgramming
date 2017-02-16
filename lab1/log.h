
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
	pthread_mutex_t lock, printLock;
public:
	ThreadSafeLog();				//constructor
	~ThreadSafeLog();				//destructor
	int insert(std::string in);		//inserts into the log
	std::string toString();			//return the log as a formatted string
	void printLog();				//print the log
};

/*
constructor

initializes the mutex locks
*/
ThreadSafeLog::ThreadSafeLog(){
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_init(&printLock, NULL);
}

/*
destructor

destroys the mutex locks
*/
ThreadSafeLog::~ThreadSafeLog(){
	pthread_mutex_destroy(&lock);
	pthread_mutex_destroy(&printLock);
}

/*
insert(string)

locks and inserts a string into the log

if memory error occurs return -1
else return 0
*/
int ThreadSafeLog::insert(std::string in){
	pthread_mutex_lock(&lock);
	try{
		log.push_back(in);
		pthread_mutex_unlock(&lock);
		return 0;
	} catch (std::bad_alloc& e){
		pthread_mutex_unlock(&lock);
		return -1;
	}
}

/*
toString()

locks then iterates through the log creating a formatted string

returns the formatted string
*/
std::string ThreadSafeLog::toString(){
	std::string logged="";

	pthread_mutex_lock(&lock);
	for(auto it=log.begin(); it!=log.end(); it++){
		logged+=*it+"\n";
	}
	pthread_mutex_unlock(&lock);

	return logged;
}

/*
printLog()

locks the print grabs the string then prints it
*/
void ThreadSafeLog::printLog(){
	pthread_mutex_lock(&printLock);
	printf("%s\n", toString().c_str());
	pthread_mutex_unlock(&printLock);
}

#endif