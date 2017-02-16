
#include <pthread.h>
#include <cstdio>
#include <string>
#include <random>
#include <vector>
#include <ctime>
#include <mutex>

#include "queue.h"
#include "hashmap.h"

#ifdef DEBUG
#include "log.h"
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
	std::mutex timeMutex;
	clock_t maxTime=0;
	#ifdef DEBUG
	ThreadSafeLog log;
	#endif
};

void* test(void* data);

#ifdef EFFICIENT
void mainRunTest(void* data);
#endif
