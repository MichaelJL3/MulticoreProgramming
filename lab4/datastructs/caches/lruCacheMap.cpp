
/*************************************\

 lruCacheMap.hpp
 Author: Michael Laucella
 Last Modified: 4/22/17

 skeletal layout for LRU cache scheme

 the cache is implemented using a map
 and is thread safe

\*************************************/

//constructor
template<typename K, typename V>
LRUCacheMap<K,V>::LRUCacheMap(size_t size):_size(size){
	pthread_rwlock_init(&lock, nullptr);
	pthread_mutex_init(&timelock, nullptr);
    cache.reserve(size);
}

//destructor
template<typename K, typename V>
LRUCacheMap<K,V>::~LRUCacheMap(){
	pthread_rwlock_destroy(&lock);
	pthread_mutex_destroy(&timelock);
}

//insert a value into the cache
template<typename K, typename V>
void LRUCacheMap<K,V>::insert(const K key, const V val){
	//if cache is full evict oldest member
	evict();
	
	//insert new value into cache
	pthread_rwlock_wrlock(&lock);
	cache[key]=std::make_pair(val, ++_time);
	pthread_rwlock_unlock(&lock);
} 

//evict oldest member from cache
//function is private(internal) is locked from its caller
template<typename K, typename V>
void LRUCacheMap<K,V>::evict(){
    auto it=cache.begin();
    auto era=it;
    size_t min=0;

    pthread_rwlock_rdlock(&lock);
	if(cache.size()==_size){
	    //search cache for oldest member
		for(; it!=cache.end(); ++it){
			if(it->second.second<min){
				min=it->second.second;
				era=it;
			}
		}
	}else{
		pthread_rwlock_unlock(&lock);
		return;	
	}
	pthread_rwlock_unlock(&lock);

	//erase oldest member
	pthread_rwlock_wrlock(&lock);
	cache.erase(era);
	pthread_rwlock_unlock(&lock);
}

//erase value from cache
template<typename K, typename V>
bool LRUCacheMap<K,V>::erase(const K key){
	pthread_rwlock_wrlock(&lock);
	++_time;
	cache.erase(key);
	pthread_rwlock_unlock(&lock);
	return true;
}

//get value from cache
template<typename K, typename V>
bool LRUCacheMap<K,V>::get(const K key, V &val){
	pthread_rwlock_rdlock(&lock);
	//check if value is in cache
	if(cache.find(key)!=cache.end()){
		val=cache[key].first;
		pthread_mutex_lock(&timelock);
		cache[key].second=++_time;
		pthread_mutex_unlock(&timelock);
		pthread_rwlock_unlock(&lock);
		return true;
	}
	pthread_rwlock_unlock(&lock);
	return false;
}

//get cache map (for printing purposes)
template<typename K, typename V>
std::unordered_map<K,std::pair<V,size_t>> LRUCacheMap<K,V>::getMap() const {
    return cache;
}
