
#pragma once

#include <cstdlib>
#include <sstream>

template<typename K, typename V>
struct List{
    K key;
    V val;
    List<K,V> *next;
};

template<typename K, typename V>
class Cache{
    virtual bool lookup(const K key, List<K,V>** prev, List<K,V>** curr)=0;
public:
    virtual bool get(const K key, V &val)=0;
    virtual bool update(const K key, const V val)=0;
    virtual bool insert(const K key, const V val)=0;
    virtual bool erase(const K key)=0;
    virtual List<K,V>* getList() const=0;
};

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