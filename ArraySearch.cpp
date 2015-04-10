//============================================================================
// File				: Source.cpp
// Author			: AHMED HANI IBRAHIM
// Copyright		: AHani
// Online Judge     : 
// Created on April 6, 2015, 8:12 PM
//============================================================================

#include <climits>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <functional>
#include <Windows.h>
#include <ctime>
#include "mpi.h"

#define Max 50 + 5
#define INF 1000000000
#define INT_MAX 2147483647
#define FOR(i, N) for(int i = 0; i < N; i++)
#define FOR1e(i, N) for(int i = 1; i <= N; i++)
#define FORe(i, N) for(int i = 0; i <= N; i++)

#pragma warning(disable : 4996)

using namespace std;

int* inputArray;
int arraySize;
int subArraysSize;
int targetValue;
MPI_Status* recieveStatus = new MPI_Status();

void input() {
	puts("Enter array size:");
	cin >> arraySize;
	inputArray = new int[arraySize];

	puts("Enter array elements:");

	for (int i = 0; i < arraySize; i++) {
		cin >> inputArray[i];
	}

	puts("Enter The Target Value:");
	cin >> targetValue;

	puts("\n");
}

void master(int processRank) {
	subArraysSize = arraySize / processRank; //... The size of each of array size
	int remaining = arraySize % processRank; //... The remaining elements size that will be added to the slaves
	int offset = 0;//... Array start index

	for (int i = 0; i < processRank; i++) {
		int arraySize = subArraysSize;

		if (remaining) {
			arraySize++;
			MPI_Send(&arraySize, 1, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD);
			MPI_Send(inputArray + offset, arraySize, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD);
			remaining--;
		}//... If there are remain, add the elements one by one to the slaves from the begin
		
		else {
			MPI_Send(&arraySize, 1, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD);
			MPI_Send(inputArray + offset, arraySize, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD);
		}//... Continue sending sub-arrays to slaves

		MPI_Send(&targetValue, 1, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD); //... Send the target value to slaves

		offset += arraySize; //... Update the array offset
	}

	bool found = false;

	for (int i = 0; i < processRank; i++) {
		int localIndex = 0;

		//... Retrieve the index from the slaves
		MPI_Recv(&localIndex, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, recieveStatus);

		//... Get the slave ID (1 based) to help on getting the original index of the target
		int source = recieveStatus->MPI_SOURCE;

		if (localIndex != -1) {
			//... Get the original index from the global array
			int originalIndex = (source - 1) * (arraySize / (processRank)) + localIndex;
			originalIndex += (source - 1); 

			printf("Target is found in the process %d\n", source);
			printf("Target Index is %d", originalIndex);

			found = true;
			break;
		} 
	}

	if(!found) puts("Element Not Found!");
}

void slave(int slaveRank) {
	bool found = false;
	int localArraySize = 0;

	//...  Get the subarray size
	MPI_Recv(&localArraySize, 1, MPI_INT, 0, slaveRank, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

	int* subArray = new int[localArraySize];

	//... Get the subarray elements
	MPI_Recv(subArray, localArraySize, MPI_INT, 0, slaveRank, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

	printf("Process %d recieved %d element(s)\n", slaveRank, localArraySize);

	int targetIndex = -1;
	int localTargetValue = 0;

	//... Get the target value to search for it
	MPI_Recv(&localTargetValue, 1, MPI_INT, 0, slaveRank, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

	for (int i = 0; i < localArraySize; i++) {
		if (subArray[i] == localTargetValue) {
			found = true;
			targetIndex = i;
			break;
		} //... IF the target is found, terminate the loops and get the index
	}

	MPI_Send(&targetIndex, 1, MPI_INT, 0, slaveRank, MPI_COMM_WORLD); //... Send the index to the Master

	delete[] subArray;
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	int rank;
	int numberOfProcesses;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

	if (rank == 0) {
		input();
		master(numberOfProcesses - 1);

	} else {
		slave(rank);
	}

	MPI_Finalize();

	return 0;
}