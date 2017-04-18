
#pragma once
#define LRUCACHE

#include "cacheADT.hpp"
#include <unordered_map>

//comment out
//work out hash cache details

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

    virtual bool get(const K key, V &val) override;
    virtual bool update(const K key, const V val) override;
    virtual bool insert(const K key, const V val) override;
    virtual bool erase(const K key) override;
    virtual List<K,V>* getList() const override;

    size_t getCacheSize() const;
};

#include "lruCache.cpp"