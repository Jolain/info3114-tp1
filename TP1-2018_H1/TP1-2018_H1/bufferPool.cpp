#include "stdafx.h"
#include "bufferPool.h"
#include "hardDrive.h"
#include <vector>
#include <iostream>

// Constructor && Destructor
bufferPool::bufferPool(int mode, hardDrive *harddrive) {
	bufferType = mode; // Toggle between bufferPool types (1 = H1; 2 = H2; 3 = disabled)
	hdd = harddrive; // Pointer to the linked HDD
	nextCellToClear = 0; // Marks the first cell of the array as next to delete
}

bufferPool::~bufferPool()
{
}

// Definitions for I/O functions

// Reads the file at the selected address
void bufferPool::readFile(int file) {

	if (bufferType != 3 && posInBuffer(file) != -1) {
		// read from cache, no time penalty
		if(bufferType == 2) { usageMatrix[file] ++; } // Log the access
	}
	else { // Read from HDD
		int data = hdd->readSector(file);

		if (bufferType != 3 && buffer->size() < 5) { // If the buffer is not full, push back the accessed data
			vMemory *temp = new vMemory;
			temp->address = file;
			temp->value = data;
			buffer->push_back(temp);
		}
		else if (bufferType == 1) { // If the buffer is full, replace the oldest data with the new one
			closeFile(buffer->at(nextCellToClear)->address, nextCellToClear); // TODO: Check this line, might cause problems with logic
			// Fill the cell with new data
			buffer->at(nextCellToClear)->address = file;
			buffer->at(nextCellToClear)->value = data;
			nextCellToClear = (nextCellToClear + 1) % 5; // Loops the counter to the start of the buffer
		}
		else if (bufferType == 2) { // If the buffer is full, replace the least used data with the new one

			// TODO: Heuristic 2

		}
	}
}

// Writes to the selected address
void bufferPool::writeFile(int file) {
	if (bufferType != 3) {
		int pos = posInBuffer(file); // Check if the file requested is in the buffer
		if (pos != -1) { // If the file is in the buffer & dirty, write it to the disk
			if (buffer->at(pos)->isDirty == true) { hdd->writeSector(file, buffer->at(pos)->value); }
		}
		// Otherwise do nothing, the write operation is not necessary
		 
	}
	else { hdd->writeSector(file); } // Write directly to disk if the buffer is not used
}

// Modifies a value either on the buffer or on the HDD directly depending on buffer mode
void bufferPool::modify(int file) {

	if (bufferType != 3) { // Buffer is used, do buffer stuff
		int pos = posInBuffer(file);
		if (pos == -1) { 
			readFile(file); // Load the file in the buffer if it's not present
			pos = posInBuffer(file); // Get the pos of the newly added file
		} 

		// Modify the value contained in the buffer by +1/-1 to represent a change of value
		if (rand() % 2 == 0) { buffer->at(pos)->value++; }
		else { buffer->at(pos)->value--; }
		markDirty(pos); // Mark the cell as dirty
	}
	else { hdd->writeSector(file); } // Act like a 'write' instruction if the buffer is not used

	
}

// Write changes to disk and clear memory slot
void bufferPool::closeFile(int file, int target) {
	if (buffer->at(target)->isDirty && buffer->at(target)->address != -1) {
		writeFile(file);
	}
	buffer->at(target)->address = -1; // Wipe memory cell
	buffer->at(target)->value = -1;
	buffer->at(target)->isDirty = false;
}

// Clears the bufferPool and writes to disk if necessary
void bufferPool::flush() {
	for (int i = 0; i < buffer->size(); i++) {
		closeFile(buffer->at(i)->address, i); // Write to disk if necessary
	}
	nextCellToClear = 0; // Reset the cell counter
	buffer->clear(); // Delete the memory cells to allow new ones to be generated
}

// If the file is present in the buffer, return the pos of the file in the memory array
int bufferPool::posInBuffer(int file) {
	for (int i = 0; i < buffer->size(); i++) {
		if (buffer->at(i)->address == file) { return i; }
	}

	return -1; // If not found, return -1
}

// Flags a memory cell as dirty (needs to be written to disk)
void bufferPool::markDirty(int cell) {
	buffer->at(cell)->isDirty = true;
}

// Returns the amount of time needed to do the operations in ms
int bufferPool::executionTime() {
	return hdd->getLatency();
}

// Displays what is currently held in the buffer
void bufferPool::displayBufferStatus() {
	cout << "        Bassin de tampons" << endl;
	cout << "  0      1      2      3      4" << endl;
	cout << "#####  #####  #####  #####  #####" << endl;
	for (int i = 0; i < 5; i++) {
		if ((i < buffer->size()) && (buffer->at(i)->value != -1)) {
			cout << "  " << buffer->at(i)->address << "    ";
		}
		else { cout << " NIL   ";  }
	}
	cout << endl << "#####  #####  #####  #####  #####";
	cout << endl << endl;
}

// New definitions for operations
// fileBuffer.open(int) -> copie le fichier du harddrive vers le buffer
// fileBuffer.close() -> ecrit tout les fichiers au HD
// fileBuffer.write(int) -> si dirtyBit == true, ecrire sur HD
// fileBuffer.modify(int) -> set dirtyBit == true