#include "stdafx.h"
#include "bufferPool.h"
#include "hardDrive.h"
#include <vector>

bufferPool::bufferPool(int mode) {
	bufferType = mode; // Toggles between bufferPool types (1 = H1; 2 = H2; 3 = disabled)
	nextCellToClear = 0; // Marks the first cell of the array as next to delete
}


bufferPool::~bufferPool()
{
}

// Definitions for I/O functions

// Reads the file at the selected address
void bufferPool::readFile(int file) {
	if (fileInBuffer(file)) { 
		// read from cache, no time penalty
	}
	else { // read from HDD
		int data = hdd.readSector(file);
		if (buffer.size() < 5) { // If the buffer is not full, push back the accessed data
			buffer.push_back(vMemory(file, data, false));
		}
		else { // If the buffer is full, replace the oldest data with the new one
			closeFile(buffer[nextCellToClear].address); // Closes the next cell
			// Fill the cell with new data
			buffer[nextCellToClear].address = file;
			buffer[nextCellToClear].value = data;
			nextCellToClear = (nextCellToClear + 1) % 5; // Loops the counter to the start of the buffer
		}
	}
}

//Writes to the selected address
void bufferPool::writeFile(int file) {
	if (fileInBuffer(file)) {
		// write to cache, no time penalty
		/* Add code that changes the data */
	}
}

// Write changes to disk and clear memory slot
void bufferPool::closeFile(int file) {

}

// Returns true if the passed address is currently in the buffer
bool bufferPool::fileInBuffer(int file) {
	
}

// Flags a memory cell as dirty (needs to be written to disk)
void bufferPool::markDirty(int cell) {

}

// Returns the amount of time needed to do the operation in ms
int bufferPool::executionTime() {
	return time;
}