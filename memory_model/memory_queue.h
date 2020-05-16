/*
	@file: memory_queue.h
	@Author: Andrew Toader
	@Purpose: Header file with implemntation for the queue data structure, derived from the abstract queue 'abstract_queue.h'
*/

#ifndef _MEMORY_QUEUE_H_
#define _MEMORY_QUEUE_H_

#include "abstract_queue.h"
#include <iostream>


// Type T must be printable via <<
// Type L must conform to AbstractList<T>
template <typename T, typename L >
class MemoryQueue: AbstractQueue<T>
{
	public:
		// return true if the priority queue is empty
		bool isEmpty()
		{
			return list.isEmpty();		// use internal list functon isEmpty
		}

		// return the length of the priority queue
		int getLength()
		{
			return list.getLength();	// user internal list function getLength
		}

		// add item to the back of the queue
		void enqueue(const T& item)
		{
			list.insert(list.getLength(), item);	// use internal list function insert. Zero indexed, so insert at index length
		}

		// add item to the front of the queue
		void enqueueFront(const T& item)
		{
			list.insert(0, item);	// use inernal list funciton insert. Insert at 0 or begining
		}

		// remove item from the front of the queue
		// @post: throw logic_error if there is nothing in the queue
		void dequeue()
		{
			if(list.isEmpty())		// if the list is empty, throw error to signal to user that they did something bad
			{
				throw std::logic_error("Trying to Dequeue front of empty queue!");
			}
			else					// if the list is not empty, remove element at index 0
			{
				list.remove(0);
			}
		}

		// remove item from the back of the queue
		// @post: throw logic_error if there is nothing in the queue
		void dequeueBack()
		{
			if(list.isEmpty())		// if the list is empty, throw error to signal to user that they did something bad
			{
				throw std::logic_error("Trying to Dequeue back of empty queue!");
			}
			else					// if the list is not empty, remove element at index 0
			{
				list.remove(list.getLength() - 1);
			}
		}

		// get the value of the item at the front of the queue
		// @post: throw logic_error if there is nothing in the queue
		T peekFront()
		{
			if(list.isEmpty())		// if the list is empty, throw error to signal to user that they did something bad
			{
				throw std::logic_error("Trying to peek front of empty queue!");
			}
			return list.getEntry(0);		// use internal list function getEntry at the begining of the list
		}

		// get the value of the item at the back of the queue
		// @post: throw logic_error if there is nothing in the queue
		T peekBack()
		{
			if(list.isEmpty())		// if the list is empty, throw error to signal to user that they did something bad
			{
				throw std::logic_error("Trying to peek back of empty queue!");
			}
			return list.getEntry(list.getLength() - 1);		// use internal list function getEntry and the lenght-1 of list
		}

		// display the contents of the queue
		void display()
		{
			for(int i = 0; i < list.getLength(); i++)		// loop through the list and print out all 
			{
				std::cout << list.getEntry(i) << std::endl;
			}
		}

	private:
		L list; // internal list
};

#endif // _MEMORY_QUEUE_H_
