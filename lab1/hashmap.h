
#ifndef HASHMAP_H
#define HASHMAP_H

#include <pthread.h>
#include <unordered_map>

template<typename K, typename V>
class ThreadSafeKVStore{
    std::unordered_map<K, V> hashmap;
    pthread_rwlock_t lock;
public:
    ThreadSafeKVStore();
    ~ThreadSafeKVStore();
    int insert(const K k, const V v);
    int accumulate(const K k, const V v);
    int lookup(const K k, V& v);
    int remove(const K k);
    int sum();
};

template<typename K, typename V>
ThreadSafeKVStore<K, V>::ThreadSafeKVStore(){
	pthread_rwlock_init(&lock, NULL);
}

template<typename K, typename V>
ThreadSafeKVStore<K, V>::~ThreadSafeKVStore(){
	pthread_rwlock_destroy(&lock);
}

template<typename K, typename V>
int ThreadSafeKVStore<K, V>::insert(const K k, const V v){
    pthread_rwlock_wrlock(&lock);
    hashmap[k]=v;
    pthread_rwlock_unlock(&lock);

    return 0;
}

template<typename K, typename V>
int ThreadSafeKVStore<K, V>::accumulate(const K k, const V v){
    pthread_rwlock_wrlock(&lock); 
    hashmap[k]+=v;
    pthread_rwlock_unlock(&lock);

    return 0;
}

template<typename K, typename V>
int ThreadSafeKVStore<K, V>::lookup(const K k, V& v){
    pthread_rwlock_rdlock(&lock);
    if(hashmap.find(k)==hashmap.end()){
        pthread_rwlock_unlock(&lock);
        return -1;
    }
    v=hashmap[k];
    pthread_rwlock_unlock(&lock);

    return 0;
}

template<typename K, typename V>
int ThreadSafeKVStore<K, V>::remove(const K k){
    pthread_rwlock_wrlock(&lock);
    hashmap.erase(k);
    pthread_rwlock_unlock(&lock);

    return 0;
}

template<typename K, typename V>
int ThreadSafeKVStore<K, V>::sum(){
    int sum=0;
    pthread_rwlock_rdlock(&lock);
    for(auto it=hashmap.begin(); it!=hashmap.end(); it++){
        sum+=it->second;
    }
    pthread_rwlock_unlock(&lock);

    return sum;
}

#endif