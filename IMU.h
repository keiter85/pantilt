#ifndef __IMU_H
#define __IMU_H

//#include "Public_StdTypes.h"
//#include "MPU9250.h"
//#include "BMP180.h"

//#define M_PI  (float)3.1415926535

extern int16_t accel[3], gyro[3];
extern float angles[3];

extern void IMU_Init(void); 
extern void IMU_GetYawPitchRoll(float *Angles) ;

#endif
