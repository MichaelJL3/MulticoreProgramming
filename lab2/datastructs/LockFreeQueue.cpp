
template <typename T>
LockFreeQueue<T>::LockFreeQueue(){
	head=new Node<T>;
	head->next=nullptr;
	tail=head;
}

template <typename T>
LockFreeQueue<T>::~LockFreeQueue(){
	Node<T>* temp;
	while(head){
		temp=head;
		head=head->next;
		delete temp;
	}
}

template <typename T>
void LockFreeQueue<T>::enqueue(T& val){
	Node<T>* currTail;
	Node<T>* node=new Node<T>;
	node->next=nullptr;
	node->value=val;
	
	while(1){
		currTail=tail;
		
		if(currTail==tail){
			if(!currTail->next){
				if(__sync_bool_compare_and_swap(&(tail->next), currTail->next, node)){
					break;
				}
			}else{
				__sync_val_compare_and_swap(&tail, currTail , node);
			}
		}
	}
	
	__sync_val_compare_and_swap(&tail, currTail, node);
}

template <typename T>
T LockFreeQueue<T>::dequeue(){
	Node<T> *currHead, *currTail;
	T val;

	while(1){
		val=head->next->value;
		currHead=head;
		currTail=tail;

		if(currHead==currTail){
			if(currHead->next==nullptr){
				return ERR;
			}
			__sync_val_compare_and_swap(&tail, currTail, currHead->next);
		}
		else{
			if(__sync_bool_compare_and_swap(&head, currHead, currHead->next)){
				break;
			}
		}
	}

	delete currHead;
	return val;
}

template <typename T>
T LockFreeQueue<T>::listenDequeue(){
	T val;

	while(1){
		if(head==tail){
			sleep();	
		}
		
		val=dequeue();
		if(val!=ERR)
			return val;
	}
}

