
#ifdef LRUCACHE

template<typename K, typename V>
LRUCache<K,V>::LRUCache(size_t size):_size(size){
    _count=0;
    pthread_rwlock_init(&lock, nullptr);

    _cache=new List<K,V>[_size]();
    _tail=_cache;
    for(int i=0; i<_size; ++i){
        _tail->next=&_cache[i];
        _tail=_tail->next;
    }
    _head=_cache;
    _tail=nullptr;
}

template<typename K, typename V>
LRUCache<K,V>::~LRUCache(){
    pthread_rwlock_destroy(&lock);
    delete []_cache;
}

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

template<typename K, typename V>
bool LRUCache<K,V>::erase(const K key){
    List<K,V> *prev=nullptr, *curr=_head;

    if(lookup(key, &prev, &curr)){
        pthread_rwlock_wrlock(&lock);
        if(_tail!=curr&&curr->key==key){
            if(prev)
                prev->next=curr->next;
            else
                _head=_head->next;
            curr->next=_tail->next;
            _tail->next=curr;
        }
        --_count;
        curr->key=0;
        curr->val=0;
        pthread_rwlock_unlock(&lock);
        return true;
    }
    return false;
}

template<typename K, typename V>
bool LRUCache<K,V>::get(const K key, V &val){
    List<K,V> *prev=nullptr, *curr=_head;

    if(lookup(key, &prev, &curr)){
        pthread_rwlock_wrlock(&lock);
        val=curr->val;
        if(_tail!=curr&&curr->key==key){
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

template<typename K, typename V>
bool LRUCache<K,V>::update(const K key, V val){
    List<K,V> *prev=nullptr, *curr=_head;

    if(lookup(key, &prev, &curr)){
        pthread_rwlock_wrlock(&lock);
        curr->val=val;
        if(_tail!=curr&&curr->key==key){
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

template<typename K, typename V>
bool LRUCache<K,V>::insert(const K key, const V val){
    if(update(key, val))
        return true;

    pthread_rwlock_wrlock(&lock);
    if(_count<_size){
        if(!_count)
            _tail=_head;
        else
            _tail=_tail->next;
        ++_count;
        _tail->key=key;
        _tail->val=val;
    }else{
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

template<typename K, typename V>
size_t LRUCache<K,V>::getCacheSize() const{
    return _size;
}

template<typename K, typename V>
List<K,V>* LRUCache<K,V>::getList() const{
    return _head;
}

#endif