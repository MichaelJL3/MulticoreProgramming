
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
void LockFreeQueue<T>::push(T val){
	Node<T>* currTail;
	Node<T>* node=new Node<T>;
	node->next=nullptr;
	node->value=val;
	
	while(1){
		currTail=tail;
		
		if(currTail==tail){
			if(!(currTail->next)){
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
void LockFreeQueue<T>::dequeue(T& value){
	Node<T> *currHead, *currTail;

	while(1){
		currHead=head;
		currTail=tail;

		if(currHead==currTail){
			if(currHead->next==nullptr){
				value=ERR;
				return;
			}
			__sync_val_compare_and_swap(&tail, currTail, currHead->next);
		}
		else{
			if(__sync_bool_compare_and_swap(&head, currHead, currHead->next)){
				value=currHead->next->value;
				delete currHead;
				break;
			}
		}
	}
}

template <typename T>
void LockFreeQueue<T>::listen(T& value){
	while(1){
		if(head==tail){
			sleep();	
		}
		
		dequeue(value);
		if(value!=ERR)
			break;
	}
}

