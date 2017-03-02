
#ifndef LAB1_HPP
#define LAB1_HPP

#include <pthread.h>
#include <cstdio>
#include <string>
#include <random>
#include <vector>
#include <ctime>
#include <mutex>

#include "datastructs/queue.h"
#include "datastructs/hashmap.h"

#ifdef DEBUG
#include "datastructs/log.h"
#endif

#define TESTSIZE 10000		//default test size
#define INSERT 2			//percent probability for insert .2
#define THREADS 4			//default number of threads
#define MIN -256			//default insert min value
#define MAX 256				//default insert max value
#define MIN32 -2147483647 	//default min int
#define MAX32 2147483647	//default max int 

//struct of items to be shared among the threads
struct dataStruct{
	ThreadSafeKVStore<std::string, int32_t> hmap;
	ThreadSafeListenerQueue<int32_t> queue;
	std::vector<double> times;
	std::mutex timeMutex;
	clock_t maxTime=0;
	
	#ifdef DEBUG
	ThreadSafeLog log;
	#endif
};

void* test(void* data);

#ifdef EFFICIENT
#include "conditionalAddon/efficient.hpp"
#endif

#ifdef COMPARE
#include "conditionalAddon/compare.hpp"
#endif

#endif