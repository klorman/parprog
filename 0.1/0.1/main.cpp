#include <stdio.h>
#include <cstdlib>
#include <mpi.h>

void mpi_0_1(int argc, char* argv[]);
void mpi_0_2(int argc, char* argv[]);
void mpi_0_3(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	//mpi_0_1(argc, argv);
	//mpi_0_2(argc, argv);
	mpi_0_3(argc, argv);
}

void mpi_0_1(int argc, char* argv[]) {
	int commsize = 0;
	int my_rank = 0;

	if (int rc = MPI_Init(&argc, &argv)) {
		printf("Start-up error!\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &commsize);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	printf("Hello World! My rank = %d, Communicator size = %d\n", my_rank, commsize);

	MPI_Finalize();
}

void mpi_0_2(int argc, char* argv[]) {
	int commsize = 0;
	int my_rank = 0;
	double startwtime = 0;
	double endwtime = 0;

	long long n = std::atoll(argv[1]);

	if (n < 0) {
		printf("N must be non-negative\n");
		MPI_Finalize();
	}

	if (int rc = MPI_Init(&argc, &argv)) {
		printf("Start-up error!\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &commsize);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (my_rank == 0) {
		startwtime = MPI_Wtime();
	}

	MPI_Bcast(&n, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

	long double drobSum = 0;
	for (long long i = my_rank; i <= n; i += commsize) {
		if (i != 0) {
			drobSum += 1.0f / i;
		}
	}

	long double result = 0;
	MPI_Reduce(&drobSum, &result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (my_rank == 0) {
		printf("Sum result = %Lf\n", result);

		endwtime = MPI_Wtime();
		printf("Execution time = %lf sec\n", endwtime - startwtime);
	}

	MPI_Finalize();
}

void mpi_0_3(int argc, char* argv[]) {
	int commsize = 0;
	int my_rank = 0;
	MPI_Status status = {};
	int tag = 1;

	if (int rc = MPI_Init(&argc, &argv)) {
		printf("Start-up error!\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &commsize);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int i = 0;

	if (my_rank != 0) {
		//printf("My rank = %d, start recv\n", my_rank);
		MPI_Recv(&i, 1, MPI_INT, my_rank - 1, tag, MPI_COMM_WORLD, &status);
	}

	printf("My rank = %2d, i = %2d\n", my_rank, ++i);

	//printf("My rank = %d, start ssend\n", my_rank);
	MPI_Ssend(&i, 1, MPI_INT, (my_rank + 1) % commsize, tag, MPI_COMM_WORLD);

	if (my_rank == 0) {
		//printf("My rank = %d, start recv\n", my_rank);
		MPI_Recv(&i, 1, MPI_INT, commsize - 1, tag, MPI_COMM_WORLD, &status);

		printf("My rank = %2d, i = %2d\n", my_rank, i);
	}

	MPI_Finalize();
}