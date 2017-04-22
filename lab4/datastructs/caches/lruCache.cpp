
/*************************************\

 lruCache.cpp
 Author: Michael Laucella
 Last Modified: 4/18/17

 implementation for lru cache

\*************************************/

#ifdef LRUCACHE

//constructor
template<typename K, typename V>
LRUCache<K,V>::LRUCache(size_t size):_size(size){
    _count=0;
    pthread_rwlock_init(&lock, nullptr);

    //pre allocate and link the list
    _cache=new List<K,V>[_size]();
    _tail=_cache;
    for(int i=0; i<_size; ++i){
        _tail->next=&_cache[i];
        _tail=_tail->next;
    }
    _head=_cache;
    _tail=nullptr;
}

//destructor
template<typename K, typename V>
LRUCache<K,V>::~LRUCache(){
    pthread_rwlock_destroy(&lock);
    delete []_cache;
}

//lookup value in cache, return the pointer as curr, return prev pointer as prev
template<typename K, typename V>
bool LRUCache<K,V>::lookup(const K key, List<K,V>** prev, List<K,V>** curr){
    pthread_rwlock_rdlock(&lock);
    while(*curr){
        if((*curr)->key==key){
            pthread_rwlock_unlock(&lock);
            return true;
        }
        *prev=*curr;
        *curr=(*curr)->next;
    }
    pthread_rwlock_unlock(&lock);
    return false;
}

//erase item from cache
template<typename K, typename V>
bool LRUCache<K,V>::erase(const K key){
    List<K,V> *prev=nullptr, *curr=_head;

    //if item is in cache
    if(lookup(key, &prev, &curr)){
        pthread_rwlock_wrlock(&lock);
        //if key has already been deleted by another thread;
        if(curr->key!=key){
        	pthread_rwlock_unlock(&lock);
	    	return true;
        }
        //if item hasn't been deleted by another thread, and is not already at the end
        if(_tail!=curr){
            if(prev)
                prev->next=curr->next;
            else
                _head=_head->next;
            curr->next=_tail->next;
            _tail->next=curr;
        }else if(prev)
        	_tail=prev;
        
        curr->key='\0';
        curr->val='\0';
        --_count;
        pthread_rwlock_unlock(&lock);
        return true;
    }
    return false;
}

//get value from cache
template<typename K, typename V>
bool LRUCache<K,V>::get(const K key, V &val){
    List<K,V> *prev=nullptr, *curr=_head;

    //if value is in cache
    if(lookup(key, &prev, &curr)){
        pthread_rwlock_wrlock(&lock);
        val=curr->val;
        //key has been deleted by another thread
        if(curr->key!=key){
        	pthread_rwlock_unlock(&lock);
            return false;
        }
        //if value is not at the end
        if(_tail!=curr){
            if(prev)
                prev->next=curr->next;
            else
                _head=_head->next;
            curr->next=_tail->next;
            _tail->next=curr;
            _tail=curr;
        }
        pthread_rwlock_unlock(&lock);
        return true;
    }
    return false;
}

//update value of element in the list
template<typename K, typename V>
bool LRUCache<K,V>::update(const K key, V val){
    List<K,V> *prev=nullptr, *curr=_head;

    //if value is in the cache
    if(lookup(key, &prev, &curr)){
        pthread_rwlock_wrlock(&lock);
        //if value has been deleted by another thread
        if(curr->key!=key){
        	pthread_rwlock_unlock(&lock);
            return false;
        }
        curr->val=val;
        //if value is not already at the end of the list
        if(_tail!=curr){
            if(prev)
                prev->next=curr->next;
            else
                _head=_head->next;
            curr->next=_tail->next;
            _tail->next=curr;
            _tail=curr;
        }
        pthread_rwlock_unlock(&lock);
        return true;
    }
    return false;
}

//insert a value into the cache
template<typename K, typename V>
bool LRUCache<K,V>::insert(const K key, const V val){
    //check if value is in list and update
    if(update(key, val))
        return true;

    pthread_rwlock_wrlock(&lock);
    //if list is not full append to end
    if(_count<_size){
        if(!_count)
            _tail=_head;
        else
            _tail=_tail->next;
        ++_count;
        _tail->key=key;
        _tail->val=val;
    }
    //else evict head of list and move new value to end
    else{
        _head->val=val;
        _head->key=key;
        _tail->next=_head;
        _tail=_head;
        _head=_head->next;
        _tail->next=nullptr;
    }
    pthread_rwlock_unlock(&lock);

    return true;
}

//get the size of the cache
template<typename K, typename V>
size_t LRUCache<K,V>::getCacheSize() const{
    return _size;
}

//get the cache list
template<typename K, typename V>
List<K,V>* LRUCache<K,V>::getList() const{
    return _head;
}

#endif