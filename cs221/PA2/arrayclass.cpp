// file:        arrayclass.cpp
// date:        2017-10-13
// description: implementation of a dynamic array class for CPSC 221 PA2

#ifdef _ARRAYCLASS_H_

// default constructor
// Creates an empty ArrayClass with DEFAULTCAPACITY
template <typename T>
ArrayClass<T>::ArrayClass()
{
	_capacity = DEFAULTCAPACITY;
	_size = 0;
	T* new_arr = new T[DEFAULTCAPACITY];
	arr = new_arr;
}

// ******************************************* these two can be implemented via a helper method

// parameterized constructor
// Creates an empty ArrayClass with capacity of n
template <typename T>
ArrayClass<T>::ArrayClass(unsigned int n)
{
  _capacity = n;
  _size = 0;
  T* new_arr = new T[n];
  arr = new_arr;
}

// copy constructor
// Creates an ArrayClass as a deep copy of the parameter
template <typename T>
ArrayClass<T>::ArrayClass(const ArrayClass& ac)
{
	copyArray(ac);     // ??????????????????????? needs to be tested
}

// destructor
template <typename T>
ArrayClass<T>::~ArrayClass()
{
  delete[] arr; // ???? ask TA or something
}

// overloaded assignment operator
template <typename T>
ArrayClass<T>& ArrayClass<T>::operator=(const ArrayClass& ac)
{
	copyArray(ac);
	return *this;
}

////////////////////////
// capacity functions //
////////////////////////

// Returns the number of elements stored in the array
template <typename T>
unsigned int ArrayClass<T>::size() const
{
  return _size;
}

// Returns the maximum capacity of the array
template <typename T>
unsigned int ArrayClass<T>::capacity() const
{
  return _capacity;
}

// Resizes the array
// If n is smaller than the size, then only the
//   first n elements are copied to the new array
template <typename T>
void ArrayClass<T>::resize(unsigned int n)
{
	T* new_arr = new T[n]; // create new array with size n
	unsigned int new_size = 0;

	// for all elements of arr up to n-1, copy them into
	// new_arr. If the size of arr is less than n, then
	// for those indices don't copy anything
	for( unsigned int i = 0; i < n; i++ ) {
		if( i < _size ) {
			new_arr[i] = arr[i];
			new_size++;   // for every added element, inc new_size
		}
	}

	_capacity = n;    // update capacity
	_size = new_size; // update size

	delete[] arr;   // delete the old-sized array
	arr = new_arr;  // arr now points to the resized array
}

// Check if array is empty
// return true if array is empty
template <typename T>
bool ArrayClass<T>::empty() const
{
  if( _size == 0 )
	  return true;
  else return false;
}

// Resizes the array to fit exactly as many items
//   as are stored in the array
template <typename T>
void ArrayClass<T>::shrinkToFit()
{
	T* new_arr = new T[_size]; // new array of size _size.

	// For the case where _size = 0, resize the array to the minimum
	// allowable size of 1
	if( _size == 0 ) {
		new_arr[0] = arr[0];
		_capacity = 1;
	}
	else {
		// copy the elements of the old-sized array into the
		// resized array
		for( unsigned int i = 0; i < _size; i++ )
			new_arr[i] = arr[i];

		_capacity = _size; // update capacity
	}

	delete[] arr;  // delete the old-sized array
	arr = new_arr; // arr now points to the resized array
}

//////////////////////////////
// element access functions //
//////////////////////////////

// Returns a reference to the element at position i
//   in the array.
// Throws an out_of_range exception if i is outside
//   the bounds of valid elements
template <typename T>
T& ArrayClass<T>::operator[](unsigned int i)
{
	return this->at(i);
}

// Returns a reference to the element at position i
//   in the array.
// Throws an out_of_range exception if i is outside
//   the bounds of valid elements
template <typename T>
T& ArrayClass<T>::at(unsigned int i)
{
	if( i >= _size )
		throw std::out_of_range( "position is out of bounds of the array" );
	else 
		return arr[i];
}

// Returns a reference to the first element in the array
// Calling this on an empty array throws out_of_range exception
template <typename T>
T& ArrayClass<T>::front()
{
	return this->at(0);
}

// Returns a reference to the last element in the array
// Calling this on an empty array throws out_of_range exception
template <typename T>
T& ArrayClass<T>::back()
{
	return this->at(_size-1);
}

