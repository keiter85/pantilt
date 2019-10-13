#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "MPU9250.h"


void intHandler(int dummy) {
    // turn off all channels
    //PCA9685_setAllPWM(fd, addr, _PCA9685_MINVAL, _PCA9685_MINVAL);
    exit(dummy);
}

int main(void){
    wiringPiSetup();
    signal(SIGINT, intHandler);
    MPU9250_Init(void)
    for(;;){
        MPU9250_READ_ACCEL();
        printf("accx: %i, accy: %i, accz: %i", accel[0], accel[1], accel[2]);
        sleep(500);
    }
}