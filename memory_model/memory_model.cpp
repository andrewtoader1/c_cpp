/*
	@file: memory_model.cpp
	@Author: Andrew Toader
	@Purpose: Implementation file for 'memory_model.hpp'
*/

#include <iostream>
#include "memory_model.h"

// will throw exception if variant is not 1 or 2
MemoryModel::MemoryModel(int var)
{
	v1 = std::set<int>();
	v2 = std::set<int>();
	v3 = std::set<int>();
	vmm = std::set<int>();

	// initialize the internal clock to 1
	Clk = 1;

	// initialize the max queue lengths to 0
	max1 = 0;
	max2 = 0;
	max3 = 0;
	maxm = 0;

	// Set the internal variant var as either 1 or 2 (if 1 will use mm1, and if 2 will use mm2)
	// throw exception if not 1 or 2
	if((var != 1) && (var != 2))
	{
		throw std::logic_error("Error:  MemoryModel(constructor): variant must be 1 or 2.");
	} 
	else
	{
		variant = var;
	}
}

MemoryModel::~MemoryModel()
{
	// make sure that the pointers are all freed
	while(!dump_queue1.isEmpty())
	{
		dump_queue1.dequeue();
	}
	while(!dump_queue2.isEmpty())
	{
		dump_queue2.dequeue();
	}
	while(!dump_queue3.isEmpty())
	{
		dump_queue3.dequeue();
	}
	while(!queue_l1.isEmpty())
	{
		queue_l1.dequeue();
	}
	while(!queue_l2.isEmpty())
	{
		queue_l2.dequeue();
	}
	while(!queue_l3.isEmpty())
	{
		queue_l3.dequeue();
	}
	while(!queue_mm.isEmpty())
	{
		queue_mm.dequeue();
	}
	while(!dump.isEmpty())
	{
		dump.dequeue();
	}
	while(!dump_queuem.isEmpty())
	{
		dump_queuem.dequeue();
	}

	// Free the pointers from the address list
	for(int i = 0; i < address_list.getLength(); i++)
	{
		delete address_list.getEntry(i);
	}
}

