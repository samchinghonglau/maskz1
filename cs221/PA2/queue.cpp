// file:        stack.cpp
// date:        2017-10-30
// description: implementation of a stack class for CPSC 221 PA2
///remove at the end
#include <iostream>
#include <cstdlib>

#ifdef _QUEUE_H_

// default constructor
template <typename T>
Queue<T>::Queue()
{
	front = 0;
	_size = 0;
	ac = ArrayClass<T>();
}

// copy constructor
template <typename T>
Queue<T>::Queue(const Queue& qq)
{
	front = qq.front;
	_size = qq._size;
	ac = ArrayClass<T>(qq.ac); // ????????? needs to be tested
}

// *************************************** should probably create a copy queue helper method for this

// assignment operator
template <typename T>
Queue<T>& Queue<T>::operator=(const Queue& qq)
{
	front = qq.front;
	_size = qq._size;
	ac = qq.ac;
	return *this;
}

///////////////
// accessors //
///////////////

// Return true if the queue contains no items
template <typename T>
bool Queue<T>::isEmpty() const
{
	if( _size == 0 )
		return true;
	else
		return false;
}

// Returns the number of items stored in the queue
template <typename T>
unsigned int Queue<T>::size() const
{
	return _size;
}

// Return a reference to the front item of the queue
// Throws an out_of_range exception if the queue is empty
template <typename T>
T& Queue<T>::peek()
{
	if( this->isEmpty() )
		throw std::out_of_range( "queue is empty" );
	else
		return ac.at(front);
}


//////////////
// mutators //
//////////////

// Inserts an item to the back of the queue
template <typename T>
void Queue<T>::enqueue(const T item)
{
	unsigned int arr_cap = ac.capacity();  // get the capacity of the array
	unsigned int arr_size = ac.size();     // get the size of the array, 
	                                       // Note: this isn't the size of the queue

	// if the queue isn't at capacity, there's no need to resize, so just add the
	// item in the correct spot. Note: a circular array is used so pushBack can't
	// be used in every case
	if( _size != arr_cap ) {
		// In this case, the size of the array hasn't reached the array capacity yet.
		// This means that the queue hasn't looped around yet. pushBack can be used.
		// For example: to enqueue(4) in the array: [1,2,3,_,_], we can just use 
		// pushBack. The underscores represent indices in the array where no items have
		// ever been allocated
		if( arr_size != arr_cap ) {
			ac.pushBack(item);
		}
		// In this case, the size of the array has reached the array capacity, and the 
		// queue has looped around. pushBack can't be used, so we have to modify the
		// item that is currently at the back of the queue. Note this item would have
		// been previously dequeued so it isn't really on the queue anymore, but is still
		// technically on the array. 
		// For example: to enqueue(4) in the array: [3,x,x,1,2] we need to put 4 at
		// index 1. x's represent previously dequeued items. In this case front = 3, 
		// _size = 3, and arr_cap = 5. So (front+_size) % arr_cap = (3+3)%5 = 1. 
		else {
			ac.at( (front + _size) % arr_cap ) = item;
		}
	}
	// if the queue is at capacity, the array needs to be resized. However, first the array
	// needs to be put in the correct order with front being index 0. Then the array needs to
	// be resized to have double the capacity, and lastly the item needs to be pushed to the back
	// For example: to enqueue(6) in the array: [3,4,5,1,2] first it needs to be rearranged to be:
	// [1,2,3,4,5], then the capacity needs to be doubled so: [1,2,3,4,5,_,_,_,_,_], and lastly
	// the item needs to be pushed to the back so: [1,2,3,4,5,6,_,_,_,_]
	else {
		ac.startFrom(front);      // rearrange the array so that the front of the queue is at index 0
		front = 0;          
		ac.resize( 2 * arr_cap ); // resize the array to have double the capacity
		ac.pushBack(item);           // push the item to the back of the queue
	}
	_size++; // increment size of the queue

	//print out whatever gets enqueue
	// std::cout << "value enqueued: "<< item << std::endl;
}

// Removes and returns (a copy of) the front item of the queue
// Throws an out_of_range exception if the queue is empty
template <typename T>
T Queue<T>::dequeue()
{
	if( this->isEmpty() ) {
		throw std::out_of_range( "queue is empty" );
	}
	else {
		T t = ac.at(front);
		
		// if the front of the queue is the last element of the array, then
		// front would wrap around and become index 0, else increment front
		if( front == ac.capacity() - 1 )
			front = 0;
		else
			front++;

		_size--;   // decrement size of the queue
		return t;  // return the item removed
	}
}

#endif