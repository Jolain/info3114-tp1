// TP1-2018_H1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hardDrive.h"
#include "bufferPool.h"
#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

struct instruction { // Struct for the instructions to send to the bufferPool
	char type;
	int address;
};

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
	// Construct a HDD with random data
	hardDrive hdd;

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

	// Init of the program
	cout << "##############################" << endl;
	cout << "#       Poirier, Jolain      #" << endl;
	cout << "#          A00192864         #" << endl;
	cout << "#      Heuristique 1 & 2     #" << endl;
	cout << "##############################" << endl;

	cout << "Pour simplicité, l'heuristique 1 et 2 se trouvent dans le même" << endl;
	cout << "programme. Entrez '1' pour l'heuristique 1 et '2' pour l'heuristique '2'." << endl;
	
	// Entry checking
	while (true) {
		cin >> mode;
		if (mode != 1 && mode != 2) {
			cout << "Entrez une valeur entre 1 et 2";
		}
		else { break; }
	}

	cout << endl;
	cout << "==================== Heuristique " << mode << " ====================" << endl;
	cout << "** Redémarer la simulation pour changer l'heuristique **" << endl << endl;

	// Main loop
	bufferPool buffer1(mode, hdd);
	bufferPool buffer2(3, hdd); // Buffer pool disabled

	while (true) {

	}

    return 0;
}