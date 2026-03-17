/*

Make an experimental comparison of some* sorting algorithms.

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
#define INSERTION_SORT 2
#define SELECTION_SORT 3
#define MERGE_SORT 4
#define RADIX_SORT 5

/* Display names for each algorithm, indexed by the ALGORITHM constants above. */
static const char* ALGORITHM_NAMES[] = {
    "bubble sort",
    "quick sort",
    "insertion sort",
    "selection sort",
    "merge sort",
    "radix sort"
};

/* Output file handle for results.csv. Opened once in main() and shared by all write functions. */
static FILE* resultsFile = NULL;

/* Seeds the random number generator with the current time so each run produces different data. */
void resetRandomSeed() { srand((unsigned int)time(NULL)); }

/* Records the current CPU clock tick. Call before the operation you want to time. */
clock_t startOperationTimer() { return clock(); }

/* Returns the elapsed time in milliseconds since startTime. */
double stopOperationTimerMs(clock_t startTime) {
    clock_t endTime = clock();
    return ((double)(endTime - startTime) * 1000.0) / CLOCKS_PER_SEC;
}

/* Allocates and returns an array of `size` random integers in the range [0, size].
   The caller is responsible for freeing the returned pointer. */
int* generateIntArray(unsigned long size) {
    int* array;

    array = malloc(sizeof(int) * size);

    for (unsigned long i = 0; i < size; i++) {
        array[i] = (int)(rand() % (size + 1));
    }

    return array;
}

/* Allocates and returns a partially sorted array of `size` integers.
   Starts as a sorted sequence 0..size-1, then performs size/factor random swaps.
   A smaller factor means more swaps (less sorted); a larger factor means fewer swaps (more sorted).
   Example: factor=10 gives ~10% of elements swapped; factor=90 gives ~1% swapped.
   The caller is responsible for freeing the returned pointer. */
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

/* Allocates and returns an array of `size` integers in strictly descending order (size-1 down to 0).
   The caller is responsible for freeing the returned pointer. */
int* generateReverseSortedIntArray(unsigned long size) {
    int* array = malloc(sizeof(int) * size);

    for (unsigned long i = 0; i < size; i++) {
        array[i] = (int)(size - 1 - i);
    }

    return array;
}

/* Sorts array in-place using bubble sort (O(n^2) average/worst, O(n) best).
   Includes an early-exit optimisation: stops if a full pass produces no swaps. */
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

