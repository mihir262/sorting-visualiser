#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>

#define WIDTH 900
#define HEIGHT 600

#define COUNT 30

int numbers[COUNT];
int activeA = -1;
int activeB = -1;
float swapDelaySeconds = 0.06f;

typedef enum SortAlgorithm {
    SORT_BUBBLE,
    SORT_INSERTION,
    SORT_SELECTION,
    SORT_QUICK,
    SORT_MERGE,
    SORT_COUNTING
} SortAlgorithm;

SortAlgorithm selectedAlgorithm = SORT_BUBBLE;

const char *algorithmName(SortAlgorithm algorithm) {
    switch (algorithm) {
        case SORT_BUBBLE:
            return "Bubble";
        case SORT_INSERTION:
            return "Insertion";
        case SORT_SELECTION:
            return "Selection";
        case SORT_QUICK:
            return "Quick";
        case SORT_MERGE:
            return "Merge";
        case SORT_COUNTING:
            return "Counting";
        default:
            return "Unknown";
    }
}

void swapInt(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void drawRectangles(int highlightA, int highlightB){
    const int leftPadding = 20;
    const int rightPadding = 20;
    const int topPadding = 20;
    const int bottomPadding = 20;

    const float availableWidth = (float)(WIDTH - leftPadding - rightPadding);
    const float availableHeight = (float)(HEIGHT - topPadding - bottomPadding);

    const float gap = (COUNT > 120) ? 0.0f : 2.0f;
    const float totalGaps = (COUNT > 0) ? (COUNT - 1) * gap : 0.0f;
    const float barWidth = (COUNT > 0) ? (availableWidth - totalGaps) / COUNT : 0.0f;

    int maxValue = 1;
    for (int i = 0; i < COUNT; i++) {
        if (numbers[i] > maxValue) {
            maxValue = numbers[i];
        }
    }

    for(int i = 0; i < COUNT; i++){
        int value = numbers[i];
        float barHeight = (value * availableHeight) / maxValue;

        float x = leftPadding + i * (barWidth + gap);
        float y = HEIGHT - bottomPadding - barHeight;

        Color barColor = RAYWHITE;
        if (i == highlightA) {
            barColor = RED;
        } else if (i == highlightB) {
            barColor = GREEN;
        }

        DrawRectangleRec((Rectangle){x, y, barWidth, barHeight}, barColor);
    }
}

void drawFrame(void) {
    BeginDrawing();
    ClearBackground(BLACK);
    drawRectangles(activeA, activeB);
    DrawText(TextFormat("Algorithm: %s", algorithmName(selectedAlgorithm)), 20, 15, 24, RAYWHITE);
    DrawText("1 Bubble  2 Insertion  3 Selection  4 Quick  5 Merge  6 Counting", 20, 45, 18, LIGHTGRAY);
    DrawText("SPACE Sort   R Reshuffle", 20, 68, 18, LIGHTGRAY);
    EndDrawing();
}

void animatedSwap(int arr[], int i, int j) {
    activeA = i;
    activeB = j;
    swapInt(&arr[i], &arr[j]);
    drawFrame();
    WaitTime(swapDelaySeconds);
}

void animatedSet(int arr[], int index, int value) {
    activeA = index;
    activeB = -1;
    arr[index] = value;
    drawFrame();
    WaitTime(swapDelaySeconds);
}

// Bubble sort: repeatedly swap adjacent out-of-order elements so larger values
// "bubble" to the end each pass. Time: O(n^2) avg/worst, O(n) best (with optimization). Space: O(1).
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                animatedSwap(arr, j, j + 1);
            }
        }
    }
}

// Insertion sort: build a sorted left side by inserting each new element into
// its correct position. Time: O(n^2) avg/worst, O(n) best. Space: O(1).
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int j = i;
        while (j > 0 && arr[j - 1] > arr[j]) {
            animatedSwap(arr, j - 1, j);
            j--;
        }
    }
}

// Selection sort: repeatedly pick the smallest remaining element and place it
// at the next sorted position. Time: O(n^2) best/avg/worst. Space: O(1).
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            animatedSwap(arr, i, minIndex);
        }
    }
}

// Quick sort: pick a pivot, partition into smaller/larger sides, then recurse.
// Time: O(n log n) avg/best, O(n^2) worst. Space: O(log n) average recursion stack.
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                if (i != j) {
                    animatedSwap(arr, i, j);
                }
            }
        }

        if (i + 1 != high) {
            animatedSwap(arr, i + 1, high);
        }
        int pivotIndex = i + 1;

        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