// memory is added to the L1 queue
// @pre: time and the adress of the request (Will be converted to MemoryEntry struct, then queued in the L1 Queue; 
// 											 Will be upgraded in exec() function if misses in L1)
// @post: member queue with MemoryEntry objected added to it at the end. Memory entry object built inside funciton
//		  based on the time and adress prarameters passed into funciton
// @return: None
void MemoryModel::add(int t, int addr)
{
	if(variant == 1)
	{
		MemoryEntry firstOff;
		DynamicArrayList<MemoryEntry> requestList;
		DynamicArrayList<int> requestAdress;

		MemoryEntry to_add(t, addr);					// create a new memory entry object with the time of the request and the adress

		if(!dump_queue.isEmpty())							// if the l1 queue has items in it, check if any of the items timeIssued the same as t
		{
			firstOff = dump_queue.peekBack();

			if(firstOff.timeIssued == t)				// if the first off of the queue has the same timeIssued as the object you want to add, check the address number
			{
				requestList.insert(0, firstOff);			// put the first off of the queue in dynamic array and dequeue it
				requestAdress.insert(0, firstOff.address);

				dump_queue.dequeueBack();
				if(!dump_queue.isEmpty())
				{
					firstOff = dump_queue.peekBack();			// take the next item off fo the queue
					while(firstOff.timeIssued == t)				// continually assess if the next item has timeIssued == t
					{
						for(int i = 0; i < requestAdress.getLength(); i++)
						{
							if(requestAdress.getEntry(i) > firstOff.address)		// insert the item taken off into dynamic array accoring to the address
							{
								requestAdress.insert(i, firstOff.address);
								requestList.insert(i, firstOff);
								break;
							}
							if(i == requestAdress.getLength() - 1)
							{
								requestAdress.insert(i + 1, firstOff.address);
								requestList.insert(i + 1, firstOff);
								break;
							}
						}
						dump_queue.dequeueBack();											// dequeue the item from the array

						if(!dump_queue.isEmpty())
						{
							firstOff = dump_queue.peekBack();							// chose next item off of array
						}
						else
						{
							break;
						}
					}
				}

				for(int i = 0; i < requestAdress.getLength(); i++)				// insert the item to add based off of the address
				{
					if(requestAdress.getEntry(i) > addr)
					{
						requestAdress.insert(i, addr);
						requestList.insert(i, to_add);
						break;
					}
					if(i == requestAdress.getLength() - 1)
					{
						requestAdress.insert(i + 1, addr);
						requestList.insert(i + 1, to_add);
						break;
					}
				}

				for(int i = 0; i < requestList.getLength(); i++)			// put the elements taken off fo the queue and the new element into the l1_queue and time queue
				{
					firstOff = requestList.getEntry(i);
					dump_queue.enqueue(firstOff);
				}
				requestList.clear();
				requestAdress.clear();
			}
			else
			{
				dump_queue.enqueue(to_add);		// enqueue this object into the l1 queue to test for a hit or miss there
			}
		}
		else
		{
			dump_queue.enqueue(to_add);		// enqueue this object into the l1 queue to test for a hit or miss there
		}
	}
	else
	{
		MemoryEntry* firstOff;
		DynamicArrayList< MemoryEntry* > requestList;
		DynamicArrayList<int> requestAdress;

		MemoryEntry* to_add;					// create a new pointer to a memory entry object with the time of the request and the adress
		to_add = new MemoryEntry(t, addr);

		if(!dump_queue1.isEmpty())							// if the l1 queue has items in it, check if any of the items timeIssued the same as t
		{
			firstOff = dump_queue1.peekBack();

			if(firstOff->timeIssued == t)				// if the first off of the queue has the same timeIssued as the object you want to add, check the address number
			{
				requestList.insert(0, firstOff);			// put the first off of the queue in dynamic array and dequeue it
				requestAdress.insert(0, firstOff->address);

				dump_queue1.dequeueBack();
				dump_queue2.dequeueBack();
				dump_queue3.dequeueBack();
				dump.dequeueBack();

				if(!dump_queue1.isEmpty())
				{
					firstOff = dump_queue1.peekBack();			// take the next item off fo the queue
					while(firstOff->timeIssued == t)				// continually assess if the next item has timeIssued == t
					{
						for(int i = 0; i < requestAdress.getLength(); i++)
						{
							if(requestAdress.getEntry(i) > firstOff->address)		// insert the item taken off into dynamic array accoring to the address
							{
								requestAdress.insert(i, firstOff->address);
								requestList.insert(i, firstOff);
								break;
							}
							if(i == requestAdress.getLength() - 1)
							{
								requestAdress.insert(i + 1, firstOff->address);
								requestList.insert(i + 1, firstOff);
								break;
							}
						}
						dump_queue1.dequeueBack();									// dequeue the item from the array 
						dump_queue2.dequeueBack();
						dump_queue3.dequeueBack();
						dump.dequeueBack();

						if(!dump_queue1.isEmpty())
						{
							firstOff = dump_queue1.peekBack();				// chose next item off of array
						}
						else
						{
							break;
						}						
					}
				}

				for(int i = 0; i < requestAdress.getLength(); i++)				// insert the item to add based off of the address
				{
					if(requestAdress.getEntry(i) > addr)
					{
						requestAdress.insert(i, addr);
						requestList.insert(i, to_add);
						break;
					}
					if(i == requestAdress.getLength() - 1)
					{
						requestAdress.insert(i + 1, addr);
						requestList.insert(i + 1, to_add);
						break;
					}
				}

				for(int i = 0; i < requestList.getLength(); i++)			// put the elements taken off fo the queue and the new element into the l1_queue and time queue
				{
					firstOff = requestList.getEntry(i);
					dump_queue1.enqueue(firstOff);
					dump_queue2.enqueue(firstOff);
					dump_queue3.enqueue(firstOff);
					dump.enqueue(firstOff);
				}
				requestList.clear();
				requestAdress.clear();
			}
			else
			{
				dump_queue1.enqueue(to_add);		// enqueue this object into the l1 queue to test for a hit or miss there
				dump_queue2.enqueue(to_add);
				dump_queue3.enqueue(to_add);
				dump.enqueue(to_add);
			}
		}
		else
		{
			dump_queue1.enqueue(to_add);		// enqueue this object into the l1 queue to test for a hit or miss there
			dump_queue2.enqueue(to_add);
			dump_queue3.enqueue(to_add);
			dump.enqueue(to_add);
		}
	}
}

// Execute the memory model simulation. Uses either the funciton mm1() or mm2() depending on if the variant is 1 or 2
// @pre: a L1 cache with adresses to search (run add(t,a) method beforehand)
//		 uses the mm1 or mm2 function (based on the variant variable inputed upon construction of object)
// @return: string with log summary of the execution
std::string MemoryModel::exec()
{
	std::string mm_out;		// initialize output string
	if(variant == 1)	// if using the first memory model, go to the mm1() simulation function
	{
		mm_out = mm1();
	}
	else				// if using the second memory model, go to the mm2() simulation funciton
	{
		// make a master list of all of the adresses using the dump queue
		while(!dump.isEmpty())
		{
			address_list.insert(address_list.getLength(), dump.peekFront());
			dump.dequeue();
		}

		mm_out = mm2();
	}

	// print out the stats of the simulation
	int max = 0;
	double mean = 0;

	for(int i = 0; i < latency_list.getLength(); i++)
	{
		if(latency_list.getEntry(i) > max)
		{
			max  = latency_list.getEntry(i);
		}
		mean += latency_list.getEntry(i);
	}

	if(latency_list.getLength() > 0)
	{
		mean = mean/latency_list.getLength();
	}
	else
	{
		throw std::logic_error("Error: no simulation ran; cant divide mean by 0.");
	}

	// print out the max and the mean
	std::cout << "Maximum Total Latency: " << max << "s" << std::endl;
	std::cout << "Mean Latency: " << mean << "s" << std::endl;

	// print out the maximum length of the queues
	std::cout << "Maximum Length L1 Queue: " << max1 << std::endl;
	std::cout << "Maximum Length L2 Queue: " << max2 << std::endl;
	std::cout << "Maximum Length L3 Queue: " << max3 << std::endl;
	std::cout << "Maximum Length MM Queue: " << maxm << std::endl;

	// print out extra line to space out if multiple tests are run at the same time
	std::cout << std::endl;

	return mm_out;		// return the string output from either mm1() or mm2()
}