/* Swaps the two integers pointed to by a and b. */
void swapInts(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Chooses the median of array[low], array[mid], array[high] as pivot,
   swaps it into the array[high] position, then partitions array[low..high].
   Elements <= pivot are moved to the left; elements > pivot to the right.
   Returns the final index of the pivot. */
long partitionQuickSort(int* array, long low, long high) {
    long mid = low + (high - low) / 2;

    /* Sort low, mid, high so that array[mid] is the median */
    if (array[low] > array[mid])   swapInts(&array[low],  &array[mid]);
    if (array[low] > array[high])  swapInts(&array[low],  &array[high]);
    if (array[mid] > array[high])  swapInts(&array[mid],  &array[high]);
    /* array[mid] is now the median; move it to high as pivot */
    swapInts(&array[mid], &array[high]);

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

/* Recursive helper for quickSort. Sorts array[low..high] in-place. */
void quickSortRecursive(int* array, long low, long high) {
    if (low < high) {
        long pivotIndex = partitionQuickSort(array, low, high);
        quickSortRecursive(array, low, pivotIndex - 1);
        quickSortRecursive(array, pivotIndex + 1, high);
    }
}

/* Sorts array in-place using quick sort (O(n log n) average, O(n^2) worst with last-element pivot). */
void quickSort(int* array, unsigned long size) {
    if (array == NULL || size < 2) {
        return;
    }

    quickSortRecursive(array, 0, (long)size - 1);
}

/* Sorts array in-place using insertion sort (O(n^2) average/worst, O(n) best on nearly sorted input). */
void insertionSort(int* array, unsigned long size) {
    if (array == NULL || size < 2) {
        return;
    }

    for (unsigned long i = 1; i < size; i++) {
        int key = array[i];
        long j = (long)i - 1;

        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j--;
        }

        array[j + 1] = key;
    }
}

/* Sorts array in-place using selection sort (O(n^2) for all cases; not sensitive to input order). */
void selectionSort(int* array, unsigned long size) {
    if (array == NULL || size < 2) {
        return;
    }

    for (unsigned long i = 0; i < size - 1; i++) {
        unsigned long minIndex = i;
        for (unsigned long j = i + 1; j < size; j++) {
            if (array[j] < array[minIndex]) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            swapInts(&array[i], &array[minIndex]);
        }
    }
}

/* Merges two adjacent sorted sub-arrays: array[left..mid] and array[mid+1..right].
   Uses two temporary arrays L and R; result is written back into array[left..right]. */
void merge(int* array, unsigned long left, unsigned long mid, unsigned long right) {
    unsigned long n1 = mid - left + 1;
    unsigned long n2 = right - mid;

    int* L = malloc(sizeof(int) * n1);
    int* R = malloc(sizeof(int) * n2);
    if (L == NULL || R == NULL) {
        free(L);
        free(R);
        return;
    }

    for (unsigned long i = 0; i < n1; i++) {
        L[i] = array[left + i];
    }
    for (unsigned long j = 0; j < n2; j++) {
        R[j] = array[mid + 1 + j];
    }

    unsigned long i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k++] = L[i++];
        } else {
            array[k++] = R[j++];
        }
    }

    while (i < n1) {
        array[k++] = L[i++];
    }

    while (j < n2) {
        array[k++] = R[j++];
    }

    free(L);
    free(R);
}

/* Recursive helper for mergeSort. Sorts array[left..right] in-place. */
void mergeSortRecursive(int* array, long left, long right) {
    if (left < right) {
        long mid = left + (right - left) / 2;
        mergeSortRecursive(array, left, mid);
        mergeSortRecursive(array, mid + 1, right);
        merge(array, (unsigned long)left, (unsigned long)mid, (unsigned long)right);
    }
}

/* Sorts array in-place using merge sort (O(n log n) for all cases; uses O(n) extra memory). */
void mergeSort(int* array, unsigned long size) {
    if (array == NULL || size < 2) {
        return;
    }

    mergeSortRecursive(array, 0, (long)size - 1);
}

/* Returns the maximum value in array. Used by radixSort to determine the number of digit passes needed. */
int getMaxValue(int* array, unsigned long size) {
    int mx = array[0];
    for (unsigned long i = 1; i < size; i++) {
        if (array[i] > mx) mx = array[i];
    }
    return mx;
}

/* Performs a stable counting sort on array by the digit at position exp (1, 10, 100, ...).
   Called once per digit by radixSort. Uses a temporary output array of size `size`. */
