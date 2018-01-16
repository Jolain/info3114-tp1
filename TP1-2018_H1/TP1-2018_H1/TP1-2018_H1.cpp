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
bufferPool pool[4];
int main()
{
	// Generate a random seed to get different values every time
	srand(rand() + time(NULL));

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

	// Main part of the program
	bufferPool h1(1); // Buffer Pool in "First In, First Out" configuration
	for (int i = 0; i <= 19; i++) {
		h1.readFile(i);
	}
	cout << endl;
	
	bufferPool h2(3); // Buffer Pool disabled
	for (int i = 0; i <= 19; i++) {
		h2.readFile(i);
	}
	cout << endl;
	cout << "Press any key to continue...";
	int data;
	cin >> data;

	bufferPool h3(2); // Buffer Pool in "Remove Least Used" configuration
	bufferPool h4(3); // Buffer Pool disabled


    return 0;
}

