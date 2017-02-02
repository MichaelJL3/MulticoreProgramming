
//include guards
#ifndef HASHMAP_H
#define HASHMAP_H

#include <iostream>
#include <functional>

//compile using list or vector
#ifdef USING_LIST
#include <list>
#else
#include <vector>
#endif

//MACROS
#define DEF_MAP_SIZE 50000 //default size of map
 
//the class set (really just a hashmap)
//stores a generic type, using generic keys of the same type;
template<typename T>
class Set{
private:
	//variables when using a list
	#ifdef USING_LIST
	typename std::list<T> *map, local;
	typename std::list<T>::iterator pos;
	//variables when using a vector
	#else
	typename std::vector<T> *map, local;
	int pos;
	#endif

	int map_size, hKey;
public:
	Set();				//default constructor
	~Set();				//destructor
	Set(int size);		//constructor with size
	int hash(T key); 	//hash a generic key into an int
	int insert(T key); 	//insert a value in the map
	int remove(T key); 	//erase a value in the map
	bool find(T key); 	//look for a value in the map
	void printSet(); 	//print all the values in the map
	void eraseSet(); 	//clear all the values in the map
};

//default constructor
template<typename T>
Set<T>::Set(){
	//allocate memory for an array of list* or vector*
	#ifdef USING_LIST
	map = new std::list<T>[DEF_MAP_SIZE];
	#else
	map = new std::vector<T>[DEF_MAP_SIZE];
	#endif

	map_size=DEF_MAP_SIZE;
}

//construct map of certain size
template<typename T>
Set<T>::Set(int size){
	//allocate memory for an array of list* or vector*
	#ifdef USING_LIST
	map = new std::list<T>[size];
	#else
	map = new std::vector<T>[size];
	#endif

	map_size=size;
}

//delete the array of pointers
template<typename T>
Set<T>::~Set(){
	if(map)
		delete []map;
}

//erase all the values in the map
template<typename T>
void Set<T>::eraseSet(){
	for(int i=0; i<map_size; i++){
		map[i].clear();
	}
}

//hash the key into an integer
template<typename T>
int Set<T>::hash(T key){
	//hash the key and mod by the map size to index it in the map
	return std::hash<T>{}(key)%map_size;
}

//insert into the map
template<typename T>
int Set<T>::insert(T key){
	//if the key is not in the map
	if(!find(key)){
		try{
			//key is already hashed in the find call
			//insert the value at the index
			map[hKey].push_back(key);
		} catch(std::bad_alloc& e) {
			//failed to allocate memory
			return -1;			
		}
	}
	return 0;
}

//erase value from the map
template<typename T>
int Set<T>::remove(T key){
	//if the key is in the map
	if(find(key)){
		//the key is already hashed in find
		//the local list is already assigned in find
		//the position is already assigned in find

		#ifdef USING_LIST
		//if its a list delete at the iterator
		local.erase(pos);
		#else
		//if its a vector delete at the distance from the beginning address
		local.erase(local.begin()+pos);
		#endif

		//save the changes back into the map
		map[hKey]=local;
	}
	return 0;
}

//find a key in the map
template<typename T>
bool Set<T>::find(T key){
	//hash the key
	hKey=hash(key);
	//get the list/vector
	local=map[hKey];

	//if using list
	#ifdef USING_LIST
	//look for the value in the list
	typename std::list<T>::iterator end;
	pos=local.begin(), end=local.end();
	for (; pos!=end; pos++) {
    	if(*pos==key){
    		//if the value is found the iterator (pos) is found
    		return true;
    	}
	}
	//else default vector
	#else
	//look for the value in the vector
	int size=local.size();
	for (pos=0; pos<size; pos++){
		if(local[pos]==key){
			//if the value is found the index (pos) is set
			return true;
		}
	}
	#endif
	
	return false;
}

//prints the entire map
template<typename T>
void Set<T>::printSet(){
	for(int i=0; i<map_size; i++){
		local=map[i];
		for(auto &i : local){
			//print every value stored
			std::cout<<i<<"\n";
		}
	}
}

#endif