void countSortByDigit(int* array, unsigned long size, int exp) {
    if (size == 0) return;

    int* output = malloc(sizeof(int) * size);
    if (output == NULL) return;

    int count[10] = {0};

    for (unsigned long i = 0; i < size; i++) {
        int digit = (array[i] / exp) % 10;
        count[digit]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (long i = (long)size - 1; i >= 0; i--) {
        int digit = (array[i] / exp) % 10;
        output[count[digit] - 1] = array[i];
        count[digit]--;
    }

    for (unsigned long i = 0; i < size; i++) {
        array[i] = output[i];
    }

    free(output);
}

/* Sorts array in-place using base-10 radix sort (O(d*(n+10)) where d = number of digits in the max value).
   Processes digits from least significant to most significant. Only works for non-negative integers. */
void radixSort(int* array, unsigned long size) {
    if (array == NULL || size < 2) return;

    int m = getMaxValue(array, size);
    for (int exp = 1; m / exp > 0; exp *= 10) {
        countSortByDigit(array, size, exp);
    }
}

/* Runs the sorting algorithm identified by `type` on `data` and returns the elapsed time in milliseconds.
   `type` must be one of the BUBBLE_SORT, QUICK_SORT, ... constants defined at the top of this file. */
double measureSortAlg(int type, int* data, unsigned long size) {
    clock_t startTime = startOperationTimer();
    switch (type) {
        case BUBBLE_SORT:
            bubbleSort(data, size);
            break; 
        case QUICK_SORT:
            quickSort(data, size);
            break;
        case INSERTION_SORT:
            insertionSort(data, size);
            break;
        case SELECTION_SORT:
            selectionSort(data, size);
            break;
        case MERGE_SORT:
            mergeSort(data, size);
            break;
        case RADIX_SORT:
            radixSort(data, size);
            break;
        default:
            printf("not implemented");
    }
    
    double elapsedMs = stopOperationTimerMs(startTime);

    return elapsedMs;
}

/* Prints the CSV column header to stdout. */
void printHeader() {
    printf("Elements, Runs, Algorithm, Variant, Elapsed time(ms)\n");
}

/* Writes one result row to stdout and to resultsFile (if open).
   Format: size, count, algorithmName, variant, elapsedMs (3 decimal places). */
void writeResultRow(unsigned long size, int count, const char* algorithmName, const char* variant, double elapsedMs) {
    char line[256];

    sprintf(line, "%lu, %d, %s, %s, %.3f", size, count, algorithmName, variant, elapsedMs);    

    printf("%s\n", line);
    if (resultsFile) {
        fprintf(resultsFile, "%s\n", line);
    }
}


/* Benchmarks algorithm `type` on arrays of length `size` across all input variants
   (reversed, random, and partially sorted at 10/30/50/70/90%).
   Each variant is run `count` times and the average elapsed time is written via writeResultRow. */
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
    writeResultRow(size, count, algorithmName, "reversed", elapsedMs);
  
    // random data
    sumMs = 0;
    for (int i = 0; i < count; i++) {
        x = generateIntArray(size);
        sumMs += measureSortAlg(type, x, size);
        free(x);
    }

    elapsedMs = sumMs/count;
    writeResultRow(size, count, algorithmName, "random", elapsedMs);

    // partially sorted
    for (int factor = 10; factor <= 100; factor += 20) {
        sumMs = 0;
        for (int i = 0; i < count; i++) {
            x = generatePartiallySortedIntArray(size, factor);
            sumMs += measureSortAlg(type, x, size);
            free(x);
        }

        elapsedMs = sumMs/count;
        char variantType[50];
        sprintf(variantType, "partially sorted %d%%", factor );
        writeResultRow(size, count, algorithmName, variantType, elapsedMs);
    }
    printf("\n");
}

/* Runs analyzeSortAlg for all six algorithms at the given array size and run count. */
void analyzeAlgo(unsigned long size, int count) {
    //analyzeSortAlg(BUBBLE_SORT, size, count);
    //analyzeSortAlg(QUICK_SORT, size, count);
    //analyzeSortAlg(INSERTION_SORT, size, count);
    analyzeSortAlg(SELECTION_SORT, size, count);
    //analyzeSortAlg(MERGE_SORT, size, count);
    //analyzeSortAlg(RADIX_SORT, size, count);
}

int main() {
    resetRandomSeed();
    /* open results file once (append) and keep it open until program end */
    resultsFile = fopen("results.csv", "a");

    printHeader();

    //analyzeAlgo(10, 1000000);
    //analyzeAlgo(50, 1000000);
    //analyzeAlgo(100, 1000000);
    //analyzeAlgo(1000, 100);
    //analyzeAlgo(10000, 10);
    //analyzeAlgo(100000, 1);
    analyzeAlgo(1000000, 1);

    fclose(resultsFile);

    return 0;
}
