
#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <list>

/*************************************************************\

Class: ThreadSafeListenerQueue
Author: Michael Laucella
Last Modified: 2/16/2017

!!OS specific requires posix libraries

creates a thread safe queue using a single lock

class supports templates

T = type of variable stored in queue

\*************************************************************/

template<typename T>
class ThreadSafeListenerQueue{
	std::list<T> queue;
	pthread_mutex_t lock;
	pthread_cond_t cond;
public:
	ThreadSafeListenerQueue();	//constructor
	~ThreadSafeListenerQueue();	//destructor
	int push(const T element);	//push an element on the stack
	int pop(T& element);		//pop an element off the stack
	int listen(T& element);		//listen
};

/*
constructor

initializes mutex lock and condition
*/
template<typename T>
ThreadSafeListenerQueue<T>::ThreadSafeListenerQueue(){
	pthread_mutex_init(&lock, NULL);	pthread_cond_init(&cond, NULL);
}

/*
destructor

destroys mutex lock and condition
*/
template<typename T>
ThreadSafeListenerQueue<T>::~ThreadSafeListenerQueue(){
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);
}

/*
push(T)

locks and pushes an element to the queue
sends a signal to listener

returns -1 if memory errors occur
else return 0;
*/
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

/*
pop(&T)

locks and grabs element from front of queue

returns 1 if queue is empty
else return 0;
*/
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

/*
listen(&T)

locks and attempts to grab value from front of queue
if it fails will continue until a signal from push is received
will continue to try until it gets a value 

!!careful not to cause deadlocks using this function 
*/
template<typename T>
int ThreadSafeListenerQueue<T>::listen(T& element){
	pthread_mutex_lock(&lock);
	while(!queue.size()){
		pthread_cond_wait(&cond, &lock);
	}

	element=queue.front();
	queue.pop_front();
	pthread_mutex_unlock(&lock);

	return 0;
}
 
#endif