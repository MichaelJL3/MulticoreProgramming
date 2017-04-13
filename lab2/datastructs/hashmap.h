
#ifndef HASHMAP_H
#define HASHMAP_H

#include <pthread.h>
#include <unordered_map>

/*************************************************************\

Class: ThreadSafeKVStore
Author: Michael Laucella
Last Modified: 2/16/2017

!!OS specific requires posix libraries

creates a thread safe unordered_map using reader/writer locks

class supports templates

K = key to store value
V = value to store/retrieve

\*************************************************************/

template<typename K, typename V>
class ThreadSafeKVStore{
    std::unordered_map<K, V> hashmap;
    #ifndef BIGLOCK
    pthread_rwlock_t lock;
    #else
    pthread_mutex_t lock;
    #endif
public:
    ThreadSafeKVStore();					//constructor
    ~ThreadSafeKVStore();					//destructor
    int insert(const K k, const V v);		//inserts new item
    int accumulate(const K k, const V v);	//adds to item
    int lookup(const K k, V& v);			//lookup an item
    int remove(const K k);					//remove an item
    int sum(int &val);						//sums all the elements
};

/*
constructor

initializes reader/writer lock
*/
template<typename K, typename V>
ThreadSafeKVStore<K, V>::ThreadSafeKVStore(){
    #ifndef BIGLOCK
	pthread_rwlock_init(&lock, NULL);
    #else
    pthread_mutex_init(&lock, NULL);
    #endif
}

/*
destructor

destroys reader/writer lock
*/
template<typename K, typename V>
ThreadSafeKVStore<K, V>::~ThreadSafeKVStore(){
    #ifndef BIGLOCK
	pthread_rwlock_destroy(&lock);
    #else
    pthread_mutex_destroy(&lock);
    #endif
}

/*
insert(K, V)

locks and inserts new value
uses write lock as map is modified
*/
template<typename K, typename V>
int ThreadSafeKVStore<K, V>::insert(const K k, const V v){
    #ifndef BIGLOCK
    if(pthread_rwlock_wrlock(&lock))
    	return -1;
    hashmap[k]=v;
    if(pthread_rwlock_unlock(&lock))
    	return -1;
    #else
    if(pthread_mutex_lock(&lock))
    	return -1;
    hashmap[k]=v;
    if(pthread_mutex_unlock(&lock))
    	return -1;
    #endif

    return 0;
}

/*
accumulate(K, V)

locks and modifies value in map
uses write lock as map is modified
*/
template<typename K, typename V>
int ThreadSafeKVStore<K, V>::accumulate(const K k, const V v){
    #ifndef BIGLOCK
    if(pthread_rwlock_wrlock(&lock))
    	return -1;
    hashmap[k]+=v;
    if(pthread_rwlock_unlock(&lock))
    	return -1;
    #else
    if(pthread_mutex_lock(&lock))
    	return -1;
    hashmap[k]+=v;
    if(pthread_mutex_unlock(&lock))
    	return -1;
    #endif

    return 0;
}

/*
remove(K)

locks and removed value in map if exists
(if doesnt exist erase returns no error)
uses write lock as map is modified
*/
template<typename K, typename V>
int ThreadSafeKVStore<K, V>::remove(const K k){
    #ifndef BIGLOCK
    if(pthread_rwlock_wrlock(&lock))
		return -1;
    hashmap.erase(k);
    if(pthread_rwlock_unlock(&lock))
    	return -1;
    #else
    if(pthread_mutex_lock(&lock))
		return -1;
    hashmap.erase(k);
    if(pthread_mutex_unlock(&lock))
    	return -1;
    #endif

    return 0;
}

/*
lookup(K, &V)

locks and reads value in map
stores the value in v

if not in map return -1
else retuen 0

uses read lock as map is not modified
*/
template<typename K, typename V>
int ThreadSafeKVStore<K, V>::lookup(const K k, V& v){
    #ifndef BIGLOCK
    if(pthread_rwlock_rdlock(&lock))
    	return -1;
    if(hashmap.find(k)==hashmap.end()){
        if(pthread_rwlock_unlock(&lock))
        	return -1;
        return -1;
    }
    v=hashmap[k];
    if(pthread_rwlock_unlock(&lock))
    	return -1;
    #else
    if(pthread_mutex_lock(&lock))
    	return -1;
    if(hashmap.find(k)==hashmap.end()){
        if(pthread_mutex_unlock(&lock))
        	return -1;
        return -1;
    }
    v=hashmap[k];
    if(pthread_mutex_unlock(&lock))
    	return -1;
    #endif

    return 0;
}

/*
sum()

returns sum of the values in map
locks while reading values
uses read lock
*/
template<typename K, typename V>
int ThreadSafeKVStore<K, V>::sum(int &val){
    int sum=0;
    #ifndef BIGLOCK
    if(pthread_rwlock_rdlock(&lock))
    	return -1;
    for(auto it=hashmap.begin(); it!=hashmap.end(); it++){
        sum+=it->second;
    }
    val=sum;
    if(pthread_rwlock_unlock(&lock))
    	return -1;
    #else
    if(pthread_mutex_lock(&lock))
    	return -1;
    for(auto it=hashmap.begin(); it!=hashmap.end(); it++){
        sum+=it->second;
    }
    val=sum;
    if(pthread_mutex_unlock(&lock))
    	return -1;
    #endif

    return 0;
}

#endif