
#pragma once

/*************************************\

 lruCache.hpp
 Author: Michael Laucella
 Last Modified: 4/18/17

 skeletal layout for LRU cache scheme
 on insert items are checked for prior
 existence if they exist the cache 
 pointer is updated and nothing happens

 the head pointer denotes the least
 recently used item and the tail denotes
 the most recently used

 the cache is implemented using a pre
 allocated set of list pointers
 that are re-used

\*************************************/

#define LRUCACHE

#include "cacheADT.hpp"
#include <unordered_map>

//LRU cache class extends cache framework
template<typename K, typename V>
class LRUCache : public Cache<K,V>{
    pthread_rwlock_t lock;
    List<K,V> *_cache, *_tail, *_head;
    const size_t _size;
    size_t _count;
    
    LRUCache();
    virtual bool lookup(const K key, List<K,V>** prev, List<K,V>** curr) override;
public:
    LRUCache(const size_t size);
    ~LRUCache();

    virtual bool get(const K key, V &val) override;             //get value from cache
    virtual bool update(const K key, const V val) override;     //update value in cache
    virtual bool insert(const K key, const V val) override;     //insert value into cache
    virtual bool erase(const K key) override;                   //erase value from cache
    virtual List<K,V>* getList() const override;                //get the cache list

    size_t getCacheSize() const;    //get cache size
};

#include "lruCache.cpp"