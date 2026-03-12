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

double measureBubbleSort(int* data, unsigned long size) {
    clock_t startTime = startOperationTimer();
    bubbleSort(data, size);
    double elapsedMs = stopOperationTimerMs(startTime);

    return elapsedMs;
}

void analyzeBubbleSort(unsigned long size, int count) {
    double elapsedMs, sumMs;
    int* x;

    // reverse sorted
    sumMs = 0;
    for (int i = 0; i < count; i++) {
        x = generateIntArray(size);
        sumMs += measureBubbleSort(x, size);
        free(x);
    }
    elapsedMs = sumMs/count;
    printf("%lu elements, %d runs, bubble sort (reversed), elapsed time: %.3f ms\n", size, count, elapsedMs);
  
    // random data
    sumMs = 0;
    for (int i = 0; i < count; i++) {
        x = generateIntArray(size);
        sumMs += measureBubbleSort(x, size);
        free(x);
    }
    elapsedMs = sumMs/count;
    printf("%lu elements, %d runs, bubble sort (random), elapsed time: %.3f ms\n", size, count, elapsedMs);
    

    // partially sorted
    for (int factor = 10; factor <= 100; factor += 20) {
        sumMs = 0;
        for (int i = 0; i < count; i++) {
            x = generatePartiallySortedIntArray(size, factor);
            sumMs += measureBubbleSort(x, size);
            free(x);
        }
        elapsedMs = sumMs/count;
        printf("%lu elements, %d runs, bubble sort (partially sorted %d%%), elapsed time: %.3f ms\n", size, count, factor, elapsedMs);
        
    }
    printf("\n");
}

int main() {
    resetRandomSeed();

    analyzeBubbleSort(10, 1000000);
    analyzeBubbleSort(50, 1000000);
    analyzeBubbleSort(100, 1000000);
    analyzeBubbleSort(1000, 100);
    analyzeBubbleSort(10000, 10);
    //analyzeBubbleSort(100000);
    // analyzeBubbleSort(1000000);

    return 0;
}
