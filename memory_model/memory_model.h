/*
	@Author: Andrew Toader
	@Purpose: Header file for the memory Model class, implementation file is  'memory_model.cpp'
*/

#ifndef MM_HPP
#define MM_HPP

#include <stdint.h>
#include <string>
#include <set>
#include "memory_queue.h"
#include "memory_entry.h"
#include "dynamic_array_list.h"

typedef enum {L1=1, L2, L3, MM} MemoryType;

// Define a familiar name for the queue
typedef MemoryQueue< MemoryEntry, DynamicArrayList< MemoryEntry> > memType;

class MemoryModel
{
	private:
		// Internal Queues for L1 - MM (Variant 1)
		memType l1_queue;
		memType l2_queue;
		memType l3_queue;
		memType mm_queue;

		// Internal Queues for L1 - MM (Variant 2)
		MemoryQueue< MemoryEntry*, DynamicArrayList< MemoryEntry* > > queue_l1;
		MemoryQueue< MemoryEntry*, DynamicArrayList< MemoryEntry* > > queue_l2;
		MemoryQueue< MemoryEntry*, DynamicArrayList< MemoryEntry* > > queue_l3;
		MemoryQueue< MemoryEntry*, DynamicArrayList< MemoryEntry* > > queue_mm;

		// Create a queue that initially stores all requeusts added
		memType dump_queue;

		// Dynamic array to hold the value of all of the latencies
		DynamicArrayList<int> latency_list;

		// Variables to hold the max length of the queues
		int max1;
		int max2;
		int max3;
		int maxm;

		// List to store all requests in variant 2
		DynamicArrayList< MemoryEntry* > address_list;
		MemoryQueue< MemoryEntry*, DynamicArrayList< MemoryEntry* > > dump;
		MemoryQueue< MemoryEntry*, DynamicArrayList< MemoryEntry* > > dump_queue1;
		MemoryQueue< MemoryEntry*, DynamicArrayList< MemoryEntry* > > dump_queue2;
		MemoryQueue< MemoryEntry*, DynamicArrayList< MemoryEntry* > > dump_queue3;
		MemoryQueue< MemoryEntry*, DynamicArrayList< MemoryEntry* > > dump_queuem;


		// Internal clock to keep track of the time during the execution
		int Clk;

		// Function to simulate memory model 1
		std::string mm1();
		// Funciton to simulate memory model 2
		std::string mm2();

		// Do not alter the codes below
		std::set<int> v1, v2, v3, vmm;
		// helper function that determines whether an address access is a hit or miss
		bool checkHit(int addr, MemoryType memoryType);
		int variant;

	public:
		// will throw exception if variant is not 1 or 2
		MemoryModel(int variant);
		~MemoryModel();
		MemoryModel(const MemoryModel & ) = delete;
		MemoryModel & operator=(const MemoryModel &) = delete;
		void initMemory(std::set<int> s1, std::set<int> s2, std::set<int> s3, std::set<int> smm);

		// Add memory to search for; include time of request and adress
		void add(int t, int addr);

		// Create simulaiton of execution of search
		// @pre: a L1 cache with adresses to search (run add(t,a) method beforehand)
		//		 uses the mm1 or mm2 function (based on the variant variable inputed upon construction of object)
		// @return: string with log summary of the execution
		std::string exec();
};

#endif
