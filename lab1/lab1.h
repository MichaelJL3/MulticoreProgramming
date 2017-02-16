
#include <pthread.h>
#include <cstdio>
#include <string>
#include <random>
#include <vector>
#include <ctime>

#include "queue.h"
#include "hashmap.h"

#ifdef DEBUG
#include "log.h"
#endif

#define TESTSIZE 10000
#define INSERT 2
#define THREADS 4
#define MIN -256
#define MAX 256
#define MIN32 -2147483647 
#define MAX32 2147483647

//struct of items to be shared among the threads
struct dataStruct{
	ThreadSafeKVStore<std::string, int32_t> hmap;
	ThreadSafeListenerQueue<int32_t> queue;	
	#ifdef DEBUG
	ThreadSafeLog log;
	#endif
};

void* test(void* data);

#ifdef EFFICIENT
void mainRunTest(void* data);
#endif
