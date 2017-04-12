
#ifndef LOCK_FREE_QUEUE
#define LOCK_FREE_QUEUE

const int ERR=-1;

#ifdef WIN32
#include <windows.h>
#define TIMEOUT 250
#else 
#include <unistd.h>
#define TIMEOUT 250000
#endif

void sleep(){
	#ifdef WIN32
	Sleep(TIMEOUT);
	#else
	usleep(TIMEOUT);
	#endif
}

template <typename T>
struct Node{
	T value;
	Node<T>* next;
};

template <typename T>
class LockFreeQueue{
	Node<T>* queue;
	Node<T>* head, * tail;
public:
	LockFreeQueue();
	~LockFreeQueue();
	void push(T &val);
	T listen();
	T dequeue();
};

#include "LockFreeQueue.cpp"

#endif