//////////////////////
// modify functions //
//////////////////////

// Add an element at the end
// If array is at capacity before adding, then
//   expand the array to double its capacity and
//   then add.
template <typename T>
void ArrayClass<T>::pushBack(const T& item)
{
	if( _size == _capacity ) 
		this->expandArray();

	// add item to the index _size. This should be 1 after 
	// the previous last element. Increment _size
	arr[_size] = item;  
	_size++;
}

// Removes the last stored element in the array,
//   reducing container size by 1.
// No effect if called on an empty container
template <typename T>
void ArrayClass<T>::popBack()
{
	// this should ensure that the item at the end of the 
	// list is no longer accessible
	if( _size != 0 ) 
		_size--;
}

// Inserts an element before the element at specified position
// Expands the array if necessary
// Elements to the right of position (inclusive) must be shifted
// Throws out_of_range if position is > _size
// POST: item is at position, all elements formerly at position and beyond are shifted
template <typename T>
void ArrayClass<T>::insert(unsigned int position, const T& item)
{
	// if position is greater than index _size, throw an exception
	if( position > _size )
		throw std::out_of_range( "position is out of bounds of the array" );

	// if the array is at capacity, expand it
	if( _size == _capacity )
		this->expandArray();

	// starting at index _size, give each index the element at their previous
	// index, stop at index position + 1. This should right shift all the elements, 
	// from index position to the end of the array, over by one
	for( unsigned int i = _size; i > position; i-- )
		arr[i] = arr[i-1];
	// insert the item at index position
	arr[position] = item;
	_size++;  // increment size
}

// Removes an element from the array at specified position.
// Elements to the right of position will be shifted over to fill the gap
// Throws out_of_range if position is >= _size
template <typename T>
void ArrayClass<T>::erase(unsigned int position)
{
	// if position is greater than equal to index _size, throw an exception
	if( position >= _size )
		throw std::out_of_range( "position is out of bounds of the array" );

	// starting at index position, give each index the element at their next index,
	// stop at position _size - 1. This should left shift all the elements, from index
	// position + 1 to the end of the array over by one
	for( unsigned int i = position; i < _size; i++ )
		arr[i] = arr[i+1];

	// even though technically there will still be a value at index _size - 1, 
	// decrementing _size will make it so that this element isn't accessible
	_size--; 
}

// Reallocates stored items into a new array of the same size,
//   starting from specified position
// No effect if position is 0 or >= _size
// e.g. startFrom(3) on {1, 2, 3, 4, 5, 6, 7}
//   produces {4, 5, 6, 7, 1, 2, 3}
template <typename T>
void ArrayClass<T>::startFrom(unsigned int position)
{
	// if position is greater than 0 or less than size, then perform the function,
	// else no effect
	if( (position > 0) && (position < _size) ) {
		T* new_arr = new T[_capacity];   // new array of the same capacity
		unsigned int new_arr_index = 0;  // index for this new array
		
		// this loops iterates through the indices position to _size -1, and adds
		// them to the new array
		for( unsigned int i = position; i < _size; i++ ) {
			new_arr[new_arr_index] = arr[i];
			new_arr_index++;
		}

		// this loop iterates through the indices 0 to position - 1, and adds them
		// to the new array
		for( unsigned int i = 0; i < position; i++ ) {
			new_arr[new_arr_index] = arr[i];
			new_arr_index++;
		}

		delete[] arr;  // delete the old array
		arr = new_arr; // arr now points to the new array
	}
}

//////////////////////
// helper functions //
//////////////////////

// helper method for deep copy
template <typename T>
void ArrayClass<T>::copyArray(const ArrayClass& ac)
{
	// copy the values of ac's capacity and size
	_capacity = ac._capacity;
	_size = ac._size;
	// create a new array and then use the standard copy function
	// to copy ac's array into the new array
	T* new_array = new T[ac._capacity];
	std::copy(ac.arr, ac.arr + ac._capacity, new_array); 

	delete[] arr;    // delete the old array
	arr = new_array; // arr now points to the new array
}

// helper method for expanding array
template <typename T>
void ArrayClass<T>::expandArray()
{
	// call the resize function with the parameter being the
	// expansion factor multiplied by the capacity
	this->resize(EXPANSIONFACTOR * _capacity);
}

#endif