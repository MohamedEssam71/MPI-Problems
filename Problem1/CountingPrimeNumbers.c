#include <mpi.h>
#include <stdio.h>

int isPrime(int x){
    if(x < 2)return 0;
    int i;
    for(i = 2;i * i <= x;++i){
        if(x % i == 0)return 0;
    }
    return 1;
}

int calc(int l,int r){
    int cnt = 0;
    for(;l <= r;++l){
        cnt += isPrime(l);
    }
    return cnt;
}


int main(int argc, char **argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int x, y;
        printf("Enter the ranges: \n");
        fflush(stdout);
        scanf("%d %d", &x, &y);
    
        int range = (y - x + 1) / size,rem = (y - x + 1) % size;
        int len = range + (0 < rem);
        int l = x, r = x + len - 1, i;
        for (i = 1; i < size; ++i) {
            len = range + (i < rem);
            l = r + 1;
            r = l + len - 1;
            MPI_Send(&l, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&r, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        len = range + (0 < rem);
        l = x,r = x + len - 1;
        int primeCnt = calc(l,r);

        printf("\nTotal number of prime numbers in P%d is: %d\n",rank,primeCnt);

        int tmp;
        for(i = 1;i < size;++i){
            MPI_Recv(&tmp,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            primeCnt += tmp;
        }
        printf("\nTotal number of prime numbers: %d\n",primeCnt);
        fflush(stdout);
    } else {
        int l,r;
        MPI_Recv(&l,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&r,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        int primeCnt = calc(l,r);
        printf("\nTotal number of prime numbers in P%d is: %d\n",rank,primeCnt);
        fflush(stdout);
        MPI_Send(&primeCnt, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}