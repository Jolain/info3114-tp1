#include "stdafx.h"
#include "hardDrive.h"
#include "cstdlib"


hardDrive::hardDrive()
{
	executionTime = 0; // Sets the latency to 0
	headPosition = 0; // Sets the head position to default.
	sectorPosition = 0; // Sets the sector position to default.
	
	//Initialise random values from 0-19 as data in all cells
	for(int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			sectors[i][j] = rand() % 20; // Any n % 20 will give a number included in [0-19]
		}
	}
}

hardDrive::~hardDrive()
{
}

/*
NOTE: To be as accurate as possible, we need to emulate the rotation of the disk
as operations are performed. To do this, we need to assume that the operations to
the buffer are instantaneous and that the disk does not have time to rotate in between
calls to the buffer. With that assumption, we can now process the time required for the
disk to rotate to the required sector when a read or write operation is performed.

Since a full rotation of the disk requires 10ms and there's 5 sectors, it takes 2ms
for the disk to move to the next sector. However, since the disk does not stop spinning,
whenever we need to do an operation that requires time we need to update the 'sectorPosition'
value to (sectorPosition + 1) % 5
*/


// Sets the head of the HDD to the correct position and calculates the time required to do so
void hardDrive::alignHead(int track) {
	// Determine in which direction the head should move (+/-)
	while(track != headPosition) {
		if(headPosition > track) {
			executionTime += 2; // Move the head
			headPosition--;
			sectorPosition = (sectorPosition + 1) % 5; // Spin the disk
		}
		else if(headPosition < track) {
			executionTime += 2; // Move the head
			headPosition++;
			sectorPosition = (sectorPosition + 1) % 5; // Spin the disk
		}
		else { break; } // Something funky is happening if the while gets here
	}

}

// Returns the data held at the requested sector. Calculates the time required.
int hardDrive::readSector(int sector) {
	int track = sector / 5; // Since C++ truncates towards 0 for integers division, this will give the correct track on which the sector is
	int section = sector % 5; // The remainder of the division will give the sector on the track found above
	alignHead(track); //Align the head to the track
	waitForSector(section); // Wait for the disk to catch up

	executionTime += 2; // Read the data and spin the disk
	sectorPosition = (sectorPosition + 1) % 5;

	return sectors[track][section]; // Returns the requested data
}

// Writes either +1 or -1 to the data held at the requested sector. Calculates the time required.
void hardDrive::writeSector(int sector) {
	int track = sector / 5; // Since C++ truncates towards 0 for integers division, this will give the correct track on which the sector is
	int section = sector % 5; // The remainder of the division will give the sector on the track found above
	alignHead(track); //Align the head to the track
	waitForSector(section); // Wait for the disk to catch up
	
	executionTime += 2; // Writes the data and spin the disk
	sectorPosition = (sectorPosition + 1) % 5;
	// Since the actual data is irrelevant, to symbolises the writes the program will randomly do +1 or -1 to the data
	if(rand() % 2 == 0) { sectors[track][section]++; }
	else { sectors[track][section]--; }
}

// Writes the sent data to the requested sector. Calculates the time required.
void hardDrive::writeSector(int sector, int value) {
	int track = sector / 5; // Since C++ truncates towards 0 for integers division, this will give the correct track on which the sector is
	int section = sector % 5; // The remainder of the division will give the sector on the track found above
	alignHead(track); //Align the head to the track
	waitForSector(section); // Wait for the disk to catch up

	executionTime += 2; // Writes the data and spin the disk
	sectorPosition = (sectorPosition + 1) % 5;

	sectors[track][section] = value;
}

// Simulates the time required for the disk to spin the sector needed under the head
void hardDrive::waitForSector(int sector) {
	while (sectorPosition != sector) {
		sectorPosition = (sectorPosition + 1) % 5;
		executionTime += 2;
	}
}

// Returns the total execution time of the operations requested (in ms)
int hardDrive::getLatency() {
	return executionTime;
}

// Resets the latency recorded on the HDD
void hardDrive::resetLatency() {
	executionTime = 0;
}