// Implementation of vairant 1 execution
// Called by exec() function, look @pre and @post for exec()
std::string MemoryModel::mm1()
{
	std::string mm_out = "";		// string variable to keep track of output log
	bool hit_res;					// boolean to check the result of checkHit()

	bool l1_servicing = false;		// bool to keep track if the cache is servicing a request
	int l1_s_time = 0;				// integer to keep track of the number of cycles it has been servicing that request
	MemoryEntry l1_entry;			// memory entry to hold the value of the entry at the top of the queue

	bool l2_servicing = false;
	int l2_s_time = 0;
	MemoryEntry l2_entry;

	bool l3_servicing = false;
	int l3_s_time = 0;
	MemoryEntry l3_entry;

	bool mm_servicing = false;
	int mm_s_time = 0;
	MemoryEntry mm_entry;

	DynamicArrayList<MemoryEntry> requestList;		// used to order multiple requeusts in the same clock cycle
	DynamicArrayList<int> requestAdress;			// used to assist in ordering multiple rquests in the same clock cycle
	memType lower_cache_queue;						// used to store requests from L2, L3, and L4 queue to print out during the next clock cycle

	while(!dump_queue.isEmpty() || !l1_queue.isEmpty() || !l2_queue.isEmpty() || !l3_queue.isEmpty() || !mm_queue.isEmpty())
	{
		// move from dump_qeue to l1_queue if it is the right time
		if(!dump_queue.isEmpty())
		{
			l1_entry = dump_queue.peekFront();
			while(l1_entry.timeIssued == Clk)
			{
				l1_queue.enqueue(l1_entry);
				mm_out.append(std::to_string(Clk) + ", L1, Request, " + std::to_string(l1_entry.address) + "\n");
				dump_queue.dequeue();

				// if the queue is not empty, see if the next element has the same time otherwise break out of the loop
				if(!dump_queue.isEmpty())
				{
					l1_entry = dump_queue.peekFront();
				}
				else
				{
					break;
				}
			}
		}

		// print out requests that move to queues L2, L3, and MM
		while(!lower_cache_queue.isEmpty())
		{
			l2_entry = lower_cache_queue.peekFront();		// get the element from the queue
			if(l2_entry.cacheToService == 2)				// if it is an L2 requeust, print that out
			{
				mm_out.append(std::to_string(Clk) + ", L2, Request, " + std::to_string(l2_entry.address) + "\n");
			}
			if(l2_entry.cacheToService == 3)				// if it is an L3 requeust, print that out
			{
				mm_out.append(std::to_string(Clk) + ", L3, Request, " + std::to_string(l2_entry.address) + "\n");
			}
			if(l2_entry.cacheToService == 4)				// if it is an MM requeust, print that out
			{
				mm_out.append(std::to_string(Clk) + ", MM, Request, " + std::to_string(l2_entry.address) + "\n");
			}
			lower_cache_queue.dequeue();					// dequeue the element from the queue
		}

		// Look for Hits or Misses in L1
		if(!l1_queue.isEmpty())		// if the L1 queue has elements in it, check and report status
		{
			// Set the max queue length of 1
			if(l1_queue.getLength() > max1)
			{
				max1 = l1_queue.getLength();
			}

			l1_entry = l1_queue.peekFront();	// get the first element on the queue

			if(l1_entry.timeIssued <= Clk)		// make sure that the queue is not servicing any element before its clock time
			{
				if(!l1_servicing)
				{
					l1_servicing = true;		// start service to the next in the queue
					l1_s_time++;
				}
				else
				{
					if(l1_s_time == 2)
					{
						hit_res = checkHit(l1_entry.address, L1);	// check if it is a hit after 2 clock cycles
						if(hit_res)
						{
							// adjust memoryEntry variables
							l1_entry.serviced = true;
							l1_entry.timeServiced = Clk;

							// Add the latency to the latency list
							latency_list.insert(latency_list.getLength(), Clk - l1_entry.timeIssued);

							// add the report to the out string
							mm_out.append(std::to_string(Clk) + ", L1, Hit, " + std::to_string(l1_entry.address) + "\n");

							// dequeue the memory entry from the l1 cache
							l1_queue.dequeue();

							// set the L1 servicing variables accordingly
							if(!l1_queue.isEmpty())
							{
								l1_entry = l1_queue.peekFront();

								if(l1_entry.timeIssued <= Clk)
								{
									l1_servicing = true;
									l1_s_time = 1;
								}
								else
								{
									l1_servicing = false;
									l1_s_time = 0;
								}
							}
							else 				// if the L1 queue is empty, set the variables accordingly
							{
								l1_servicing = false;
								l1_s_time = 0;
							}
						}
						else
						{
							// queue entry to display the reuquest next clock cycle
							l1_entry.cacheToService = 2;
							l1_entry.timeMoved = Clk + 1;
							lower_cache_queue.enqueue(l1_entry);

							// enqueue the entry into L2
							l2_queue.enqueue(l1_entry);

							// dequeue entry from L1
							l1_queue.dequeue();

							// add the report to the string
							mm_out.append(std::to_string(Clk) + ", L1, Miss, " + std::to_string(l1_entry.address) + "\n");

							// set the L1 servicing variables accordingly
							if(!l1_queue.isEmpty())
							{
								l1_entry = l1_queue.peekFront();

								if(l1_entry.timeIssued <= Clk)
								{
									l1_servicing = true;
									l1_s_time = 1;
								}
								else
								{
									l1_servicing = false;
									l1_s_time = 0;
								}
							}
							else 				// if the L1 queue is empty, set the variables accordingly
							{
								l1_servicing = false;
								l1_s_time = 0;
							}
						}
					}
					else
					{
						l1_s_time++;
					}
				}
			}
		}

		// Look for Hits or Misses in L2
		if(!l2_queue.isEmpty())		// if the L2 queue has elements in it, check and report status
		{
			// Set the max queue length of 2
			if(l2_queue.getLength() > max2)
			{
				max2 = l2_queue.getLength();
			}

			l2_entry = l2_queue.peekFront();	// get the first element on the queue
			if(!l2_servicing)
			{
				l2_servicing = true;		// start service to the next in the queue
			}
			else
			{
				if(l2_s_time == 5)
				{
					hit_res = checkHit(l2_entry.address, L2);	// check if it is a hit after 5 clock cycles
					if(hit_res)
					{
						// adjust memoryEntry variables
						l2_entry.serviced = true;
						l2_entry.timeServiced = Clk;

						// Add the latency to the latency list
						latency_list.insert(latency_list.getLength(), Clk - l2_entry.timeIssued);

						// add the report to the out string
						mm_out.append(std::to_string(Clk) + ", L2, Hit, " + std::to_string(l2_entry.address) + "\n");

						// dequeue the memory entry from the L2 cache
						l2_queue.dequeue();

						// set the L2 servicing variables accordingly
						if(!l2_queue.isEmpty())
						{
							l2_entry = l2_queue.peekFront();

							if(l2_entry.timeMoved <= Clk)
							{
								l2_servicing = true;
								l2_s_time = 1;
							}
							else
							{
								l2_servicing = false;
								l2_s_time = 1;
							}
						}
						else 				// if the L2 queue is empty, set the variables accordingly
						{
							l2_servicing = false;
							l2_s_time = 0;
						}
					}
					else
					{
						// queue entry to display the reuquest next clock cycle
						l2_entry.cacheToService = 3;
						l2_entry.timeMoved = Clk + 1;
						lower_cache_queue.enqueue(l2_entry);

						// enqueue the entry into L3
						l3_queue.enqueue(l2_entry);

						// dequeue entry from L2
						l2_queue.dequeue();

						// add the report to the string
						mm_out.append(std::to_string(Clk) + ", L2, Miss, " + std::to_string(l2_entry.address) + "\n");

						// set the L2 servicing variables accordingly
						if(!l2_queue.isEmpty())
						{
							l2_entry = l2_queue.peekFront();

							if(l2_entry.timeMoved <= Clk)
							{
								l2_servicing = true;
								l2_s_time = 1;
							}
							else
							{
								l2_servicing = false;
								l2_s_time = 1;
							}
						}
						else 				// if the L2 queue is empty, set the variables accordingly
						{
							l2_servicing = false;
							l2_s_time = 0;
						}
					}
				}
				else
				{
					l2_s_time++;
				}
			}
		}

		// Look for Hits or Misses in L3
		if(!l3_queue.isEmpty())		// if the L3 queue has elements in it, check and report status
		{
			// Set the max queue length of 3
			if(l3_queue.getLength() > max3)
			{
				max3 = l3_queue.getLength();
			}

			l3_entry = l3_queue.peekFront();	// get the first element on the queue

			if(!l3_servicing)
			{
				l3_servicing = true;		// start service to the next in the queue
			}
			else
			{
				if(l3_s_time == 10)
				{
					hit_res = checkHit(l3_entry.address, L3);	// check if it is a hit after 10 clock cycles
					if(hit_res)
					{
						// adjust memoryEntry variables
						l3_entry.serviced = true;
						l3_entry.timeServiced = Clk;

						// Add the latency to the latency list
						latency_list.insert(latency_list.getLength(), Clk - l3_entry.timeIssued);

						// add the report to the out string
						mm_out.append(std::to_string(Clk) + ", L3, Hit, " + std::to_string(l3_entry.address) + "\n");

						// dequeue the memory entry from the l3 cache
						l3_queue.dequeue();

						// set the L3 servicing variables accordingly
						if(!l3_queue.isEmpty())
						{
							l3_entry = l3_queue.peekFront();

							if(l3_entry.timeMoved <= Clk)
							{
								l3_servicing = true;
								l3_s_time = 1;
							}
							else
							{
								l3_servicing = false;
								l3_s_time = 1;
							}
						}
						else 				// if the L3 queue is empty, set the variables accordingly
						{
							l3_servicing = false;
							l3_s_time = 0;
						}
					}
					else
					{
						// queue entry to display the reuquest next clock cycle
						l3_entry.cacheToService = 4;
						l2_entry.timeMoved = Clk + 1;
						lower_cache_queue.enqueue(l3_entry);

						// enqueue the entry into MM
						mm_queue.enqueue(l3_entry);

						// dequeue entry from L3
						l3_queue.dequeue();

						// add the report to the string
						mm_out.append(std::to_string(Clk) + ", L3, Miss, " + std::to_string(l3_entry.address) + "\n");

						// set the L3 servicing variables accordingly
						if(!l3_queue.isEmpty())
						{
							l3_entry = l3_queue.peekFront();

							if(l3_entry.timeMoved <= Clk)
							{
								l3_servicing = true;
								l3_s_time = 1;
							}
							else
							{
								l3_servicing = false;
								l3_s_time = 1;
							}
						}
						else 				// if the L3 queue is empty, set the variables accordingly
						{
							l3_servicing = false;
							l3_s_time = 0;
						}
					}
				}
				else
				{
					l3_s_time++;
				}
			}
		}

		// Look for Hits or Misses in MM
		if(!mm_queue.isEmpty())		// if the MM queue has elements in it, check and report status
		{
			// Set the max queue length of mm
			if(mm_queue.getLength() > maxm)
			{
				maxm = mm_queue.getLength();
			}

			mm_entry = mm_queue.peekFront();	// get the first element on the queue

			if(!mm_servicing)
			{
				mm_servicing = true;		// start service to the next in the queue
			}
			else
			{
				if(mm_s_time == 100)
				{
					hit_res = checkHit(mm_entry.address, MM);	// check if it is a hit after 100 clock cycles
					if(hit_res)
					{
						// adjust memoryEntry variables
						mm_entry.serviced = true;
						mm_entry.timeServiced = Clk;

						// Add the latency to the latency list
						latency_list.insert(latency_list.getLength(), Clk - mm_entry.timeIssued);

						// add the report to the out string
						mm_out.append(std::to_string(Clk) + ", MM, Hit, " + std::to_string(mm_entry.address) + "\n");

						// dequeue the memory entry from the MM cache
						mm_queue.dequeue();

						// Set the MM servicing variables accordingly
						if(!mm_queue.isEmpty())
						{
							mm_entry = mm_queue.peekFront();

							if(mm_entry.timeMoved <= Clk)
							{
								mm_servicing = true;
								mm_s_time = 1;
							}
							else
							{
								mm_servicing = false;
								mm_s_time = 1;
							}
						}
						else 				// if the MM queue is empty, set the variables accordingly
						{
							mm_servicing = false;
							mm_s_time = 0;
						}
					}
					else 		// throw error if it does not exist in main memory
					{
						// Add the latency to the latency list
						latency_list.insert(latency_list.getLength(), Clk - mm_entry.timeIssued);

						// dequeue entry from MM
						mm_queue.dequeue();

						// add the report to the string
						mm_out.append(std::to_string(Clk) + ", MM, Miss, " + std::to_string(mm_entry.address) + "\n");

						// Set the MM servicing variables accordingly
						if(!mm_queue.isEmpty())
						{
							mm_entry = mm_queue.peekFront();

							if(mm_entry.timeMoved <= Clk)
							{
								mm_servicing = true;
								mm_s_time = 1;
							}
							else
							{
								mm_servicing = false;
								mm_s_time = 1;
							}
						}
						else 				// if the MM queue is empty, set the variables accordingly
						{
							mm_servicing = false;
							mm_s_time = 0;
						}	
					}
				}
				else
				{
					mm_s_time++;
				}
			}
		}
		Clk++;						// increment the clock
	}

	return mm_out;					// return the output string
}



