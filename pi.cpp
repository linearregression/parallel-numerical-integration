#include <iostream>
#include <iomanip>
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int init(int *argc, char *argv[]) {
	if (*argc < 2) {
		MPI_Abort(MPI_COMM_WORLD, 1);
		return 1;
	}
	
	if (MPI_Init(argc, &argv) != MPI_SUCCESS) {
		cout << "Error initializing MPI!" << endl;
		return 1;
	}
	
	return 0;
}

void getRankAndSize(int *rank, int *size) {
	MPI_Comm_rank(MPI_COMM_WORLD, rank);
	MPI_Comm_size(MPI_COMM_WORLD, size);
}

void getSlicesToCompute(const int *slices, const int *rank, const int *size, int *first, int *last) {
	int slicesPerProcess = *slices / *size;
	*first = *rank * slicesPerProcess + 1;
	if (*rank < *size - 1) {
		*last = *first + slicesPerProcess - 1;
	}
	else {
		*last = *slices;
	}
}

//function to integrate
double function(const double *x) {
	return 4/(1+(*x)*(*x));
}

double integrate(const int *n, const int *first, const int *last) {
	const double A = 0;
	const double B = 1;
	
	double result = 0;
	double h = (B-A)/ *n;

	for (int i = *first; i <= *last; i++) {
		double x1 = (i-1)*h + A;
		double x2 = i*h + A;
		result += (function(&x1) + function(&x2))*(h/2);
	}	
	
	return result;
}

int main(int argc, char *argv[]) {	
	//Exit if init failed
	if (init(&argc, argv)) return 1;		
	
	//Get number of slices for integration as an int
	int slices = atoi(argv[1]);		
	
	//Get rank and size from MPI
	int rank, size;
	getRankAndSize(&rank, &size);
	
	MPI_Barrier(MPI_COMM_WORLD);	
	double start = MPI_Wtime();
	
	//Compute first and last slice number (both inclusive) to compute
	int first, last;
	getSlicesToCompute(&slices, &rank, &size, &first, &last);
		
	double partialResult = integrate(&slices, &first, &last);
	
	double result;
    MPI_Reduce(&partialResult, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (!rank) {
		double time = MPI_Wtime() - start;
		cout << setprecision (12) << fixed << "Process " << rank << " computed " << result << " in " << time << " s " << endl;
	}	
	MPI_Finalize();
	return 0; 
}