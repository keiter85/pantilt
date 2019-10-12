#include "MPU9250.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "stdio.h"

int16_t magn[3];
int16_t accel[3], gyro[3];
unsigned char BUF[10];
MPU9250_TypeDef MPU9250_Offset={0};
MPU9250_TypeDef_Off MPU9250_Magn_Offset={0}

bool I2C_WriteOneByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t Data){
    wiringPiI2CWriteReg8(DevAddr, RegAddr, Data);
    return true;
}

uint8_t I2C_ReadOneByte(uint8_t DevAddr, uint8_t RegAddr){
    uint8_t TempVal = 0;
    TempVal = wiringPiI2CRedReg8(DevAddr, RegAddr);
    return TempVal;
}

/**
  * @brief  Initializes MPU9250
  * @param  None
  * @retval None
  */
	void MPU9250_Init(void)
{
  I2C_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x00);
	I2C_WriteOneByte(GYRO_ADDRESS,SMPLRT_DIV, 0x07);
	I2C_WriteOneByte(GYRO_ADDRESS,CONFIG, 0x06);
	I2C_WriteOneByte(GYRO_ADDRESS,GYRO_CONFIG, 0x10);
	I2C_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG, 0x01);
	
	delay(10);
	if(MPU9250_Check())
	{
		printf("Comms with MPU9250 established");
	}
	else
	{
		printf("Comms with MPU9250 failed!!!!");
	}
	
	MPU9250_InitGyrOffset();
}

/**
  * @brief Get accelerometer datas
  * @param  None
  * @retval None
  */
void MPU9250_READ_ACCEL(void)
{ 
   uint8_t i;
	 int16_t InBuffer[3] = {0}; 
	 static int32_t OutBuffer[3] = {0};
	 static MPU9250_AvgTypeDef MPU9250_Filter[3];

   BUF[0]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_L); 
   BUF[1]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_H);
   InBuffer[0]=	(BUF[1]<<8)|BUF[0];

   BUF[2]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_L);
   BUF[3]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_H);
   InBuffer[1]=	(BUF[3]<<8)|BUF[2];
					   
   BUF[4]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_L);
   BUF[5]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_H);
   InBuffer[2]=	(BUF[5]<<8)|BUF[4];			       
   
   for(i = 0; i < 3; i ++)	
   {
      MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
   }
   accel[0] = *(OutBuffer + 0);
   accel[1] = *(OutBuffer + 1);
   accel[2] = *(OutBuffer + 2); 
}

/**
  * @brief Get gyroscopes datas
  * @param  None
  * @retval None
  */
void MPU9250_READ_GYRO(void)
{ 
   uint8_t i;
	 int16_t InBuffer[3] = {0}; 
	 static int32_t OutBuffer[3] = {0};
	 static MPU9250_AvgTypeDef MPU9250_Filter[3];

   BUF[0]=I2C_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_L); 
   BUF[1]=I2C_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_H);
   InBuffer[0]=	(BUF[1]<<8)|BUF[0];
   
   BUF[2]=I2C_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_L);
   BUF[3]=I2C_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_H);
   InBuffer[1] = (BUF[3]<<8)|BUF[2];
    
   BUF[4]=I2C_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_L);
   BUF[5]=I2C_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_H);
   InBuffer[2] = (BUF[5]<<8)|BUF[4];	

   for(i = 0; i < 3; i ++)	
   {
      MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
   }
   gyro[0] = *(OutBuffer + 0) - MPU9250_Offset.X;
   gyro[1] = *(OutBuffer + 1) - MPU9250_Offset.Y;
   gyro[2] = *(OutBuffer + 2) - MPU9250_Offset.Z;
}

/**
  * @brief Get compass datas
  * @param  None
  * @retval None
  */
void MPU9250_READ_MAG(void)
{ 
   uint8_t i;
	 int16_t InBuffer[3] = {0}; 
	 static int32_t OutBuffer[3] = {0};
	 static MPU9250_AvgTypeDef MPU9250_Filter[3];

    I2C_WriteOneByte(GYRO_ADDRESS,0x37,0x02);//turn on Bypass Mode 
    delay(10);
    I2C_WriteOneByte(MAG_ADDRESS,0x0A,0x01);	
    delay(10);
    BUF[0]=I2C_ReadOneByte (MAG_ADDRESS,MAG_XOUT_L);
    BUF[1]=I2C_ReadOneByte (MAG_ADDRESS,MAG_XOUT_H);
    InBuffer[1] =(BUF[1]<<8)|BUF[0];

    BUF[2]=I2C_ReadOneByte(MAG_ADDRESS,MAG_YOUT_L);
    BUF[3]=I2C_ReadOneByte(MAG_ADDRESS,MAG_YOUT_H);
    InBuffer[0] =	(BUF[3]<<8)|BUF[2];
    
    BUF[4]=I2C_ReadOneByte(MAG_ADDRESS,MAG_ZOUT_L);
    BUF[5]=I2C_ReadOneByte(MAG_ADDRESS,MAG_ZOUT_H);
    InBuffer[2] =	(BUF[5]<<8)|BUF[4];	
    InBuffer[2] = -InBuffer[2];
	 
   for(i = 0; i < 3; i ++)	
   {
      MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
   } 
	    magn[0] = *(OutBuffer + 0)-MPU9250_Magn_Offset.X_Off_Err;
			magn[1] = *(OutBuffer + 1)-MPU9250_Magn_Offset.Y_Off_Err;
			magn[2] = *(OutBuffer + 2)-MPU9250_Magn_Offset.Z_Off_Err;
}

/**
  * @brief  Check MPU9250,ensure communication succeed
  * @param  None
  * @retval true: communicate succeed
  *               false: communicate fualt 
  */
bool MPU9250_Check(void) 
{
   	if(WHO_AM_I_VAL == I2C_ReadOneByte(DEFAULT_ADDRESS, WHO_AM_I))  
   	{
   		return true;
   	}
   	else 
   	{
   		return false;
   	}	
}

/**
  * @brief  Digital filter
  * @param *pIndex:
  * @param *pAvgBuffer:
  * @param InVal:
  * @param pOutVal:
  *
  * @retval None
  *               
  */
void MPU9250_CalAvgValue(uint8_t *pIndex, int16_t *pAvgBuffer, int16_t InVal, int32_t *pOutVal)
{	
	uint8_t i;
	
	*(pAvgBuffer + ((*pIndex) ++)) = InVal;
  	*pIndex &= 0x07;
  	
  	*pOutVal = 0;
	for(i = 0; i < 8; i ++) 
  	{
    	*pOutVal += *(pAvgBuffer + i);
  	}
  	*pOutVal >>= 3;
}
/**
  * @brief  Initializes gyroscopes offset
  * @param  None
  * @retval None
  */
void MPU9250_InitGyrOffset(void)
{
	uint8_t i;
	int32_t	TempGx = 0, TempGy = 0, TempGz = 0;
	
 	for(i = 0; i < 32; i ++)
 	{
		MPU9250_READ_GYRO();
		
		TempGx += gyro[0];
		TempGy += gyro[1];
		TempGz += gyro[2];

	    delayMicroseconds(100);
	}

	MPU9250_Offset.X = TempGx >> 5;
	MPU9250_Offset.Y = TempGy >> 5;
	MPU9250_Offset.Z = TempGz >> 5;

}
