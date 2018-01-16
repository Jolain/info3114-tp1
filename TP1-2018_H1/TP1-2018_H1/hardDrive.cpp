#include "stdafx.h"
#include "hardDrive.h"
#include "cstdlib"


hardDrive::hardDrive()
{
	executionTime = 0; // Sets the latency to 0
	headPosition = 0; // Sets the head position to default.
	
	//Initialise random values from 0-19 as data in all cells
	for(int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			sectors[i][j] = rand() % 20; // Any n % 20 will give a number included in [0-19]
		}
	}
}

// Virtual destructor for compiling purposes
hardDrive::~hardDrive()
{
}

// Sets the head of the HDD to the correct position and calculates the time required to do so
void hardDrive::alignHead(int track) {
	// Determine in which direction the head should move (+/-)
	while(track != headPosition) {
		if(headPosition > track) {
			executionTime += 2; // Move the head
			headPosition--;
		}
		else if(headPosition < track) {
			executionTime += 2; // Move the head
			headPosition++;
		}
		else { break; } // Something funky is happening if the while gets here
	}

}

// Returns the data held at the requested sector. Calculates the time required.
int hardDrive::readSector(int sector) {
	int track = sector / 5; // Since C++ truncates towards 0 for integers division, this will give the correct track on which the sector is
	int section = sector % 5; // The remainder of the division will give the sector on the track found above
	alignHead(track); //Align the head to the track
	
	executionTime += 2; // Read the data
	return sectors[track][section]; // Returns the requested data
}

// Writes either +1 or -1 to the data held at the requested sector. Calculates the time required.
void hardDrive::writeSector(int sector) {
	int track = sector / 5; // Since C++ truncates towards 0 for integers division, this will give the correct track on which the sector is
	int section = sector % 5; // The remainder of the division will give the sector on the track found above
	alignHead(track); //Align the head to the track
	
	executionTime += 2; // Writes the data
	// Since the actual data is irrelevant, to symbolises the writes the program will randomly do +1 or -1 to the data
	if(rand() % 2 == 0) { sectors[track][section]++; }
	else { sectors[track][section]--; }
}

// Writes the sent data to the requested sector. Calculates the time required.
void hardDrive::writeSector(int sector, int value) {
	int track = sector / 5; // Since C++ truncates towards 0 for integers division, this will give the correct track on which the sector is
	int section = sector % 5; // The remainder of the division will give the sector on the track found above
	alignHead(track); //Align the head to the track

	executionTime += 2; // Writes the data
	sectors[track][section] = value;
}


// Returns the total execution time of the operations requested (in ms)
int hardDrive::getLatency() {
	return executionTime;
}

// Resets the latency recorded on the HDD
void hardDrive::resetLatency() {
	executionTime = 0;
}