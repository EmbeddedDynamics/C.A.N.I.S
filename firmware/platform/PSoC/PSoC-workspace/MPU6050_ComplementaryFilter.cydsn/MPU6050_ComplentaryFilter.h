/* ========================================
 * MPU6050.h
 * Simple MPU6050 implementation for PSoC5
 * Similar to Arduino style
 * ======================================== */

#ifndef MPU6050_ComplementaryFilter_H
#define MPU6050_ComplementaryFilter_H

#include "project.h"
#include <math.h>

#define MPU 0x68

// Global Variables
extern float X_Rate, Y_Rate, Z_Rate;
extern float X_Accel, Y_Accel, Z_Accel;
extern float Gyro_Angle_Roll, Gyro_Angle_Pitch, Gyro_Angle_Yaw;
extern float Accel_Angle_Roll, Accel_Angle_Pitch;
extern float Angle_Roll, Angle_Pitch, Angle_Yaw;
extern float Roll_Angle_Previous, Pitch_Angle_Previous, Yaw_Angle_Previous;
extern float X_Rate_Calibration, Y_Rate_Calibration, Z_Rate_Calibration;

// Function Prototypes
void Init_Gyro(void);
void Gyro_Calibration(void);
void Get_Gyro_Data(void);
void Read_Angle(void);

#endif /* MPU6050_ComplementaryFilter */