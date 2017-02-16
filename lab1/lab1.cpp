
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <random>
#include <vector>
#include <ctime>

#include "queue.h"
#include "hashmap.h"

#ifdef DEBUG
#include <mutex>
std::mutex lock;
#endif

#define TESTSIZE 10000
#define THREADS 4
#define MIN -256
#define MAX 256
#define MIN32 -2147483647 
#define MAX32 2147483647

struct dataStruct{
	ThreadSafeKVStore<std::string, int32_t> hmap;
	ThreadSafeListenerQueue<int32_t> queue;	
};

void* test(void* data){
	dataStruct *dt=(dataStruct*) data;

	int32_t probability, generatedVal, val, sum=0;
	
	std::vector<std::string> users;
	std::string user;

	std::default_random_engine gen(time(NULL));
	std::uniform_int_distribution<int32_t> dist(MIN, MAX);
	std::uniform_int_distribution<int32_t> distProbability(MIN32, MAX32);

	for(int i=0; i<TESTSIZE; i++){
		probability=distProbability(gen)%10;

		if(probability<2){
			generatedVal=dist(gen);
			sum+=generatedVal;

			#ifdef DEBUG
			lock.lock();
			printf("%d\n", generatedVal);
			lock.unlock();
			#endif

			user="user"+std::to_string(generatedVal);
			users.push_back(user);
			
			dt->hmap.accumulate(user, generatedVal);
		}else if(users.size()){
			user=users[distProbability(gen)%users.size()];
			if(dt->hmap.lookup(user, val)==-1){
				#ifdef DEBUG
				lock.lock();
				printf("Error looking up: %d\n", val);
				lock.unlock();
				#endif
				exit(1);
			}
		}
	}

	#ifdef DEBUG
	lock.lock();
	printf("sum for thread: %d\n", sum);
	lock.unlock();
	#endif

	if(dt->queue.push(sum)==-1){
		#ifdef DEBUG
		lock.lock();
		printf("Error adding value to queue: %d\n", sum);
		lock.unlock();
		#endif
		exit(1);
	}

	pthread_exit(NULL);
}

int main(int argv, char** argc){
	dataStruct data;

	pthread_t *threads;
	int rc, val, deduct=0, numThreads=THREADS;

	if(argv>1){
		numThreads=atoi(argc[1]);
	}else{
		printf("No Thread number entered using default: %d\n", THREADS);
	}
	
	#ifdef EFFICIENT
	numThreads--;
	printf("Running in efficiency mode\nnumber of threads: %d\nreduced as main runs the function itself\n", numThreads);
	#endif

	threads=new pthread_t[numThreads];

	for(int i=0; i<numThreads; i++){
		rc=pthread_create(&threads[i], NULL, test, (void*)&data);
		if(rc){
			printf("Error creating thread: %d error: %d\n", i, rc);
			deduct++;
		}
	}

	#ifdef EFFICIENT
	//test((void*)&data);
	//numThreads++;
	#endif

	numThreads-=deduct;
	for(int i=0; i<numThreads; i++){
		data.queue.listen(val);
		printf("Queue: %d\n", val);
	}

	delete threads;

	return 0;
}
