/** ADT Stack implementation.
    @file Stack.cpp */

#include <iostream>
#include <cstddef>
#include "Stack.hpp"
#include "Node.hpp"

// Constructor
template<class ItemType>
Stack<ItemType>::Stack() 
{
	currentSize = 0;
	headPtr = nullptr;
}  // end default constructor

// Destructor
template<class ItemType>
Stack<ItemType>::~Stack()
{
	clear();			// call the clear method, which deallocates all stack Node pointers and sets the head pointer to a nullptr
}  // end destructor

// Checks if stack is empty
template<class ItemType>
bool Stack<ItemType>::isEmpty() const
{
	if(currentSize == 0)		// check the currentSize member varaible to test emptiness condition
	{
		return true;
	}
	else
	{
		return false;
	}
}  // end isEmpty

// Returns size of Stack
template<class ItemType>
int Stack<ItemType>::size() const
{
	return currentSize;
}  // end size

// Push onto the stack
template<class ItemType>
bool Stack<ItemType>::push(const ItemType& newItem)
{
	Node<ItemType>* toadd_ptr = nullptr;			 // create new node with item in it
	toadd_ptr = new Node<ItemType>(newItem, headPtr); // set the next pointer as the current head pointer				
	headPtr = toadd_ptr;							 // set the head pointer as the new node
	currentSize++;									 // increment the current size of the stack
	return true;								 	 // signal to user that add was sucessful
}  // end push

// Peek first element off of the stack
template<class ItemType>
ItemType Stack<ItemType>::peek() const throw(logic_error)
{
	if(currentSize > 0)
	{
		ItemType returnItem;
		returnItem = headPtr->getItem();		// use the getItem method from the node class to return that item
		return returnItem;
	}
	else	// throw an exception if the current size is greater than 0
	{
		throw logic_error("Peeking at an empty stack");
	}
}  // end peek

// Pop first element from the stack
template<class ItemType>
bool Stack<ItemType>::pop() 
{
	if(headPtr == nullptr || currentSize == 0)	// if it is impossible to pop element from stack, return false
	{
		return false;
	}
	else
	{
		Node<ItemType>* to_remove = headPtr;	// set temporary pointer of current head 
		headPtr = headPtr->getNext();			// set the head pointer as the next pointer
		currentSize--;							// decrement the size variable
		delete to_remove;						// deallocate the previous head pointer
		to_remove = nullptr;					// set previous head pointer to null to prevent accidental memory leaks
		return true;							// signal to user that item was popped sucessfully
	}
}  // end pop

// clear the stack
template<class ItemType>
void Stack<ItemType>::clear()
{
	if(headPtr != nullptr)
	{
		Node<ItemType>* current_ptr = headPtr;						// initialize the pointer to clear to the next pointer to the second pointer
		Node<ItemType>* next_ptr = headPtr->getNext();				// initialize the next pointer

		while(next_ptr != nullptr) 					// loop through stack to free all nodes
		{
			delete current_ptr;						// delete pointer to current node
			current_ptr = next_ptr;
			next_ptr = current_ptr->getNext();
		}

		delete current_ptr;

		currentSize = 0;						// set size to 0
		headPtr = nullptr;						// set the head of the stack pointer to null
	}
}  // end clear

