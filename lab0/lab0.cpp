
#include <vector>
#include <random>
#include <cstdio>
#include "hashmap.h"

using namespace std;

//MACROS
#define TESTS 10 //number of tests
#define LOOP 100 //size of insertion loop per test
#define MIN 0 	 //minimum value for int generator
#define MAX 200  //maximum value for int generator

//a complete test
void thoroughTest(int tests, int iter);

//a test with certain shortcuts
void lazyTest(int tests, int iter);

int main(){

	//default run the thorough test
	#ifdef LAZY
	lazyTest(TESTS, LOOP);
	#else
	thoroughTest(TESTS, LOOP);
	#endif
	
	return 0;
}

/*
	lazyTest
	int tests - number of tests
	int iter - insertions/lookups per test

	inserts a value then immediately checks if it finds it
	if it doesn't find the value the test fails and it moves
	to the next test

	the test is called lazy because it only checks for the value
	right after it has been inserted and so if there were an 
	error where an old value were overwritten it would never be
	detected
*/
void lazyTest(int tests, int iter){
	default_random_engine gen;
	uniform_int_distribution<int> dist(MIN, MAX);
	Set<int> testSet;
	int val, failed=0;

	for(int i=0; i<tests; i++){
		for(int j=0; j<iter; j++){
			//generate random number
			val=dist(gen);

			//if the insertion fails
			if(testSet.insert(val)==-1){
				//debug messages
				#ifdef INFORM
				printf("Failed on lazyTest()\ninserting value: %d\n", val);
				#endif
				
				printf("Test %d Failed\n", i);
				failed++;
				break;
			}

			//if it fails to find the inserted value
			if(!testSet.find(val)){
				//debug messages
				#ifdef INFORM
				printf("Failed on lazyTest()\nfinding value: %d\n", val);
				testSet.printSet();
				#endif
				
				printf("Test %d Failed\n", i);
				failed++;
				break;
			}
		}

		//empties the map
		testSet.eraseSet();
	}

	//print results of the test
	printf("Total Tests: %d\nFailed: %d\nSuccess Rate: %.2f\n", tests, failed, (double)(tests-failed)/tests);
}

/*
	thoroughTest
	int tests - number of tests
	int iter - insertions/lookups per test

	inserts all the generated values in the set
	and a vector, if any point fails the whole test
	fails and the next test begins.

	after insertion all the previously inserted values
	are looked for if it fails to find one then the 
	test fails
*/
void thoroughTest(int tests, int iter){
	default_random_engine gen;
	uniform_int_distribution<int> dist(MIN, MAX);
	Set<int> testSet;
	vector<int> savedItems(iter);
	int val, failed=0;
	bool skip;

	for(int i=0; i<tests; i++){
		skip=false;
		for(int j=0; j<iter; j++){
			//generate a random integer
			val=dist(gen);
			savedItems[j]=val;

			//if insertion fails
			if(testSet.insert(val)==-1){
				//debug information
				#ifdef INFORM
				printf("Failed on thoroughTest()\ninserting value: %d\n", val);
				#endif
				
				printf("Test %d Failed\n", i);
				failed++;
				skip=true;
				break;
			}
		}

		//if it hasn't already failed on an insert
		if(!skip){
			//check that all the inserted values can be found in the set
			for(int j=0; j<iter; j++){
				val=savedItems[j];
				//if its not found
				if(!testSet.find(val)){
					//debug information
					#ifdef INFORM
					printf("Failed on thoroughTest()\nfinding value: %d\n", val);
					testSet.printSet();
					#endif

					printf("Test %d Failed\n", i);
					failed++;
					break;
				}
			}
		}

		//empties the map
		testSet.eraseSet();
	}

	//print results of the test
	printf("Total Tests: %d\nFailed: %d\nSuccess Rate: %.2f\n", tests, failed, (double)(tests-failed)/tests);
}