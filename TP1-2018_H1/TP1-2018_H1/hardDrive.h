#pragma once
// Simulates data access times to a HDD. All the defined functions returns the time
// in ms required to do the demanded action.
class hardDrive
{
	int sectors[4][5];  // 4 tracks with 5 sectors each
	int headPosition; // Stores the current track on which the head is aligned
	int executionTime;
public:
	hardDrive();
	~hardDrive();
	void alignHead(int);
	int readSector(int);
	void writeSector(int);
	void writeSector(int, int);
	int getLatency(void);
	void resetLatency(void);
};

