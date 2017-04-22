
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
	pthread_mutex_init(&lock, nullptr);
    cache.reserve(size);
}

//destructor
template<typename K, typename V>
LRUCacheMap<K,V>::~LRUCacheMap(){
	pthread_mutex_destroy(&lock);
}

//insert a value into the cache
template<typename K, typename V>
void LRUCacheMap<K,V>::insert(const K key, const V val){
	pthread_mutex_lock(&lock);
	//if cache is full evict oldest member
	if(cache.size()==_size)
		evict();
	
	//insert new value into cache
	cache[key]=std::make_pair(val, ++_time);
	pthread_mutex_unlock(&lock);
} 

//evict oldest member from cache
//function is private(internal) is locked from it is locked from its caller
template<typename K, typename V>
void LRUCacheMap<K,V>::evict(){
    auto it=cache.begin();
    auto era=it;
    size_t min=0;

    //search cache for oldest member
	for(; it!=cache.end(); ++it){
		if(it->second.second<min){
			min=it->second.second;
			era=it;
		}
	}

	//erase oldest member
	cache.erase(era);
}

//erase value from cache
template<typename K, typename V>
bool LRUCacheMap<K,V>::erase(const K key){
	pthread_mutex_lock(&lock);
	++_time;
	cache.erase(key);
	pthread_mutex_unlock(&lock);
	return true;
}

//get value from cache
template<typename K, typename V>
bool LRUCacheMap<K,V>::get(const K key, V &val){
	pthread_mutex_lock(&lock);
	//check if value is in cache
	if(cache.find(key)!=cache.end()){
		val=cache[key].first;
		cache[key].second=++_time;
		pthread_mutex_unlock(&lock);
		return true;
	}
	pthread_mutex_unlock(&lock);
	return false;
}

//get cache map (for printing purposes)
template<typename K, typename V>
std::unordered_map<K,std::pair<V,size_t>> LRUCacheMap<K,V>::getMap() const {
    return cache;
}
