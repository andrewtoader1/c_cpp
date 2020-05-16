#ifndef _MEMORY_ENTRY_H
#define _MEMORY_ENTRY_H

#include <iostream>

/*
 * Container struct for entries in the MemoryQueue
 */
struct MemoryEntry{
	int timeIssued;						// When was this entry issued
	int address;						// What address does this entry refer to?
	bool serviced;						// Has this entry been completed?
	int timeServiced;					// When was this entry completed (-1 if not)
	int timeMoved;						// When was this element moved to a different cache
	int cacheToService;					// When moving the entry to other queues, specify which queue to move it to so it is easier to print out the request
	MemoryEntry(){						// Default Constructor
		timeIssued = -1;
		address = -1;
		serviced = false;
		timeServiced = -1;
		timeMoved = -1;
		cacheToService = 1;
	}
	MemoryEntry(int time, int addr){	// Constructor
		timeIssued = time;
		address = addr;
		serviced = false;
		timeServiced = -1;
		timeMoved = -1;
		cacheToService = 1;
	}
	friend std::ostream& operator << (std::ostream& o, MemoryEntry m){
      	o 	<< "(" << m.timeIssued
			<< "," << m.address
			<< "," << (m.serviced ? "true":"false")
			<< "," << m.timeServiced
			<< ")";
      	return o;
	}
};

#endif
