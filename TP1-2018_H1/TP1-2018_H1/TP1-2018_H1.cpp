// TP1-2018_H1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hardDrive.h"
#include "bufferPool.h"
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

// Declare the 3 programs to emulate
instruction programme1[10];
instruction programme2[10];
instruction programme3[10];


// Array that will hold the 4 different buffer pools required for the assignement
int main()
{
	int mode;
	// Generate a random seed to get different values every time
	srand(rand() + time(NULL));

	// Init of the program
	cout << "##############################" << endl;
	cout << "#       Poirier, Jolain      #" << endl;
	cout << "#          A00192864         #" << endl;
	cout << "#      Heuristique 1 & 2     #" << endl;
	cout << "##############################" << endl;

	cout << "Pour simplicite, l'heuristique 1 et 2 se trouvent dans le meme" << endl;
	cout << "programme. Entrez '1' pour l'heuristique 1 et '2' pour l'heuristique '2'." << endl;

	// Entry checking
	while (true) {
		cin >> mode;
		if (mode != 1 && mode != 2) {
			cout << "Entrez une valeur entre 1 et 2: ";
			mode = NULL;
		}
		else { break; }
	}

	cout << endl;
	cout << "==================== Heuristique " << mode << " ====================" << endl;
	cout << "** Redemarer la simulation pour changer l'heuristique **" << endl << endl;

	// Construct a HDD with random data
	hardDrive *hdd = new hardDrive();
	// Declare the buffer pools
	bufferPool *buffer1 = new bufferPool(mode, hdd);
	bufferPool *buffer2 = new bufferPool(3, hdd);// Buffer pool disabled

	// Fill the programs with random instructions
	for (int i = 0; i < 10; i++) {
		if (rand() % 2 == 0) {
			programme1[i].type = 'r'; // Read instruction
		}
		else { programme1[i].type = 'w'; } // Write instruction
		programme1[i].address = rand() % 19; // Random address from 0-19
	}
	for (int i = 0; i < 10; i++) {
		if (rand() % 2 == 0) {
			programme2[i].type = 'r'; // Read instruction
		}
		else { programme2[i].type = 'w'; } // Write instruction
		programme2[i].address = rand() % 19; // Random address from 0-19
	}
	for (int i = 0; i < 10; i++) {
		if (rand() % 2 == 0) {
			programme3[i].type = 'r'; // Read instruction
		}
		else { programme3[i].type = 'w'; } // Write instruction
		programme3[i].address = rand() % 19; // Random address from 0-19
	}

	int timeWithBuffer, timeWithoutBuffer;

	// Main program loop
	while (true) {
		// Display the available programs
		buffer1->displayBufferStatus();
		cout << "Programme #1: ";
		displayProgram(programme1);
		cout << "Programme #2: ";
		displayProgram(programme2);
		cout << "Programme #3: ";
		displayProgram(programme3);

		cout << endl << "Entrer le programme a executer (1-3): ";
		int prgToRun;
		while (true) {
			cin >> prgToRun;
			if (mode != 1 && mode != 2 && prgToRun != 3) {
				cout << "Entrez une valeur entre 1 et 3: ";
				prgToRun = NULL;
			}
			else { break; }
		}
		cout << endl << "Execution du programme " << prgToRun << endl;
		cout << "------------------------" << endl;
		
		if (prgToRun == 1) {
			// Run with the buffer first, then without the buffer
			hdd->resetLatency(); // Clear leftover latency for a new run
			runProgram(programme1, *buffer1);
			timeWithBuffer = buffer1->executionTime();
			hdd->resetLatency(); // Reset the time for the second run
			runProgram(programme1, *buffer2); 
			timeWithoutBuffer = buffer2->executionTime();

			// Display the results
			cout << "Temps d'execution avec buffer (H" << mode << "): "
				<< timeWithBuffer << " ms" << endl;
			cout << "Temps d'execution sans buffer (H" << mode << "): "
				<< timeWithoutBuffer << " ms" << endl;
		}
		else if (prgToRun == 2) {

		}
		else if (prgToRun == 3) {

		}


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
	for (int i = 0; i < 10; i++) {
		if (prg[i].type == 'r') {
			target.readFile(prg[i].address);
		}
		else if (prg[i].type == 'w') {
			target.writeFile(prg[i].address);
		}
	}
}