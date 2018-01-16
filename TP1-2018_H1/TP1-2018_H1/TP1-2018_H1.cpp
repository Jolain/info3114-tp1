// TP1-2018_H1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hardDrive.h"
#include "bufferPool.h"
#include <iostream>
#include <vector>
using namespace std;

int main()
{
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

