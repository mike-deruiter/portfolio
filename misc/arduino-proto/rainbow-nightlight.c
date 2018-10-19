/* rainbow-nightlight: Prototype for a light that gently phases between 
   different colors of light - Red -> Orange -> Yellow -> Green -> Blue, etc. */

#include <stdio.h>
#include <stdlib.h>

#define STEP        20
#define MAX_OUTPUT  100

void phaseLight();
void printVals();

int RedVal, GreenVal, BlueVal;

void init() {
    RedVal = MAX_OUTPUT - 1;
    GreenVal = 0;
    BlueVal = 0;
}

int main() {
    init();
    
    while (RedVal != MAX_OUTPUT) {
        phaseLight();
        printVals();
    }
    
    exit(0);
}

void phaseLight() {
    if (RedVal > 0 && BlueVal <= 0) {
      RedVal = ((RedVal-STEP) > 0) ? RedVal - STEP : 0;
      GreenVal = ((GreenVal+STEP) < MAX_OUTPUT) ? GreenVal + STEP : MAX_OUTPUT;
    } else if (GreenVal > 0) {
      GreenVal = ((GreenVal-STEP) > 0) ? GreenVal - STEP : 0;
      BlueVal = ((BlueVal+STEP) < MAX_OUTPUT) ? BlueVal + STEP : MAX_OUTPUT;
    } else {
      BlueVal = ((BlueVal-STEP) > 0) ? BlueVal - STEP : 0;
      RedVal = ((RedVal+STEP) < MAX_OUTPUT) ? RedVal + STEP : MAX_OUTPUT;
    }
}

void printVals() {
    printf("Red: %3d / Green: %3d / Blue: %3d\n", RedVal, GreenVal, BlueVal);
}
