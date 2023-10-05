#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 4
#define CACHE_SIZE 3

typedef struct Block {
    int data[BLOCK_SIZE];
    int size;
} Block;

typedef struct ExternalMemory {
    Block cache[CACHE_SIZE];
} ExternalMemory;

void initializeExternalMemory(ExternalMemory* em) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            em->cache[i].data[j] = 0;
        }
        em->cache[i].size = 0;
    }
}

void readArray(ExternalMemory* em, int* array, int size) {
    int currentBlock = 0;
    int currentIndex = 0;

    for (int i = 0; i < size; i++) {
        if (currentIndex == BLOCK_SIZE) {
            // If the current block is full, move to the next block
            currentBlock++;
            currentIndex = 0;
        }

        // Store the array element in the cache
        em->cache[currentBlock].data[currentIndex] = array[i];
        currentIndex++;

        // Update the block size
        em->cache[currentBlock].size = currentIndex;
    }
}

void merge(Block* output, Block* input1, Block* input2) {
    int i = 0, j = 0, k = 0;
    while (i < input1->size && j < input2->size && k < BLOCK_SIZE) {
        if (input1->data[i] < input2->data[j]) {
            output->data[k++] = input1->data[i++];
        } else {
            output->data[k++] = input2->data[j++];
        }
    }
    while (i < input1->size && k < BLOCK_SIZE) {
        output->data[k++] = input1->data[i++];
    }
    while (j < input2->size && k < BLOCK_SIZE) {
        output->data[k++] = input2->data[j++];
    }
    output->size = k;
}

void externalMergeSort(ExternalMemory* em, int numBlocks) {
    while (numBlocks > 1) {
        int i = 0;
        int j = 1;
        while (j < numBlocks) {
            // Merge two adjacent blocks and store the result in the first block
            merge(&em->cache[i], &em->cache[i], &em->cache[j]);

            // Update the size of the merged block
            em->cache[i].size = BLOCK_SIZE;

            // Clear the second block
            em->cache[j].size = 0;

            // Move to the next pair of blocks
            i++;
            j++;
        }
        numBlocks = i + 1;
    }
}

int main() {
    int array[] = {14, 7, 3, 12, 9, 11, 6, 2};
    int arraySize = sizeof(array) / sizeof(array[0]);

    ExternalMemory em;
    initializeExternalMemory(&em);

    // Read the array into the external memory
    readArray(&em, array, arraySize);

    // Perform external merge sort
    externalMergeSort(&em, CACHE_SIZE);

    // Print the sorted block (in memory)
    printf("Sorted Block 0: ");
    for (int i = 0; i < em.cache[0].size; i++) {
        printf("%d ", em.cache[0].data[i]);
    }
    printf("\n");

    return 0;
}
