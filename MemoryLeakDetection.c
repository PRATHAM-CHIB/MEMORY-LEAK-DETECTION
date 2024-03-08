#include <stdio.h>
#include <stdlib.h>

// Structure to store allocation information
typedef struct {
    void* ptr;
    size_t size;
    const char* file;
    int line;
} AllocationInfo;

// Dynamic array to store allocation records
AllocationInfo* allocationRecords = NULL;
size_t allocationCount = 0;

// Function to simulate memory allocation
void* customMalloc(size_t size, const char* file, int line) {
    void* ptr = malloc(size);

    // Add allocation information to records
    AllocationInfo info = {ptr, size, file, line};
    allocationRecords = realloc(allocationRecords, (allocationCount + 1) * sizeof(AllocationInfo));
    allocationRecords[allocationCount++] = info;

    return ptr;
}

// Function to simulate memory deallocation
void customFree(void* ptr, const char* file, int line) {
    // Find the allocation record for the given pointer
    for (size_t i = 0; i < allocationCount; ++i) {
        if (allocationRecords[i].ptr == ptr) {
            // Display deallocation information
            printf("Deallocated memory at %p in %s:%d\n", ptr, file, line);

            // Remove the allocation record
            for (size_t j = i; j < allocationCount - 1; ++j) {
                allocationRecords[j] = allocationRecords[j + 1];
            }
            allocationRecords = realloc(allocationRecords, (allocationCount - 1) * sizeof(AllocationInfo));
            --allocationCount;

            // Free the memory
            free(ptr);
            return;
        }
    }

    // Display an error message if the allocation record is not found
    printf("Error: Attempted to deallocate memory at %p, but no corresponding allocation record found.\n", ptr);
}

// Macros to replace standard malloc and free
#define malloc(size) customMalloc(size, __FILE__, __LINE__)
#define free(ptr) customFree(ptr, __FILE__, __LINE__)

// Function to display allocation and deallocation information in a graph format
void displayMemoryGraph() {
    printf("Memory Allocation and Deallocation Graph:\n");
    printf("=========================================\n");

    // Display the graph header
    printf("%-20s | %-10s | %-10s | %-10s |\n", "Memory Address", "Size", "File", "Line");
    printf("---------------------|------------|------------|------------|\n");

    // Display allocation and deallocation information in the graph
    for (size_t i = 0; i < allocationCount; ++i) {
        printf("%-20p | %-10zu | %-10s | %-10d |\n",
               allocationRecords[i].ptr,
               allocationRecords[i].size,
               allocationRecords[i].file,
               allocationRecords[i].line);
    }
}

int main() {
    // Sample memory allocation and deallocation
    int* dynamicArray1 = (int*)malloc(5 * sizeof(int));
    int* dynamicArray2 = (int*)malloc(3 * sizeof(int));

    // Display memory allocation and deallocation information in a graph format
    displayMemoryGraph();

    // Deallocate memory for dynamicArray1
    free(dynamicArray1);

    // Display updated memory allocation and deallocation information in a graph format
    displayMemoryGraph();

    // Deallocate memory for dynamicArray2
    free(dynamicArray2);

    return 0;
}