static void mergeRange(int arr[], int temp[], int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (int idx = left; idx <= right; idx++) {
        animatedSet(arr, idx, temp[idx]);
    }
}

static void mergeSortRange(int arr[], int temp[], int left, int right) {
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;
    mergeSortRange(arr, temp, left, mid);
    mergeSortRange(arr, temp, mid + 1, right);
    mergeRange(arr, temp, left, mid, right);
}

// Merge sort: divide, sort halves, then merge. Time: O(n log n). Space: O(n).
void mergeSort(int arr[], int n) {
    int *temp = malloc(sizeof(int) * n);
    if (temp == NULL) {
        fprintf(stderr, "Failed to allocate merge sort buffer.\n");
        return;
    }

    mergeSortRange(arr, temp, 0, n - 1);
    free(temp);
}

// Counting sort: count occurrences by value range, then rebuild. Time: O(n + k). Space: O(n + k).
void countingSort(int arr[], int n) {
    if (n <= 1) {
        return;
    }

    int minValue = arr[0];
    int maxValue = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < minValue) {
            minValue = arr[i];
        }
        if (arr[i] > maxValue) {
            maxValue = arr[i];
        }
    }

    int range = maxValue - minValue + 1;
    int *counts = calloc((size_t)range, sizeof(int));
    int *output = malloc(sizeof(int) * n);
    if (counts == NULL || output == NULL) {
        fprintf(stderr, "Failed to allocate counting sort buffers.\n");
        free(counts);
        free(output);
        return;
    }

    for (int i = 0; i < n; i++) {
        counts[arr[i] - minValue]++;
    }

    for (int i = 1; i < range; i++) {
        counts[i] += counts[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int value = arr[i];
        int position = --counts[value - minValue];
        output[position] = value;
    }

    for (int i = 0; i < n; i++) {
        animatedSet(arr, i, output[i]);
    }

    free(counts);
    free(output);
}

int main(void){
    
    for(int i = 0; i < COUNT; i++){
        numbers[i] = i + 1;
    }

    //fisher yates shuffle - randomising the array
    for (int i = COUNT - 1; i > 0; i--) {
        int j = GetRandomValue(0, i);
        swapInt(&numbers[i], &numbers[j]);
    }

    InitWindow(WIDTH, HEIGHT, "Sorting Visualiser");
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        if (IsKeyPressed(KEY_ONE)) {
            selectedAlgorithm = SORT_BUBBLE;
        }
        if (IsKeyPressed(KEY_TWO)) {
            selectedAlgorithm = SORT_INSERTION;
        }
        if (IsKeyPressed(KEY_THREE)) {
            selectedAlgorithm = SORT_SELECTION;
        }
        if (IsKeyPressed(KEY_FOUR)) {
            selectedAlgorithm = SORT_QUICK;
        }
        if (IsKeyPressed(KEY_FIVE)) {
            selectedAlgorithm = SORT_MERGE;
        }
        if (IsKeyPressed(KEY_SIX)) {
            selectedAlgorithm = SORT_COUNTING;
        }
        if (IsKeyPressed(KEY_R)) {
            for (int i = 0; i < COUNT; i++) {
                numbers[i] = i + 1;
            }
            for (int i = COUNT - 1; i > 0; i--) {
                int j = GetRandomValue(0, i);
                swapInt(&numbers[i], &numbers[j]);
            }
            activeA = -1;
            activeB = -1;
        }
        if (IsKeyPressed(KEY_SPACE)) {
            activeA = -1;
            activeB = -1;
            if (selectedAlgorithm == SORT_BUBBLE) {
                bubbleSort(numbers, COUNT);
            } else if (selectedAlgorithm == SORT_INSERTION) {
                insertionSort(numbers, COUNT);
            } else if (selectedAlgorithm == SORT_SELECTION) {
                selectionSort(numbers, COUNT);
            } else if (selectedAlgorithm == SORT_QUICK) {
                quickSort(numbers, 0, COUNT - 1);
            } else if (selectedAlgorithm == SORT_MERGE) {
                mergeSort(numbers, COUNT);
            } else if (selectedAlgorithm == SORT_COUNTING) {
                countingSort(numbers, COUNT);
            }
            activeA = -1;
            activeB = -1;
        }

        drawFrame();
    }
    CloseWindow();
    return 0;
}
