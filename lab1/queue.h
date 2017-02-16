
#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <list>

template<typename T>
class ThreadSafeListenerQueue{
	std::list<T> queue;
	pthread_mutex_t lock;
	pthread_cond_t cond;
public:
	ThreadSafeListenerQueue();
	~ThreadSafeListenerQueue();
	int push(const T element);
	int pop(T& element);
	int listen(T& element);
};

template<typename T>
ThreadSafeListenerQueue<T>::ThreadSafeListenerQueue(){
	pthread_mutex_init(&lock, NULL);
}

template<typename T>
ThreadSafeListenerQueue<T>::~ThreadSafeListenerQueue(){
	pthread_mutex_destroy(&lock);
}

template<typename T>
int ThreadSafeListenerQueue<T>::push(const T element){
	pthread_mutex_lock(&lock);
	try{
		queue.push_back(element);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&lock);
		return 0;
	} catch(std::bad_alloc& e) {
		pthread_mutex_unlock(&lock);
		return -1;
	}
}

template<typename T>
int ThreadSafeListenerQueue<T>::pop(T& element){
	pthread_mutex_lock(&lock);
	if(!queue.size()){
		pthread_mutex_unlock(&lock);
		return 1;
	}

	element=queue.front();
	queue.pop_front();
	pthread_mutex_unlock(&lock);

	return 0;
}

template<typename T>
int ThreadSafeListenerQueue<T>::listen(T& element){
	pthread_mutex_lock(&lock);
	while(!queue.size()){
		//err??
		pthread_cond_wait(&cond, &lock);
	}

	element=queue.front();
	queue.pop_front();
	pthread_mutex_unlock(&lock);

	return 0;
}
 
#endif