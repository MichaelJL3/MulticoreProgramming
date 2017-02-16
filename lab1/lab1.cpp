
#include "lab1.h"

/*************************************\

 lab1.cpp
 Author: Michael Laucella
 Last Modified: 2/16/17

 runs a test among n threads of a key
 value store using 10000 iterations
 of randomly generated keys with
 20% probability of inserting and 80%
 of reading

 failures to find a previously inserted
 item results in a crash, as nothing
 should be removed from the map

 at the end all the run times are 
 displayed and the sums of each thread
 is compared to that of the map

\*************************************/
int main(int argv, char** argc){
	//shared data structure
	dataStruct data;

	pthread_t *threads;
	int rc, val, sum=0, deduct=0, numThreads=THREADS;

	//get the number of threads if no value is entered uses default
	if(argv>1){
		numThreads=atoi(argc[1]);
		printf("Running %d Threads\n", numThreads);
	}else{
		printf("No Thread number entered using default: %d\n", numThreads);
	}
	
	//efficiency allows main to run the function as well using n-1 threads
	#ifdef EFFICIENT
	numThreads--;
	printf("Running in efficiency mode\nnumber of threads: %d\nreduced as main runs the function itself\n", numThreads);
	#endif

	threads=new pthread_t[numThreads];

	//create the threads
	for(int i=0; i<numThreads; i++){
		rc=pthread_create(&threads[i], NULL, test, (void*)&data);
		//checks for errors creating the thread 
		if(rc){
			printf("Error creating thread: %d error: %d\n", i, rc);
			deduct++;
		}
	}

	//runs one instance of the test on main
	#ifdef EFFICIENT
	mainRunTest((void*)&data);
	numThreads++;
	#endif

	//deduct any threads that failed to create
	//as they will have no entries in the queue
	numThreads-=deduct;
	for(int i=0; i<numThreads; i++){
		data.queue.listen(val);
		#ifdef DEBUG
		printf("Value received from Queue: %d\n", val);
		#endif
		sum+=val;
	}

	//grab the maps sum
	val=data.hmap.sum();

	#ifdef DEBUG
	if(val!=sum)
		puts("[ERROR] -> sums do not match");
	puts("\nPrinting log report");
	data.log.printLog();
	#endif

	//display the two sums
	printf("Value from threads: %d\nValue from map: %d\n", sum, val);

	delete threads;

	return 0;
}

/*
test(void*)

runs a 10000/TESTSIZE iteration test
TESTSIZE defined in lab1.h

using .2%/INSERT insert and .8% lookup
INSERT defined in lab1.h

inserts values between -256 and 256 (MIN and MAX)
MIN and MAX defined in lab1.h

adds all the values and stores in a queue
before exiting

compiling with debug allows reports of failures
and status updates to be viewed
*/
void* test(void* data){
	clock_t st=clock(), en;

	dataStruct *dt=(dataStruct*) data;

	int32_t probability, generatedVal, val, sum=0;
	
	std::vector<std::string> users;
	std::string user;

	//random generators
	std::default_random_engine gen(time(NULL));
	std::uniform_int_distribution<int32_t> dist(MIN, MAX);
	std::uniform_int_distribution<int32_t> distProbability(MIN32, MAX32);

	#ifdef DEBUG
	pthread_t thread=pthread_self();
	std::string id=std::to_string((unsigned long int) thread);
	dt->log.insert("Starting Thread: "+id);
	#endif

	for(int i=0; i<TESTSIZE; i++){
		//probability 0-9
		probability=distProbability(gen)%10;

		//20% probability insert
		if(probability<INSERT){
			generatedVal=dist(gen);
			sum+=generatedVal;

			user="user"+std::to_string(generatedVal);
			users.push_back(user);
			
			//modify the map
			dt->hmap.accumulate(user, generatedVal);
		}else if(users.size()){
			//80% probability lookup
			//if theres a value in the users vector grab a random one and look for it
			user=users[distProbability(gen)%users.size()];
			
			//if value is not in map exit
			if(dt->hmap.lookup(user, val)==-1){
				#ifdef DEBUG
				dt->log.insert("Error looking up value: "+std::to_string(val)+" on Thread: "+id);
				dt->log.printLog();
				#endif
				exit(1);
			}
		}
	}

	#ifdef DEBUG
	dt->log.insert("Sum: "+std::to_string(sum)+" from Thread: "+id);
	#endif

	//push to queue, if it fails exit
	if(dt->queue.push(sum)==-1){
		#ifdef DEBUG
		dt->log.insert("Error adding sum to queue: "+std::to_string(sum)+" from Thread: "+id);
		dt->log.printLog();
		#endif
		exit(1);
	}

	#ifdef DEBUG
	dt->log.insert("Finished Thread: "+id);
	#endif

	//the largest en
	//and all the run times

	en=clock();
	//(double)en-st/CLOCKS_PER_SEC;
	//how to handle time display/last time^^^^^^^

	pthread_exit(NULL);
}


//if efficient mode is enabled define the function for main
//to run (only difference between the original id the call to pthread_exit)
#ifdef EFFICIENT

//same as above test(void* data)
void mainRunTest(void* data){
	dataStruct *dt=(dataStruct*) data;

	int32_t probability, generatedVal, val, sum=0;
	
	std::vector<std::string> users;
	std::string user;

	//random generators
	std::default_random_engine gen(time(NULL));
	std::uniform_int_distribution<int32_t> dist(MIN, MAX);
	std::uniform_int_distribution<int32_t> distProbability(MIN32, MAX32);

	#ifdef DEBUG
	dt->log.insert("Starting Test From Main");
	#endif

	for(int i=0; i<TESTSIZE; i++){
		//probability 0-9
		probability=distProbability(gen)%10;

		//20% probability insert
		if(probability<INSERT){
			generatedVal=dist(gen);
			sum+=generatedVal;

			user="user"+std::to_string(generatedVal);
			users.push_back(user);
			
			//modify the map
			dt->hmap.accumulate(user, generatedVal);
		}else if(users.size()){
			//80% probability lookup
			//if theres a value in the users vector grab a random one and look for it
			user=users[distProbability(gen)%users.size()];
			
			//if value is not in map exit
			if(dt->hmap.lookup(user, val)==-1){
				#ifdef DEBUG
				dt->log.insert("Error looking up value: "+std::to_string(val)+" on Main");
				dt->log.printLog();
				#endif
				exit(1);
			}
		}
	}

	#ifdef DEBUG
	dt->log.insert("Sum: "+std::to_string(sum)+" from Main");
	#endif

	//push to queue, if it fails exit
	if(dt->queue.push(sum)==-1){
		#ifdef DEBUG
		dt->log.insert("Error adding sum to queue: "+std::to_string(sum)+" from Thread Main");
		dt->log.printLog();
		#endif
		exit(1);
	}

	#ifdef DEBUG
	dt->log.insert("Finished Main Run");
	#endif
}

#endif