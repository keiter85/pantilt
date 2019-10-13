#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>


//#include "Public_StdMacros.h"
//#include "Public_StdTypes.h"
//#include "MPU9250.h"
//#include "BMP180.h"
#include "IMU.h"
	


void intHandler(int dummy) {
    // turn off all channels
    //PCA9685_setAllPWM(fd, addr, _PCA9685_MINVAL, _PCA9685_MINVAL);
    exit(dummy);
}

void get_10DOF(void){
    MPU9250_READ_MAG();
    MPU9250_READ_GYRO();
    MPU9250_READ_ACCEL();
}

void main(void){

    wiringPiSetup();
    signal(SIGINT, intHandler);
    IMU_Init();

    while(1){
        get_10DOF();
        printf("magx: %i --magy %i --magz %i", magn[0], magn[1], magn[2]);
        printf("accx: %i --accy %i --accz %i", accel[0], accel[1], accel[2]);
        printf("gyrx: %i --gyry %i --gyrz %i", gyro[0], gyro[1], gyro[2]);
        sleep(500);

    }


}
