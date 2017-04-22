
#pragma once

/*************************************\

 cacheADT.hpp
 Author: Michael Laucella
 Last Modified: 4/18/17

 Framework for caching algorithms

\*************************************/

#include <cstdlib>
#include <sstream>

//cache storage extend for custom cache storage
template<typename K, typename V>
struct List{
    K key;
    V val;
    List<K,V> *next;
};

//cache skeleton
template<typename K, typename V>
class Cache{
    //must be able to perform internal lookup
    virtual bool lookup(const K key, List<K,V>** prev, List<K,V>** curr)=0;
public:
    //must be able to retrieve value from cache
    virtual bool get(const K key, V &val)=0;
    //must be able to update the value within the cache
    virtual bool update(const K key, const V val)=0;
    //must be able to insert a value into the cache
    virtual bool insert(const K key, const V val)=0;
    //must be able to delete a value from the cache
    virtual bool erase(const K key)=0;

    //must be able to get the cache list (for printing purposes)
    virtual List<K,V>* getList() const=0;
};

//print out cache contents 
template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const Cache<K,V>& cache){
    List<K,V> *temp;

    temp=cache.getList();
    while(temp){
        os<<temp->key<<": "<<temp->val<<"\n";
        temp=temp->next;
    }

    return os;
}