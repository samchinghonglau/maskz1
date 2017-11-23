// file:        stack.cpp
// date:        2017-10-30
// description: implementation of a stack class for CPSC 221 PA2

#ifdef _STACK_H_

// default constructor
template <typename T>
Stack<T>::Stack()
{
	// create a new ArrayClass
	ac = ArrayClass<T>();
}

// copy constructor
template <typename T>
Stack<T>::Stack(const Stack& st)
{
	// create a new ArrayClass using the Arrayclass from st
	ac = ArrayClass<T>(st.ac);  // ???????????????????? needs to be tested
}

// assignment operator
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& st)
{
	ac = st.ac;
	return *this;
}

///////////////
// accessors //
///////////////

// Return true if the stack contains no items
template <typename T>
bool Stack<T>::isEmpty() const
{
	return ac.empty();
}

// Returns the number of items stored in the stack
template <typename T>
unsigned int Stack<T>::size() const
{
	return ac.size();
}

// Return a reference to the top item of the stack
// Throws an out_of_range exception if the stack is empty
template <typename T>
T& Stack<T>::peek()
{
	if( this->isEmpty() )
		throw std::out_of_range( "stack is empty" );
	else
		return ac.front();
}


//////////////
// mutators //
//////////////

// Pushes an item to the top of the stack
template <typename T>
void Stack<T>::push(const T item)
{
	ac.insert(0, item);
}

// Removes and returns (a copy of) the top item of the stack
// Throws an out_of_range exception if the stack is empty
template <typename T>
T Stack<T>::pop()
{
	if( this->isEmpty() )
		throw std::out_of_range( "stack is empty" );
	else {
		T t = ac.front(); 
		ac.erase(0);
		return t;
	}
}

#endif