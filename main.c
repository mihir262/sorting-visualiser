#include <stdio.h>
#include <raylib.h>

#define WIDTH 900
#define HEIGHT 600

#define COUNT 10

int numbers[COUNT];

void swapInt(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swapInt(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            swapInt(&arr[i], &arr[minIndex]);
        }
    }
}

void drawRectangles(void){
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

        DrawRectangleRec((Rectangle){x, y, barWidth, barHeight}, RAYWHITE);
    }
}
    
int main(void){
    
    for(int i = 0; i < COUNT; i++){
        numbers[i] = i;
    }

    InitWindow(WIDTH, HEIGHT, "Sorting Visualiser");

    while(!WindowShouldClose()){
        BeginDrawing();
        
        drawRectangles();

        EndDrawing();
    }
    return 0;
}
