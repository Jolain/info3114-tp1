#include "stdafx.h"
#include "bufferPool.h"
#include "hardDrive.h"
#include <vector>
#include <iostream>

bufferPool::bufferPool(int mode, hardDrive *harddrive) {
	bufferType = mode; // Toggles between bufferPool types (1 = H1; 2 = H2; 3 = disabled)
	hdd = harddrive;
	nextCellToClear = 0; // Marks the first cell of the array as next to delete
}

// Virtual destructor for compiling purposes
bufferPool::~bufferPool()
{
}

// Definitions for I/O functions

// Reads the file at the selected address
void bufferPool::readFile(int file) {

	if (fileInBuffer(file) && bufferType != 3) {
		// read from cache, no time penalty
	}
	else { // Read from HDD
		int data = hdd->readSector(file);
		cout << data << " ";
		if (buffer->size() < 5 && bufferType != 3) { // If the buffer is not full, push back the accessed data
			vMemory *temp = new vMemory;
			temp->address = file;
			temp->value = data;
			buffer->push_back(temp);
		}
		else if (bufferType == 1) { // If the buffer is full, replace the oldest data with the new one
			closeFile(buffer->at(nextCellToClear)->address); // Closes the next cell
			// Fill the cell with new data
			buffer->at(nextCellToClear)->address = file;
			buffer->at(nextCellToClear)->value = data;
			nextCellToClear = (nextCellToClear + 1) % 5; // Loops the counter to the start of the buffer
		}
		else if (bufferType == 2) { // If the buffer is full, replace the least used data with the new one
			//TODO: Do code for heuristic 2
		}
	}
}

//Writes to the selected address
void bufferPool::writeFile(int file) {
	if (fileInBuffer(file) && bufferType != 3) {
		// write to cache, no time penalty
		// Find the cell requested
		for (int i = 0; i < buffer->size(); i++) {
			if (buffer->at(i)->address == file) {
				// Randomly do +1 or -1 to the data to symbolise a data write/change
				if (rand() % 2 == 0) { buffer->at(i)->value++; }
				else { buffer->at(i)->value--; }
				buffer->at(i)->isDirty = true; // Marks the memory as "needs to be written to the disk"
			}
		}
	}
	else { // Write directly to the hdd
		hdd->writeSector(file);
	}
}

// Write changes to disk and clear memory slot
void bufferPool::closeFile(int file) {
	if (buffer->at(nextCellToClear)->isDirty) {
		hdd->writeSector(file); // Write to disk since information has changed
	}
	buffer->at(nextCellToClear)->address = NULL; // Wipe memory cell
	buffer->at(nextCellToClear)->value = NULL;
	buffer->at(nextCellToClear)->isDirty = false;
}

// Returns true if the passed address is currently in the buffer
bool bufferPool::fileInBuffer(int file) {
	for (int i = 0; i < buffer->size(); i++) {
		if (buffer->at(i)->address == file) { return true; }
	}

	return false;
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