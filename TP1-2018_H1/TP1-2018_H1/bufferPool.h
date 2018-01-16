#pragma once
#include <vector>
#include "hardDrive.h"
using namespace std;
// Simulation of a buffer pool. Contains the functions necessary to interact with
// a "hardDrive" type object. For the purpose of this assignement, the buffer pool
// acts as if it communicates with both the address bus and the data bus, even though
// the actual data contained on the address is meaningless. I find that it helps to
// visualize the actual process better.

struct vMemory {
	int address = NULL; // Memory cells initializes with a null address
	int value = NULL; // Memory cells initializes with a null value
	bool isDirty = false;
};

// Declaration of bufferPool functions and vars
class bufferPool
{
	vector<vMemory> buffer;
	hardDrive hdd; // HDD that the buffer is attached to
	int nextCellToClear;
	int bufferType;
	int time;
public:
	bufferPool(int, hardDrive);
	~bufferPool();
	void readFile(int);
	void writeFile(int);
	void closeFile(int);
	bool fileInBuffer(int);
	void markDirty(int);
	int executionTime(void);
};

