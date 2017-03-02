
/*
    can only enter both sections if this is being
    included through lab1 it uses all the same structures
    and includes
*/

#ifndef EFFICIENT_HPP
#ifdef LAB1_HPP
#define EFFICIENT_HPP

//if efficient mode is enabled define the function for main
//to run (only difference between the original is the call to pthread_exit)

//same as above test(void* data)
void mainRunTest(void* data){
	//starting time
	clock_t st=clock(), en;

	dataStruct *dt=(dataStruct*) data;

	int32_t probability, generatedVal, val, sum=0;
	pthread_t thread=pthread_self();
	
	std::vector<std::string> users;
	std::string user;

	//random generators
	std::default_random_engine gen(time(NULL)*thread);
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
			user="user"+std::to_string(generatedVal);
			
			//modify the map
			if(dt->hmap.accumulate(user, generatedVal)==-1){
				#ifdef DEBUG
				dt->log.insert("Error inserting value: mutex lock failed");
				dt->log.printLog();
				#endif
				exit(1);
			}

			users.push_back(user);
			sum+=generatedVal;
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

	//end time
	en=clock();

	dt->timeMutex.lock();
	dt->times.push_back(((double)en-st)/CLOCKS_PER_SEC);
	if(en>dt->maxTime)
		dt->maxTime=en;
	dt->timeMutex.unlock();
}

#endif
#endif