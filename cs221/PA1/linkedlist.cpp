#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include "linkedlist.h"

// helper function for deep copy
// Used by copy constructor and operator
// should create new Kebab objects
void LinkedList::copyList(const LinkedList& ll){
	
	Node* copyhead = ll.front;

	//if the list passed in is a empty one, then return the empty list
	if(copyhead == NULL) {
		
		return;
	}

	//if the linkedlist contains at least one Node
	Node* curr = new Node(copyhead->data);
	
	front = curr;
	
	length++;

	while (copyhead->next != NULL) {
		
		curr->next = new Node(copyhead->next->data);
		
		curr = curr->next;
		
		copyhead = copyhead->next;
		
		length++;
	}

	back = curr;
	
	return;
}
  
// helper function for deep delete
// Used by destructor and copy/assignment
// Deallocates Nodes and the Kebabs inside
void LinkedList::deleteList(){
	
	Node* curr = front;
	
	while( curr != NULL ) {
    	
    	Node* next = curr->next;
    	
    	delete curr;
    	
    	curr = next;
	}
	
	front = NULL;
	
	back = NULL;
	
	length = 0;
}


// default constructor
LinkedList::LinkedList(){
    
    length = 0;
	
	front = NULL;
	
	back = NULL;
}

// copy constructor, performs deep copy of list elements
LinkedList::LinkedList(const LinkedList& ll){
	
	copyList(ll);
}

// destructor
LinkedList::~LinkedList(){
	
	deleteList();
}

// MUTATORS

// Inserts an item at the front of the list
// POST:  List contains item at position 0
// PARAM: item = item to be inserted
void LinkedList::insertFront(Kebab* item){
	
	Node* tmp = new Node(item);
	
	tmp->next = front;
	
	front = tmp;
	
	length++;

	//if it was an empty list then set back = front

	if(back == NULL) {
		
		back = tmp;
	}

}

// Inserts an item at the back of the list
// POST:  List contains item at back
// PARAM: item = item to be inserted
void LinkedList::insertBack(Kebab* item){
	
	Node* tmp = new Node(item);
	
	length++;
	
	if(front == NULL) {
		
		front = tmp;
		
		back = tmp;
		
		return;
	}

	back->next = tmp;
	
	back = tmp;

	return;
}

// Inserts an item in position p (0-indexed)
// Returns false for invalid index
// PRE:   0 <= p <= size
// POST:  List contains item at position p
// PARAM: item = item to be inserted, p = position where item will be inserted
bool LinkedList::insertAt(Kebab* item, unsigned int p){
	
	if( p > length){
		
		return false;
	}

	//insert front
	if(p == 0) {
		
		insertFront(item);
		
		return true;
	}

	//insert back
	if(p == length) {
		
		insertBack(item);
		
		return true;
	}

	//insert in the middle of the list
	Node* tmp = new Node(item);
	
	Node* curr = front;
	
	for(unsigned int i = 0; i < p-1; i++) {
		
		curr = curr->next;
	}

	tmp->next = curr->next;
	
	curr->next = tmp;
	
	length++;	

	return true;

}

// Removes and returns an item from position p (0-indexed)
// Return NULL if index is invalid
// PRE:   0 <= p < size
// POST:  Item is removed from list.
//        Node is deallocated but Kebab should in memory
//        Thus set data to NULL before deleting
// PARAM: p = position from where item will be removed
Kebab* LinkedList::removeAt(unsigned int p){
	//invalid index
	if(p >= length){
		
		return NULL;
	}

	//empty list
	if(length == 0 || front == NULL) {

		return NULL;
	}

	Kebab* kebab_to_remove;

	//all the cases indicated below would cause decrement in size by 1
	length--;

	//delete the only node in the list
	if(front->next == NULL) {
		
		kebab_to_remove = front->data;
		
		front->data = NULL;
		
		delete front;
		
		front = NULL;
		
		back = NULL;
		
		return kebab_to_remove;
	}

	Node* node_to_remove = NULL;

	//delete the first node in the list with length > 1
	if(p == 0) {
		
		kebab_to_remove = front->data;
		
		node_to_remove = front;
		
		front = node_to_remove->next;
		
		node_to_remove->data = NULL;
		
		delete node_to_remove;

		return kebab_to_remove;
	}

	//delete a node in the middle of the list with length >=3
	Node* curr = front;

	for(unsigned int i = 0; i < p-1; i++) {
		
		curr = curr -> next;
	}

	kebab_to_remove = curr->next->data;
	
	node_to_remove = curr->next;
	
	curr->next = node_to_remove->next;
	
	node_to_remove->data = NULL;
	
	delete node_to_remove;

	//delete the last node in the list
	if(length == p) {
		
		back = curr;
	}

	return kebab_to_remove;
}

// ACCESSORS

// Returns size of list
unsigned int LinkedList::size() const {
	
	return length;
}

// Returns whether the list is empty
bool LinkedList::isEmpty() const {
	
	if(front == NULL && length == 0) {
		
		return true;
	}
	
	return false;
}

// Returns existence of a Kebab that has at least
//   all the same ingredients as k, in any order
//   i.e. the list has a Kebab whose ingredients
//   are a super-multiset of k's ingredients
bool LinkedList::contains(Kebab& k) const {
	
	Node* curr = front;

	for (unsigned int i = 0; i < length; i++) {

		if(curr->data->hasSameIngredients(k)) {

			return true;
		}
	}

	return false;
}

// Returns item at index (0-indexed)
// Returns NULL if index is invalid
Kebab* LinkedList::elementAt(unsigned int p) const {
	
	if(p >= length) {
		
		return NULL;
	}

	Node* curr = front;

	for (unsigned int i = 0; i < p; i++) {
		curr = curr->next; 
	}

	return curr->data;	
}

// OVERLOADED OPERATORS

// overloaded assignment operator
// must work in the following cases:
// list2 = list1 -> general case
// list2 = list2 -> should do nothing
LinkedList& LinkedList::operator=(const LinkedList& ll) {
	if (this != &ll){
        length = ll.length;
        LinkedList* list1 = NULL;
        LinkedList* list2 = new LinkedList(ll);
        for (unsigned int i=0; i< length; i++){
            list1[i] = list2[i];
        }
    }
    return *this;
}
