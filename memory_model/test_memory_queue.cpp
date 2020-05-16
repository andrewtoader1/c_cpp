/*
	test_memory_queue.cpp
	@Author: Stephen Longofono and Andrew Toader
	@Purpose: Test the memory queue
*/


#include <iostream>
#include "memory_queue.h"
#include "memory_entry.h"
#include "dynamic_array_list.h"
#include <cassert>

// Define a familiar name for this type so we don't have to be so verbose
typedef MemoryQueue< MemoryEntry, DynamicArrayList< MemoryEntry> > memType;

int main(){
	// test valid empty queue
	memType L1;
	assert(L1.isEmpty());

	// Test valid queue enqueue
	L1.enqueue(MemoryEntry());
	assert(!L1.isEmpty());

	// Test dequeueBack
	L1.dequeueBack();
	assert(L1.isEmpty());

	// Test valid enqueueFront and MemoryEntry
	for(int i = 0; i < 5; ++i){
		for(int j = 0; j < 5; ++j){
			L1.enqueueFront(MemoryEntry(i,j));
		}
	}

	// Test peek and dequeue, manipulating MemoryEntry types
	int time = 9;
	for(int i = 0; i < 25; ++i){
		MemoryEntry temp = L1.peekFront();
		std::cout << "Servicing " << temp << ":";
		temp.serviced = true;
		temp.timeServiced = time++;
		std::cout << temp << std::endl;
		L1.dequeue();
	}
	assert(L1.isEmpty());

	/////////////////////////////////////////////////
	// Personal Tests
	////////////////////////////////////////////////

	MemoryQueue< int, DynamicArrayList< int > > L2;
	assert(L2.isEmpty());

	bool catch_err = false;

	// try to peek on empty queue front
	try
	{
		L2.peekFront();
	}
	catch(std::logic_error l) 
	{
		catch_err = true;
	}
	assert(catch_err);

	// try to peek on empty queue back
	catch_err = false;
	try
	{
		L2.peekBack();
	}
	catch(std::logic_error l)
	{
		catch_err = true;
	}
	assert(catch_err);

	// try to dequeue on empty queue front
	catch_err = false;
	try
	{
		L2.dequeue();
	}
	catch(std::logic_error l)
	{
		catch_err = true;
	}
	assert(catch_err);

	// try to dequeue on empty queue back
	catch_err = false;
	try
	{
		L2.dequeueBack();
	}
	catch(std::logic_error l)
	{
		catch_err = true;
	}
	assert(catch_err);

	L2.enqueue(3);
	L2.enqueueFront(1);
	L2.enqueue(14);
	L2.enqueueFront(4);

	std::cout << "Order should be:" << std::endl << 4 << std::endl << 1 << std::endl << 3 << std::endl << 14 << std::endl << std::endl;
	std::cout << "Order is:" << std::endl;

	L2.display();	// display the order of the queue using the member function

	assert(L2.peekFront() == 4);
	L2.dequeue();

	assert(L2.peekBack() == 14);
	L2.dequeueBack();

	assert(L2.peekFront() == 1);
	L2.dequeue();

	assert((L2.peekBack() == 3) && (L2.peekFront() == 3));
	L2.dequeueBack();

	assert(L2.isEmpty());

	std::cout << "================ PASSED ALL TESTS ================" << std::endl;
}
