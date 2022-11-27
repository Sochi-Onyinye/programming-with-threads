#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 4

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

//int MAX;

typedef struct{
  int row;
  int column;
}Cordinate;

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
//void* computeSum(void* args) { // pass in the number of the ith thread
//    return NULL;
//}
void* computeSum(void* args) { 
    Cordinate *entry = (Cordinate *)args;
    int row = entry->row;
    int column = entry->column;
    matSumResult[row][column] = matA[row][column]+matB[row][column];
}
// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
    Cordinate *entry = (Cordinate *)args;
    int row = entry->row;
    int column = entry->column;
    matDiffResult[row][column] = matA[row][column]-matB[row][column];
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
   Cordinate *entry = (Cordinate *)args;
   int row = entry->row;
   int column = entry->column;
   matProductResult[row][column] = matA[row][column]*matB[row][column];
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main(int argc, char *argv[]) {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    int threadCount = MAX*MAX;
    pthread_t threads[threadCount];
    
    // 4. Create a thread for each cell of each matrix operation.
    // 
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute.
    // 
    // One way to do this is to malloc memory for the thread number i, populate the coordinates
    // into that space, and pass that address to the thread. The thread will use that number to calcuate 
    // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
  int i;
    for (i = 0;i<threadCount;i++) {
    Cordinate *entry = malloc(sizeof(Cordinate));
    entry -> row = i / MAX;
    entry -> column = i % MAX;
    pthread_create(&threads[i],NULL, &computeSum,(void *)(entry));
    pthread_create(&threads[i],NULL, &computeDiff,(void *)(entry));
    pthread_create(&threads[i],NULL, &computeProduct, (void *)(entry));
    }
    // 5. Wait for all threads to finish.
    for (i = 0;i<threadCount;i++) {
      pthread_join(threads[i], NULL);
   }
    
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
  
}