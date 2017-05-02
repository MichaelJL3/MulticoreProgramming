
#pragma once

/*************************************\

 lruCacheMap.hpp
 Author: Michael Laucella
 Last Modified: 4/22/17

 skeletal layout for LRU cache scheme

 the cache is implemented using a map
 and is thread safe

\*************************************/

#include <unordered_map>
#include <utility>
#include <pthread.h>
#include <cstdlib>

template<typename K, typename V>
class LRUCacheMap{
	pthread_mutex_t lock;
	std::unordered_map<K,std::pair<V,size_t>> cache;
	std::map<size_t,K> times;
	size_t _time;
	const size_t _size;

	void evict();
	LRUCacheMap();
public:
	LRUCacheMap(size_t size);
	~LRUCacheMap();
	void insert(const K key, const V val);
	bool get(const K key, V& val);
	bool erase(const K key);
    std::unordered_map<K,std::pair<V,size_t>> getMap() const;
};

#include "lruCacheMap.cpp"

//print the cache
template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const LRUCacheMap<K,V>& cache){
    auto temp=cache.getMap();
    auto it=temp.begin();

    for(; it!=temp.end(); ++it){
        os<<it->first<<": "<<it->second.first<<" @: "<<it->second.second<<"\n";
    }

    return os;
}
