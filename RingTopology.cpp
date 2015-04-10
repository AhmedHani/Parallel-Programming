//============================================================================
// File				: Source2.cpp
// Author			: AHMED HANI IBRAHIM
// Copyright		: AHani
// Online Judge     : 
// Created on April 7, 2015, 4:16 AM
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

/*
	Sorry for the non-clean code, i was in a hurry :D
*/

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	int rank;
	int numberOfProcesses;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
	
	if (rank == 0) {
		int size = numberOfProcesses;
		int start = 0;
		int end = size - 1;
		MPI_Send(&start, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); 
		cout << "\nMaster sends element " << 0 <<  " to Process " << 1 << endl;
		MPI_Recv(&end, 1, MPI_INT, numberOfProcesses - 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		cout << "\nMaster receives element " << end <<  " from Process " << numberOfProcesses - 1 << endl;	
	} else {
		int currentRank = rank;
		int element = 0;
		MPI_Recv(&element, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		int nextElement = element + 1;
		cout << "\nProcess " << rank << " received element " << element << " from Process " << rank - 1 << endl;
		if (rank + 1 == numberOfProcesses) {
			MPI_Send(&nextElement, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			cout << "\nProcess " << currentRank << " sends element " << nextElement << " to Master" << endl; 
		} else {
			MPI_Send(&nextElement, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
			cout << "\nProcess " << currentRank << " sends element " << nextElement << " to Process " << rank + 1 << endl;
		}
	}

	MPI_Finalize();

	return 0;
}