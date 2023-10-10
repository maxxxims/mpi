#include <mpi.h>
#include <stdio.h>
#include <time.h>


void create_file(char *filename)
{
	FILE *file;
		
	if ((file = fopen(filename, "w")) == NULL)
	{
		printf("Не удалось открыть файл");
		getchar();
	}
	else
	{
		fprintf(file, "latency latency_norm\n");
	}
	
	fclose(file);
}


void send_nothing(int rank, int send_n_times, char *filename)
{
	
	clock_t start, end;
	short msg = 0;
	int i = 0;
	
	if (rank == 0) 
	{
        	start = clock();
		
		for(i; i < send_n_times; i++)
		{
			MPI_Ssend(&msg, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
			MPI_Recv(&msg,  1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		
		clock_t end = clock();
		double latency = (double)(end - start) / CLOCKS_PER_SEC;
		printf("The latency is %f seconds \n", latency / (2. * send_n_times));
		
		// writing to file
		
		FILE *file;
		
		if ((file = fopen(filename, "a")) == NULL)
		{
			printf("Не удалось открыть файл");
			getchar();
		}
		else
		{
			fprintf(file, "%f %f\n", latency, latency / (2. * send_n_times));
		}
		
		fclose(file);
		
    	} else if (rank == 1) {

		for(i; i < send_n_times; i++)
		{
		    MPI_Recv(&msg,  1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    MPI_Ssend(&msg, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
		}
    	}
    	
    	
}


int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    
    int SEND_N_TIMES = 1e6;
    char FILE_NAME[] = "data.txt";
    
    if (rank == 0)
    {
    	create_file(FILE_NAME);
    }
    
    send_nothing(rank, SEND_N_TIMES, FILE_NAME);


    MPI_Finalize();
    return 0;
}
