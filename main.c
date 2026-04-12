#include <stdio.h>
#include <raylib.h>

#define WIDTH 900
#define HEIGHT 600

#define COUNT 10

int numbers[COUNT];

void drawRectangles(){
    for(int i = 0; i < COUNT; i++){
        int value = numbers[i];
        DrawRectangle(i * 50, HEIGHT/2, 20, value, RAYWHITE);
    }
}
    
    

int main(){
    
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

