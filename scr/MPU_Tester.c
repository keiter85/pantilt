#include <stdio.h>
//#include <stdint.h>
//#include <stdbool.h>
//#include <unistd.h>
#include <signal.h>
//#include <stdlib.h>
#include <wiringPi.h>
//#include <wiringPiI2C.h>

//#include "MPU9250.h"


void intHandler(int dummy) {
    // turn off all channels
    //PCA9685_setAllPWM(fd, addr, _PCA9685_MINVAL, _PCA9685_MINVAL);
    //exit(dummy);
}

int main(void){
    wiringPiSetup();
    //wiringPiI2CSetup (68);
    //signal(SIGINT, intHandler);
    //MPU9250_Init();
    for(;;){
        //MPU9250_READ_ACCEL();
        int a = 1;
        int b = 2;
        int c = 3;
        printf("accx: %d, accy: %d, accz: %d", a, b, c);
        delay(500);
    }
}