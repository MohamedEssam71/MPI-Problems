#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>

char shiftSingle(char c, int shiftValue) {
    if (c >= 'a' && c <= 'z') {
        int order = (c - 'a' + shiftValue + 26) % 26;
        return (char) ('a' + order);
    } else {
        int order = (c - 'A' + shiftValue + 26) % 26;
        return (char) ('A' + order);
    }
}

void shift(char *str, int len, int shiftValue) {
    int i;
    for (i = 0; i < len; ++i) {
        str[i] = shiftSingle(str[i], shiftValue);
    }
}

const int N = 100;

void handleInput(char *str, int *type) {
    printf("\n------------ Ceasar Cipher -------------\n1-Encryption\n"
           "2-Decryption\n");
    printf("Choose a number: ");
    fflush(stdout);

    scanf("%d", type);

    printf("\nSelect the string input mode:\n"
           "1-Console\n"
           "2-File\n");
    printf("Choose a number: ");
           fflush(stdout);
    int mode;
    scanf("%d", &mode);

    if (mode == 1) {
        printf("\nEnter the string: \n");
        fflush(stdout);
        scanf("%s", str);
    } else {
        char path[N];
        printf("\nEnter the file path:\n");
        fflush(stdout);
        scanf("%s",path);

        FILE *file = fopen(path,"r");
        if(file == NULL){
            printf("This file does not exits!");
            exit(-1);
        }
        int cur = 0;
        char c;
        while ((c = fgetc(file)) != EOF){
            str[cur++] = c;
        }
        str[cur] = '\0';
    }
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

        char str[N];
        int type, shiftValue;
        handleInput(str, &type);

        switch (type) {
            case 1:
                shiftValue = 3;
                break;
            case 2:
                shiftValue = -3;
                break;
            default:
                printf("Invalid input\n");
                exit(-1);
        }

        int strLength = strlen(str);

        //
        int r = strLength / size, i, start = 0, rem = strLength % size, len;
        for (i = 1; i < size; ++i) {
            start += r + (i - 1 < rem);
            len = r + (i < rem);
            MPI_Send((str + start), len, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            MPI_Send(&shiftValue, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        shift(str, r + (0 < rem), shiftValue);

        start = 0;
        char tmp[N];
        for (i = 1; i < size; ++i) {
            start += r + (i - 1 < rem);
            len = r + (i < rem);
            MPI_Recv(&tmp, N, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int j;
            for (j = start; j < start + len; ++j) {
                str[j] = tmp[j - start];
            }
        }
        printf("The string after ");
        printf((type == 1 ? "encryption: ":"decryption: "));
        printf("%s\n", str);
        fflush(stdout);
    } else {
        int len, shiftValue;
        char str[N];
        MPI_Recv(str, N, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&shiftValue, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        len = strlen(str);
        shift(str, len, shiftValue);
        MPI_Send(str, N, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}