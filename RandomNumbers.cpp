//============================================================================
// File				: Source.cpp
// Author			: AHMED HANI IBRAHIM
// Copyright		: AHani
// Online Judge     : 
// Created on March 22, 2015, 7:23 PM
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

void send(int* buffer, int tag) {
	MPI_Send(buffer, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
}

void recieve(int* buffer, int numberOfProcesses)  {
	FOR(i, numberOfProcesses) {
		MPI_Recv(&buffer[i], 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}

void getCurrentTime() {
	time_t currentTime = time(NULL);
	char* timeValue = ctime(&currentTime);

	printf("I am Master!\n%s\n", timeValue);
}

void master() {
	getCurrentTime();
}

void slave(int slaveID) {
	srand(slaveID);

	FOR(i, 5) printf("%d\n", rand());
}


int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	int rank;
	int numberOfProcesses;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
	
	if (rank == 0) {
		master();
		int* buffer = new int[numberOfProcesses - 1];
		recieve(buffer, numberOfProcesses - 1);
		int sum = 0;

		FOR(i, numberOfProcesses - 1) {
			sum += buffer[i];
		}

		if (sum == numberOfProcesses - 1) {
			master();
		}
	} else {
		slave(rank);
		
		int ok = 1;
		int tag = rank;

		send(&ok, tag);
	}

	MPI_Finalize();

	return 0;
}