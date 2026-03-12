/*

Make an experimental comparison of some* sorting algorithms.

*
the sorting algorithms included in the comparison must include, at the
minimum, all those which were studied in the Algorithms and Data
Structures 1 lecture, but the recommendation is to also include other
existing sorting algorithms. (bubble, insertion, selection, merge, quick, radix sort)

For this:

Implement
the algorithms (you can use any programming language, but some implicit
recommendations are Python, C, C++ - being the languages you studied or
are studying).

Organize test data for the experiment. Consider different (many) cases
according:

    -> to length: from small (20, 30, 50, 100) to medium (thousands, tens of
thousands) to large (millions, tens, hundreds of millions). For small
lists, sort many (as sorting one will not yield relevant results, e.g.
100000 lists of 20, 30, 50)

    -> according to structure: random
lists, sorted lists, reverse sorted, almost sorted (e.g. 98% of elements
are sorted), mixed (half sorted), flat lists (long lists, 100000+, but
with very few distinct values)

    -> according to element type: integers, floats, strings, etc.

    -> according to data types: lists as arrays, lists as linked lists, etc.

    -> others: e.g. sorting in parallel, sorting streams.

Run the tests.

Analyze the results. Are the observed behaviors in accordance with expectations? If no, why?


TO DO:

    -> implement sorting algorithms
    -> data generator:
        - data type: arrays:
        - element type: integers, floats, strings
        - structure: random, sorted, reverse sorted, almost sorted, mixed sorted, flat lists
        - length: small -> medium -> large arrays

    -> measure sorting time

*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUBBLE_SORT 0
#define QUICK_SORT 1

static const char* ALGORITHM_NAMES[] = {
    "bubble sort",
    "quick sort",
   // "insertion sort",
   // "selection sort",
    // "merge sort",
    // "radix sort"
};

void resetRandomSeed() { srand((unsigned int)time(NULL)); }

clock_t startOperationTimer() { return clock(); }

double stopOperationTimerMs(clock_t startTime) {
    clock_t endTime = clock();
    return ((double)(endTime - startTime) * 1000.0) / CLOCKS_PER_SEC;
}

int* generateIntArray(unsigned long size) {
    int* array;

    array = malloc(sizeof(int) * size);

    for (unsigned long i = 0; i < size; i++) {
        array[i] = (int)(rand() % (size + 1));
    }

    return array;
}

int* generatePartiallySortedIntArray(unsigned long size, int factor) {
    int* array = malloc(sizeof(int) * size);

    for (unsigned long i = 0; i < size; i++) {
        array[i] = (int)i;
    }

    unsigned long swapCount = size / factor;
    if (swapCount == 0 && size > 1) {
        swapCount = 1;
    }

    for (unsigned long i = 0; i < swapCount; i++) {
        unsigned long indexA = (unsigned long)(rand() % size);
        unsigned long indexB = (unsigned long)(rand() % size);

        int temp = array[indexA];
        array[indexA] = array[indexB];
        array[indexB] = temp;
    }

    return array;
}

int* generateReverseSortedIntArray(unsigned long size) {
    int* array = malloc(sizeof(int) * size);

    for (unsigned long i = 0; i < size; i++) {
        array[i] = (int)(size - 1 - i);
    }

    return array;
}

void bubbleSort(int* array, unsigned long size) {
    if (array == NULL || size < 2) {
        return;
    }

    for (unsigned long i = 0; i < size - 1; i++) {
        int swapped = 0;

        for (unsigned long j = 0; j < size - 1 - i; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
                swapped = 1;
            }
        }

        if (!swapped) {
            break;
        }
    }
}

void swapInts(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

long partitionQuickSort(int* array, long low, long high) {
    int pivot = array[high];
    long i = low - 1;

    for (long j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;
            swapInts(&array[i], &array[j]);
        }
    }

    swapInts(&array[i + 1], &array[high]);
    return i + 1;
}

void quickSortRecursive(int* array, long low, long high) {
    if (low < high) {
        long pivotIndex = partitionQuickSort(array, low, high);
        quickSortRecursive(array, low, pivotIndex - 1);
        quickSortRecursive(array, pivotIndex + 1, high);
    }
}

void quickSort(int* array, unsigned long size) {
    if (array == NULL || size < 2) {
        return;
    }

    quickSortRecursive(array, 0, (long)size - 1);
}

double measureSortAlg(int type, int* data, unsigned long size) {
    clock_t startTime = startOperationTimer();
    switch (type) {
        case BUBBLE_SORT:
            bubbleSort(data, size);
            break;
        
        case QUICK_SORT:
            quickSort(data, size);
            break;
        
        default:
            printf("not implemented");
    }
    
    double elapsedMs = stopOperationTimerMs(startTime);

    return elapsedMs;
}

void analyzeSortAlg(int type, unsigned long size, int count) {
    double elapsedMs, sumMs;
    const char* algorithmName = ALGORITHM_NAMES[type];
    int* x;

    // reverse sorted
    sumMs = 0;
    for (int i = 0; i < count; i++) {
        x = generateReverseSortedIntArray(size);
        sumMs += measureSortAlg(type, x, size);
        free(x);
    }
    elapsedMs = sumMs/count;
    printf("%lu elements, %d runs, %s (reversed), elapsed time: %.3f ms\n", size, count, algorithmName, elapsedMs);
  
    // random data
    sumMs = 0;
    for (int i = 0; i < count; i++) {
        x = generateIntArray(size);
        sumMs += measureSortAlg(type, x, size);
        free(x);
    }
    elapsedMs = sumMs/count;
    printf("%lu elements, %d runs, %s (random), elapsed time: %.3f ms\n", size, count, algorithmName, elapsedMs);
    

    // partially sorted
    for (int factor = 10; factor <= 100; factor += 20) {
        sumMs = 0;
        for (int i = 0; i < count; i++) {
            x = generatePartiallySortedIntArray(size, factor);
            sumMs += measureSortAlg(type, x, size);
            free(x);
        }
        elapsedMs = sumMs/count;
        printf("%lu elements, %d runs, %s (partially sorted %d%%), elapsed time: %.3f ms\n", size, count, algorithmName, factor, elapsedMs);
        
    }
    printf("\n");
}

void analyzeAlgo(unsigned long size, int count) {
    analyzeSortAlg(BUBBLE_SORT, size, count);
    analyzeSortAlg(QUICK_SORT, size, count);
}

int main() {
    resetRandomSeed();

    analyzeAlgo(10, 1000000);
    analyzeAlgo(50, 1000000);
    analyzeAlgo(100, 1000000);
    analyzeAlgo(1000, 100);
    analyzeAlgo(10000, 10);
    //analyzeBubbleSort(100000);
    // analyzeBubbleSort(1000000);

    return 0;
}
