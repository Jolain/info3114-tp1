// TP1-2018_H1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hardDrive.h"
#include "bufferPool.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
using namespace std;

struct instruction { // Struct for the instructions to send to the bufferPool
	char type;
	int address;
};

// Additional display functions declaration
void displayProgram(instruction prg[]);
void runProgram(instruction prg[], bufferPool target);
void generateProgram();

// Declare the 3 programs to emulate
instruction programme1[10000];
instruction programme2[10000];
instruction programme3[10000];

// Array that will hold the 4 different buffer pools required for the assignement
int main()
{
	// Generate a random seed to get different values every time
	srand(rand() + time(NULL));

	// Init of the program
	cout << "##############################" << endl;
	cout << "#       Poirier, Jolain      #" << endl;
	cout << "#          A00192864         #" << endl;
	cout << "##############################" << endl;

	cout << endl;
	cout << "======== Heuristique 1 =======" << endl;

	// Construct a HDD with random data
	hardDrive *hdd = new hardDrive();
	// Declare the buffer pools
	bufferPool *buffer1 = new bufferPool(true, hdd); // Buffer pool enabled
	bufferPool *buffer2 = new bufferPool(false, hdd);// Buffer pool disabled
	
	generateProgram();

	int timeWithBuffer, timeWithoutBuffer;

	// Main program loop
	while (true) {
		// Display the buffer status
		cout << endl;
		buffer1->displayBufferStatus();

		// Reset the values contained in the buffers
		buffer1->flush();
		buffer2->flush();

		cout << endl << "(Q)uitter | (E)xecuter la simulation | (R)egenerer les 3 programmes" << endl << "Entrez une commande: ";
		char option;
		bool loop = true;
		// Input checking
		while (loop) {
			cin >> option;
			if (isalpha(option)) {
				switch (toupper(option)) {
					case 'Q': return 0; // Exit program
					case 'E': { // Continue normal execution
						loop = false;
						break;
					}
					case 'R': { // Create new values for the 3 programs
						generateProgram();
						cout << "Programmes regenerer." << endl << "Entrez une commande: ";
						break;
					}
					
					default: { // Invalid character entered
						cout << "Charactere invalide. Entrez une commande: ";
						break;
					}
				}
			}
			else {
				cout << "Charactere invalide. Entrez une commande: ";
			}
			cin.clear();
		}
		
		int prgToRun;
		cout << endl << "Entrez le programme a executer [1-3]: ";
		while (true) {
			if(cin >> prgToRun && (prgToRun <= 3 && prgToRun >= 1)) {
				break;
			}
			else {
				cout << "Caractere invalide. Entrez un chiffre inclus dans [1-3]: ";
			}
			cin.clear();
		}

		cout << endl << "Execution du programme " << prgToRun << endl;
		cout << "---------------------------------------" << endl;
		
		if (prgToRun == 1) {
			// Run with the buffer first, then without the buffer
			hdd->resetLatency(); // Clear leftover latency for a new run
			runProgram(programme1, *buffer1);
			timeWithBuffer = buffer1->executionTime();
			hdd->resetLatency(); // Reset the time for the second run
			runProgram(programme1, *buffer2); 
			timeWithoutBuffer = buffer2->executionTime();
		}
		else if (prgToRun == 2) {
			// Run with the buffer first, then without the buffer
			hdd->resetLatency(); // Clear leftover latency for a new run
			runProgram(programme2, *buffer1);
			timeWithBuffer = buffer1->executionTime();
			hdd->resetLatency(); // Reset the time for the second run
			runProgram(programme2, *buffer2);
			timeWithoutBuffer = buffer2->executionTime();
		}
		else if (prgToRun == 3) {
			// Run with the buffer first, then without the buffer
			hdd->resetLatency(); // Clear leftover latency for a new run
			runProgram(programme3, *buffer1);
			timeWithBuffer = buffer1->executionTime();
			hdd->resetLatency(); // Reset the time for the second run
			runProgram(programme3, *buffer2);
			timeWithoutBuffer = buffer2->executionTime();
		}
		
		// Calculate the effiency gained
		// Multiply by 100 because we want to display as % and we need to round the result
		float effiency = ((float)timeWithoutBuffer / (float)timeWithBuffer) * 100;
		effiency = (roundf(effiency * 100) / 100) - 100; 
		// Display the results
		cout << "Temps d'execution avec buffer: " << timeWithBuffer << " ms" << endl;
		cout << "Temps d'execution sans buffer: " << timeWithoutBuffer << " ms" << endl;
		cout << "Efficacite gagner avec buffer: " << effiency << "%" << endl;
		cout << "---------------------------------------" << endl;
	}

    return 0;
}

// Displays the instructions contained in a program
void displayProgram(instruction prg[]) {
	for (int i = 0; i < 10; i++) {
		cout << prg[i].type << prg[i].address << "; ";
	}
	cout << endl;
}

// Runs the selected program on the bufferPool
void runProgram(instruction prg[], bufferPool target) {
	for (int i = 0; i < 10000; i++) {
		if (prg[i].type == 'r') {
			target.readFile(prg[i].address);
		}
		else if (prg[i].type == 'w') {
			target.writeFile(prg[i].address);
		}
		else if (prg[i].type == 'm') {
			target.modify(prg[i].address);
		}
	}
}

// Generates 3 random programs
void generateProgram() {
	int temp;
	for (int i = 0; i < 10000; i++) {
		temp = rand() % 3;
		switch (temp) {
		case 0:
			programme1[i].type = 'r'; // Read instruction
			break;
		case 1:
			programme1[i].type = 'w'; // Write instruction
			break;
		case 2:
			programme1[i].type = 'm'; // Modify instruction
			break;
		}
		programme1[i].address = rand() % 19; // Random address from 0-19
	}
	for (int i = 0; i < 10000; i++) {
		temp = rand() % 3;
		switch (temp) {
		case 0:
			programme2[i].type = 'r';
			break;
		case 1:
			programme2[i].type = 'w';
			break;
		case 2:
			programme2[i].type = 'm';
			break;
		}
		programme2[i].address = rand() % 19; // Random address from 0-19
	}
	for (int i = 0; i < 10000; i++) {
		temp = rand() % 3;
		switch (temp) {
		case 0:
			programme3[i].type = 'r';
			break;
		case 1:
			programme3[i].type = 'w';
			break;
		case 2:
			programme3[i].type = 'm';
			break;
		}
		programme3[i].address = rand() % 19; // Random address from 0-19
	}
}