// Implementation of vairant 2 execution
// Called by exec() function, look @pre and @post for exec()
std::string MemoryModel::mm2()
{
	std::string mm_out = "";		// string variable to keep track of output log
	bool hit_res;					// boolean to check the result of checkHit()

	bool l1_servicing = false;		// bool to keep track if the cache is servicing a request
	int l1_s_time = 0;				// integer to keep track of the number of cycles it has been servicing that request
	//MemoryEntry l1_entry;			// memory entry to hold the value of the entry at the top of the queue

	bool l2_servicing = false;
	int l2_s_time = 0;
	//MemoryEntry l2_entry;

	bool l3_servicing = false;
	int l3_s_time = 0;
	//MemoryEntry l3_entry;

	bool mm_servicing = false;
	int mm_s_time = 0;
	//MemoryEntry mm_entry;

	while(!dump_queuem.isEmpty() || !dump_queue1.isEmpty() || !dump_queue2.isEmpty() || !dump_queue3.isEmpty() || !queue_l1.isEmpty() || !queue_l2.isEmpty() || !queue_l3.isEmpty() || !queue_mm.isEmpty())
	{

		////////////////////////////////////////////////////////////////////////////////////////////
		// L1
		////////////////////////////////////////////////////////////////////////////////////////////

		// move from dump_qeue to l1_queue if it is the right time and print out requeust
		if(!dump_queue1.isEmpty())
		{
			while(dump_queue1.peekFront()->timeIssued == Clk)
			{
				queue_l1.enqueue(dump_queue1.peekFront());
				mm_out.append(std::to_string(Clk) + ", L1, Request, " + std::to_string(dump_queue1.peekFront()->address) + "\n");
				dump_queue1.dequeue();

				// if the queue is empty, break out of the loop
				if(dump_queue1.isEmpty())
				{
					break;
				}
			}
		}

		// Look for Hits or Misses in L1
		if(!queue_l1.isEmpty())		// if the L1 queue has elements in it, check and report status
		{
			// Set the max queue length of 1
			if(queue_l1.getLength() > max1)
			{
				max1 = queue_l1.getLength();
			}

			if(!l1_servicing)
			{
				l1_servicing = true;		// start service to the next in the queue
				l1_s_time++;
			}
			else
			{
				if(l1_s_time == 2)
				{
					hit_res = checkHit(queue_l1.peekFront()->address, L1);	// check if it is a hit after 2 clock cycles
					if(hit_res)
					{
						// adjust memoryEntry variables
						queue_l1.peekFront()->serviced = true;
						queue_l1.peekFront()->timeServiced = Clk;

						// Add the latency to the latency list
						latency_list.insert(latency_list.getLength(), Clk - queue_l1.peekFront()->timeIssued);

						// add the report to the out string
						mm_out.append(std::to_string(Clk) + ", L1, Hit, " + std::to_string(queue_l1.peekFront()->address) + "\n");

						// dequeue the memory entry from the l1 cache
						queue_l1.dequeue();

						// set the L1 servicing variables accordingly
						if(!queue_l1.isEmpty())
						{
							if(queue_l1.peekFront()->timeIssued <= Clk)
							{
								l1_servicing = true;
								l1_s_time = 1;
							}
							else
							{
								l1_servicing = false;
								l1_s_time = 0;
							}
						}
						else 				// if the L1 queue is empty, set the variables accordingly
						{
							l1_servicing = false;
							l1_s_time = 0;
						}
					}
					else
					{
						// add the report to the string
						mm_out.append(std::to_string(Clk) + ", L1, Miss, " + std::to_string(queue_l1.peekFront()->address) + "\n");

						// dequeue entry from L1
						queue_l1.dequeue();

						// set the L1 servicing variables accordingly
						if(!queue_l1.isEmpty())
						{
							if(queue_l1.peekFront()->timeIssued <= Clk)
							{
								l1_servicing = true;
								l1_s_time = 1;
							}
							else
							{
								l1_servicing = false;
								l1_s_time = 0;
							}
						}
						else 				// if the L1 queue is empty, set the variables accordingly
						{
							l1_servicing = false;
							l1_s_time = 0;
						}
					}
				}
				else
				{
					l1_s_time++;
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// L2
		////////////////////////////////////////////////////////////////////////////////////////////
		
		// move from dump_qeue to l2_queue if it is the right time and print out requeust
		if(!dump_queue2.isEmpty())
		{
			while(dump_queue2.peekFront()->timeIssued == Clk)
			{
				queue_l2.enqueue(dump_queue2.peekFront());
				mm_out.append(std::to_string(Clk) + ", L2, Request, " + std::to_string(dump_queue2.peekFront()->address) + "\n");
				dump_queue2.dequeue();

				// if the queue is empty, break out of the loop
				if(dump_queue2.isEmpty())
				{
					break;
				}
			}
		}


		// If a request is serviced by another queue during a request, dequeue it and start another one
		if(!queue_l2.isEmpty())
		{
			while(queue_l2.peekFront()->serviced)
			{
				queue_l2.dequeue();

				if(queue_l2.isEmpty())
				{
					l2_servicing = false;
					l2_s_time = 0;
					break;
				}
				else
				{
					l2_servicing = true;
					l2_s_time = 0;
				}
			}
		}

		// Look for Hits or Misses 
		if(!queue_l2.isEmpty())		// if the queue has elements in it, check and report status
		{
			// Set the max queue length of 2
			if(queue_l2.getLength() > max2)
			{
				max2 = queue_l2.getLength();
			}

			if(!l2_servicing)
			{
				l2_servicing = true;		// start service to the next in the queue
				l2_s_time++;
			}
			else
			{
				if(l2_s_time == 5)
				{
					hit_res = checkHit(queue_l2.peekFront()->address, L2);	// check if it is a hit after 5 clock cycles
					if(hit_res)
					{
						// adjust memoryEntry variables
						queue_l2.peekFront()->serviced = true;
						queue_l2.peekFront()->timeServiced = Clk;

						// Add the latency to the latency list
						latency_list.insert(latency_list.getLength(), Clk - queue_l2.peekFront()->timeIssued);

						// add the report to the out string
						mm_out.append(std::to_string(Clk) + ", L2, Hit, " + std::to_string(queue_l2.peekFront()->address) + "\n");

						// dequeue the memory entry from the l2 cache
						queue_l2.dequeue();

						// set the L2 servicing variables accordingly
						if(!queue_l2.isEmpty())
						{
							while(queue_l2.peekFront()->serviced)		// dequeue any requeusts that have already been serviced before the clock starts
							{
								queue_l2.dequeue();
								if(queue_l2.isEmpty())					// break if queue 2 is empty
								{
									break;
								}
							}

							if(!queue_l2.isEmpty())
							{
								if(queue_l2.peekFront()->timeIssued <= Clk)
								{
									l2_servicing = true;
									l2_s_time = 1;
								}
								else
								{
									l2_servicing = false;
									l2_s_time = 0;
								}
							}
							else
							{
								l2_servicing = false;
								l2_s_time = 0;
							}
						}
						else 				// if the L2 queue is empty, set the variables accordingly
						{
							l2_servicing = false;
							l2_s_time = 0;
						}
					}
					else
					{
						// add the report to the string
						mm_out.append(std::to_string(Clk) + ", L2, Miss, " + std::to_string(queue_l2.peekFront()->address) + "\n");

						// dequeue entry from L2
						queue_l2.dequeue();

						// set the L2 servicing variables accordingly
						if(!queue_l2.isEmpty())
						{
							while(queue_l2.peekFront()->serviced)		// dequeue any requeusts that have already been serviced before the clock starts
							{
								queue_l2.dequeue();
								if(queue_l2.isEmpty())					// break if queue 2 is empty
								{
									break;
								}
							}

							if(!queue_l2.isEmpty())
							{
								if(queue_l2.peekFront()->timeIssued <= Clk)
								{
									l2_servicing = true;
									l2_s_time = 1;
								}
								else
								{
									l2_servicing = false;
									l2_s_time = 0;
								}
							}
							else
							{
								l2_servicing = false;
								l2_s_time = 0;
							}
						}
						else 				// if the L2 queue is empty, set the variables accordingly
						{
							l2_servicing = false;
							l2_s_time = 0;
						}
					}
				}
				else
				{
					l2_s_time++;
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		// L3
		////////////////////////////////////////////////////////////////////////////////////////////
		
		// move from dump_qeue to l3_queue if it is the right time and print out requeust
		if(!dump_queue3.isEmpty())
		{
			while(dump_queue3.peekFront()->timeIssued == Clk)
			{
				queue_l3.enqueue(dump_queue3.peekFront());
				mm_out.append(std::to_string(Clk) + ", L3, Request, " + std::to_string(dump_queue3.peekFront()->address) + "\n");
				dump_queue3.dequeue();

				// if the queue is empty, break out of the loop
				if(dump_queue3.isEmpty())
				{
					break;
				}
			}
		}

		// If a request is serviced by another queue during a request, dequeue it and start another one
		if(!queue_l3.isEmpty())
		{
			while(queue_l3.peekFront()->serviced)
			{
				queue_l3.dequeue();

				if(queue_l3.isEmpty())
				{
					l3_servicing = false;
					l3_s_time = 0;
					break;
				}
				else
				{
					l3_servicing = true;
					l3_s_time = 0;
				}
			}
		}

		// Look for Hits or Misses 
		if(!queue_l3.isEmpty())		// if the queue has elements in it, check and report status
		{	
			// Set the max queue length of 2
			if(queue_l3.getLength() > max3)
			{
				max3 = queue_l3.getLength();
			}

			if(!l3_servicing)
			{
				l3_servicing = true;		// start service to the next in the queue
				l3_s_time++;
			}
			else
			{
				if(l3_s_time == 10)
				{
					hit_res = checkHit(queue_l3.peekFront()->address, L3);	// check if it is a hit after 10 clock cycles
					if(hit_res)
					{
						// adjust memoryEntry variables
						queue_l3.peekFront()->serviced = true;
						queue_l3.peekFront()->timeServiced = Clk;

						// Add the latency to the latency list
						latency_list.insert(latency_list.getLength(), Clk - queue_l3.peekFront()->timeIssued);

						// add the report to the out string
						mm_out.append(std::to_string(Clk) + ", L3, Hit, " + std::to_string(queue_l3.peekFront()->address) + "\n");

						// dequeue the memory entry from the l3 cache
						queue_l3.dequeue();

						// set the L3 servicing variables accordingly
						if(!queue_l3.isEmpty())
						{
							while(queue_l3.peekFront()->serviced)		// dequeue any requeusts that have already been serviced before the clock starts
							{
								queue_l3.dequeue();
								if(queue_l3.isEmpty())					// break if queue 3 is empty
								{
									break;
								}
							}

							if(!queue_l3.isEmpty())
							{
								if(queue_l3.peekFront()->timeIssued <= Clk)
								{
									l3_servicing = true;
									l3_s_time = 1;
								}
								else
								{
									l3_servicing = false;
									l3_s_time = 0;
								}
							}
							else
							{
								l3_servicing = false;
								l3_s_time = 0;
							}
						}
						else 				// if the L3 queue is empty, set the variables accordingly
						{
							l3_servicing = false;
							l3_s_time = 0;
						}
					}
					else
					{
						// adjust member variables of the entry
						queue_l3.peekFront()->timeMoved = Clk + 1;

						// enqueue entry into queue that will distrubute the entry to the mm_queue
						dump_queuem.enqueue(queue_l3.peekFront());

						// add the report to the string
						mm_out.append(std::to_string(Clk) + ", L3, Miss, " + std::to_string(queue_l3.peekFront()->address) + "\n");

						// dequeue entry from L3
						queue_l3.dequeue();

						// set the L3 servicing variables accordingly
						if(!queue_l3.isEmpty())
						{
							while(queue_l3.peekFront()->serviced)		// dequeue any requeusts that have already been serviced before the clock starts
							{
								queue_l3.dequeue();
								if(queue_l3.isEmpty())					// break if queue 3 is empty
								{
									break;
								}
							}

							if(!queue_l3.isEmpty())
							{
								if(queue_l3.peekFront()->timeIssued <= Clk)
								{
									l3_servicing = true;
									l3_s_time = 1;
								}
								else
								{
									l3_servicing = false;
									l3_s_time = 0;
								}
							}
							else
							{
								l3_servicing = false;
								l3_s_time = 0;
							}
						}
						else 				// if the L3 queue is empty, set the variables accordingly
						{
							l3_servicing = false;
							l3_s_time = 0;
						}
					}
				}
				else
				{
					l3_s_time++;
				}
			}
		}


		////////////////////////////////////////////////////////////////////////////////////////////
		// MM
		////////////////////////////////////////////////////////////////////////////////////////////
		
		// move from dump_qeue to mm_queue if it is the right time and print out requeust
		if(!dump_queuem.isEmpty())
		{
			while(dump_queuem.peekFront()->timeMoved == Clk)
			{
				queue_mm.enqueue(dump_queuem.peekFront());
				mm_out.append(std::to_string(Clk) + ", MM, Request, " + std::to_string(dump_queuem.peekFront()->address) + "\n");
				dump_queuem.dequeue();

				// if the queue is empty, break out of the loop
				if(dump_queuem.isEmpty())
				{
					break;
				}
			}
		}

		// Look for Hits or Misses 
		if(!queue_mm.isEmpty())		// if the queue has elements in it, check and report status
		{
			// Set the max queue length of mm
			if(queue_mm.getLength() > maxm)
			{
				maxm = queue_mm.getLength();
			}

			if(!mm_servicing)
			{
				mm_servicing = true;		// start service to the next in the queue
				mm_s_time++;
			}
			else
			{
				if(mm_s_time == 100)
				{
					hit_res = checkHit(queue_mm.peekFront()->address, MM);	// check if it is a hit after 100 clock cycles
					if(hit_res)
					{
						// adjust memoryEntry variables
						queue_mm.peekFront()->serviced = true;
						queue_mm.peekFront()->timeServiced = Clk;

						// Add the latency to the latency list
						latency_list.insert(latency_list.getLength(), Clk - queue_mm.peekFront()->timeIssued);

						// add the report to the out string
						mm_out.append(std::to_string(Clk) + ", MM, Hit, " + std::to_string(queue_mm.peekFront()->address) + "\n");

						// dequeue the memory entry from the mm cache
						queue_mm.dequeue();

						// set the MM servicing variables accordingly
						if(!queue_mm.isEmpty())
						{
							if(queue_mm.peekFront()->timeMoved <= Clk)
							{
								mm_servicing = true;
								mm_s_time = 1;
							}
							else
							{
								mm_servicing = false;
								mm_s_time = 0;
							}
						}
						else 				// if the MM queue is empty, set the variables accordingly
						{
							mm_servicing = false;
							mm_s_time = 0;
						}
					}
					else
					{
						// add the report to the string
						mm_out.append(std::to_string(Clk) + ", MM, Miss, " + std::to_string(queue_mm.peekFront()->address) + "\n");
						
						// adjust memoryEntry variables
						queue_mm.peekFront()->serviced = true;
						queue_mm.peekFront()->timeServiced = Clk;

						// Add the latency to the latency list
						latency_list.insert(latency_list.getLength(), Clk - queue_mm.peekFront()->timeIssued);

						// dequeue entry from MM
						queue_mm.dequeue();

						// set the MM servicing variables accordingly
						if(!queue_mm.isEmpty())
						{
							if(queue_mm.peekFront()->timeIssued <= Clk)
							{
								mm_servicing = true;
								mm_s_time = 1;
							}
							else
							{
								mm_servicing = false;
								mm_s_time = 0;
							}
						}
						else 				// if the MM queue is empty, set the variables accordingly
						{
							mm_servicing = false;
							mm_s_time = 0;
						}
					}
				}
				else
				{
					mm_s_time++;
				}
			}
		}

		Clk++; 			// increment the clock
	}
	return mm_out;					// return the output string
}

// Do not alter the codes below
void MemoryModel::initMemory(std::set<int> s1, std::set<int> s2, std::set<int> s3, std::set<int> smm)
{
	v1 = std::set<int>(s1);
	v2 = std::set<int>(s2);
	v3 = std::set<int>(s3);
	vmm = std::set<int>(smm);
}

bool MemoryModel::checkHit(int addr, MemoryType memoryType)
{
	bool hit = false;
	switch (memoryType)
	{
		case L1:
			hit = v1.count(addr) > 0;
			break;
		case L2:
			hit = v2.count(addr) > 0;
			break;
		case L3:
			hit = v3.count(addr) > 0;
			break;
		case MM:
			hit = vmm.count(addr) > 0;
			break;
		default:
			break;
	}
	return hit;
}
