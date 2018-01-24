#include "stdafx.h"
#include "bufferPool.h"
#include "hardDrive.h"
#include <vector>
#include <iostream>

bufferPool::bufferPool(bool bufferEnabled, hardDrive *harddrive) {
	enabled = bufferEnabled; // Bool that marks if the buffer is used or not
	hdd = harddrive; // Pointer to the linked HDD
	nextCellToClear = 0; // Marks the first cell of the array as next to delete
}

// Virtual destructor for compiling purposes
bufferPool::~bufferPool()
{
}

// Definitions for I/O functions

// Reads the file at the selected address
void bufferPool::readFile(int file) {

	if (enabled && posInBuffer(file) != -1) {
		// read from cache, no time penalty
	}
	else { // Read from HDD
		int data = hdd->readSector(file);

		if (buffer->size() < 5 && enabled) { // If the buffer is not full, push back the accessed data
			vMemory *temp = new vMemory;
			temp->address = file;
			temp->value = data;
			buffer->push_back(temp);
		}
		else if (enabled) { // If the buffer is full, replace the oldest data with the new one
			closeFile(buffer->at(nextCellToClear)->address, nextCellToClear); // TODO: Check this line, might cause problems with logic
			// Fill the cell with new data
			buffer->at(nextCellToClear)->address = file;
			buffer->at(nextCellToClear)->value = data;
			nextCellToClear = (nextCellToClear + 1) % 5; // Loops the counter to the start of the buffer
		}
	}
}

// Writes to the selected address
void bufferPool::writeFile(int file) {
	if (enabled) {
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

	if (enabled) { // Buffer is used, do buffer stuff
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
	if (buffer->at(target)->isDirty && buffer->at(target)->address != NULL) {
		writeFile(file);
	}
	buffer->at(target)->address = NULL; // Wipe memory cell
	buffer->at(target)->value = NULL;
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

void bufferPool::displayBufferStatus() {
	cout << "        Bassin de tampons" << endl;
	cout << "  0      1      2      3      4" << endl;
	cout << "#####  #####  #####  #####  #####" << endl;
	for (int i = 0; i < 5; i++) {
		if ((i < buffer->size()) && (buffer->at(i)->value != NULL)) {
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