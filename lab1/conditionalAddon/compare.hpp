
#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <cstdio>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <unordered_map>

void compare(int numThreads, double duration);
double serialTest(int n);

void compare(int numThreads, double duration){
	double speedup;
	double efficiency;
	double serial=serialTest(numThreads);
	speedup=serial/duration;
	if(serial<duration)
		speedup=0-speedup;
	efficiency=speedup/numThreads;
	printf("Speedup from T(1) to T(%d) = %f\nEfficiency = %f\nTestsize of %ld\n", numThreads, speedup, efficiency, (long)TESTSIZE*numThreads);
}

//Serial version of the program 
//returns duration of serial version
double serialTest(int n){
	clock_t st=clock(), en;
	srand(time(NULL));

	std::unordered_map<std::string, int32_t> umap;
	std::vector<std::string> users;
	
	std::default_random_engine gen(time(NULL));
	std::uniform_int_distribution<int32_t> dist(MIN, MAX);

	std::string user;

	double runtime;
	long testsize=n*TESTSIZE;
	int probability, value, sum=0;

	for(long i=0; i<testsize; i++){
		probability=rand()%10;
		
		if(probability<INSERT){
			value=dist(gen);
			sum+=value;
			user="user"+std::to_string(value);
			umap[user]+=value;
		}else if(users.size()){
			user=users[rand()%users.size()];
			if(umap.find(user)==umap.end()){
				exit(1);
			}
		}
	}

	printf("Sum of Sequential: %d\n", sum);

	en=clock();
	runtime=((double)en-st)/CLOCKS_PER_SEC;
	printf("Sequential runtime: %f\n", runtime);

	return runtime;
}

#endif