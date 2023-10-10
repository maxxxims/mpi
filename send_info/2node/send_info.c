#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


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
		fprintf(file, "msg_size time time_norm\n");
	}
	
	fclose(file);
}


void send_info(int rank, int send_n_times, int msg_size, char *filename)
{
	clock_t start, end;
	int i = 0;
	
	long *msg;
	//long msg = 10;
	msg = (long *)calloc( msg_size, sizeof( long ) );
	if( msg == NULL )	printf( "Can't allocate memory\n" );
	
	if (rank == 0) 
	{
        	start = clock();
		
		for(i; i < send_n_times; i++)
		{
			MPI_Ssend(msg,  msg_size, MPI_LONG, rank + 1, 0, MPI_COMM_WORLD);
			MPI_Recv(msg,   msg_size, MPI_LONG, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		
		end = clock();
		
		double latency = (double)(end - start) / CLOCKS_PER_SEC;
		printf("The elapsed time is %f seconds \n", latency / (2. * send_n_times));
		
		// writing to file
		
		FILE *file;
		
		if ((file = fopen(filename, "a")) == NULL)
		{
			printf("Не удалось открыть файл");
			getchar();
		}
		else
		{
			fprintf(file, "%i %f %f\n", msg_size, latency, latency / (2. * send_n_times));
		}
		
		fclose(file);
		
    	} else if (rank == 1) {

		for(i; i < send_n_times; i++)
		{
		    MPI_Recv(msg,  msg_size, MPI_LONG, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    MPI_Ssend(msg, msg_size, MPI_LONG, rank - 1, 0, MPI_COMM_WORLD);
		}
    	}
    	
    	free( msg );
}

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    
    //int SEND_N_TIMES = 1e2, STEP = 1e5, MAX_MSG_SIZE = 1e7;
    int SEND_N_TIMES = 1e4, STEP = 1, MAX_MSG_SIZE = 1e2; //2640000 + 1000;
    char FILE_NAME[] = "data_small.txt";
    
    if (rank == 0){
    	create_file(FILE_NAME);
    }
    
    int counter = 1;
    
    while (counter * STEP < MAX_MSG_SIZE)
    {
    	if (rank == size - 1)	printf("step = %i\n", counter * STEP);
    	send_info(rank, SEND_N_TIMES, counter * STEP, FILE_NAME);
    	counter += 1;
    }
    


    MPI_Finalize();
    return 0;
}
