
#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <semaphore.h>
#include <list>

/*************************************************************\

Class: ThreadSafeListenerQueue
Author: Michael Laucella
Last Modified: 2/16/2017

!!OS specific requires posix libraries

creates a thread safe queue using two locks which can operate 
in parallel as long as the queue has size
the size is denoted by a semaphore value

class supports templates

T = type of variable stored in queue

\*************************************************************/

template<typename T>
class ThreadSafeListenerQueue{
	std::list<T> queue;
	pthread_mutex_t enque;
	pthread_mutex_t deque;
	sem_t count;
public:
	ThreadSafeListenerQueue();	//constructor
	~ThreadSafeListenerQueue();	//destructor
	int push(const T element);	//push an element on the stack
	int pop(T& element);		//pop an element off the stack
	int listen(T& element);		//listen
};

/*
constructor

initializes mutex locks, condition, and semaphore
*/
template<typename T>
ThreadSafeListenerQueue<T>::ThreadSafeListenerQueue(){
	pthread_mutex_init(&enque, NULL);
	pthread_mutex_init(&deque, NULL);	
	sem_init(&count, 0, 0);
}

/*
destructor

destroys mutex locks, condition, and semaphore
*/
template<typename T>
ThreadSafeListenerQueue<T>::~ThreadSafeListenerQueue(){
	pthread_mutex_destroy(&enque);
	pthread_mutex_destroy(&deque);
	sem_destroy(&count);
}

/*
push(T)

locks and pushes an element to the queue
increments the semaphore
sends a signal to listener

returns -1 if memory errors occur
else return 0;
*/
template<typename T>
int ThreadSafeListenerQueue<T>::push(const T element){
	if(pthread_mutex_lock(&enque))
		return -1;
	try{
		queue.push_back(element);
		sem_post(&count);
		if(pthread_mutex_unlock(&enque))
			return -1;
		return 0;
	} catch(std::bad_alloc& e) {
		if(pthread_mutex_unlock(&enque))
			return -1;
		return -1;
	}
}

/*
pop(&T)

locks and grabs the element from the front of the queue
checks if the queue has elements through semaphore value

returns 1 if queue is empty
else return 0;
*/
template<typename T>
int ThreadSafeListenerQueue<T>::pop(T& element){
	if(sem_trywait(&count)){
		return 1;
	}

	if(pthread_mutex_lock(&deque))
		return -1;
	element=queue.front();
	queue.pop_front();
	if(pthread_mutex_unlock(&deque))
		return -1;

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
	while(sem_wait(&count)){}
	
	if(pthread_mutex_lock(&deque))
		return -1;
	element=queue.front();
	queue.pop_front();
	if(pthread_mutex_unlock(&deque))
		return -1;
	return 0;
}
 
#endif