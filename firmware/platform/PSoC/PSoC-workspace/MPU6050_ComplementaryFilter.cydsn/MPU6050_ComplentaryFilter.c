/* ========================================
 * MPU6050.c
 * Simple MPU6050 implementation for PSoC5
 * Similar to Arduino style
 * ======================================== */

#include "MPU6050_ComplentaryFilter.h"
#include <math.h>

// Global Variables
float X_Rate, Y_Rate, Z_Rate;
float X_Accel, Y_Accel, Z_Accel;
float Gyro_Angle_Roll, Gyro_Angle_Pitch, Gyro_Angle_Yaw;
float Accel_Angle_Roll, Accel_Angle_Pitch;
float Angle_Roll, Angle_Pitch, Angle_Yaw;
float Roll_Angle_Previous, Pitch_Angle_Previous, Yaw_Angle_Previous;
float X_Rate_Calibration, Y_Rate_Calibration, Z_Rate_Calibration;
int RateCalibrationNumber;

//========================================================================================================================//
//                                                  Function Init_Gyro                                                    //                                                                 
//========================================================================================================================//

void Init_Gyro(void) {
    // Init I2C hardware
    I2C_1_Start();
    CyDelay(100);
    
    // Disable sleep mode (default on when powering up)
    I2C_1_MasterSendStart(MPU, I2C_1_WRITE_XFER_MODE); // Gives a read bit
    I2C_1_MasterWriteByte(0x6B);
    I2C_1_MasterWriteByte(0x00);
    I2C_1_MasterSendStop();
    CyDelay(10);
    
    // Set DLPF at 10 Hz
    I2C_1_MasterSendStart(MPU, I2C_1_WRITE_XFER_MODE);
    I2C_1_MasterWriteByte(0x1A);
    I2C_1_MasterWriteByte(0x05);
    I2C_1_MasterSendStop();
    CyDelay(10);
    
    // Set Gyro Full Scale Range at +-500 dps
    I2C_1_MasterSendStart(MPU, I2C_1_WRITE_XFER_MODE);
    I2C_1_MasterWriteByte(0x1B);
    I2C_1_MasterWriteByte(0x08);
    I2C_1_MasterSendStop();
    CyDelay(10);
    
    // Set Accel Full Scale Range at +-4g
    I2C_1_MasterSendStart(MPU, I2C_1_WRITE_XFER_MODE);
    I2C_1_MasterWriteByte(0x1C);
    I2C_1_MasterWriteByte(0x08);
    I2C_1_MasterSendStop();
    CyDelay(10);
}

//========================================================================================================================//
//                                                 Function Gyro_Calibration                                              //                                                                 
//========================================================================================================================//

void Gyro_Calibration(void) {
    // Get 2000 samples for each axis
    for(RateCalibrationNumber = 0; RateCalibrationNumber < 2000; RateCalibrationNumber++) {
        Get_Gyro_Data();
        X_Rate_Calibration += X_Rate;
        Y_Rate_Calibration += Y_Rate;
        Z_Rate_Calibration += Z_Rate;
        CyDelay(1);
    }
    
    // Divide with 2000 to remove drift
    X_Rate_Calibration /= 2000;
    Y_Rate_Calibration /= 2000;
    Z_Rate_Calibration /= 2000;
}

//========================================================================================================================//
//                                                  Function Get_Gyro_Data                                                //                                                                 
//========================================================================================================================//

void Get_Gyro_Data(void) {
    uint8 buffer[6];
    int16_t raw_X_Rate, raw_Y_Rate, raw_Z_Rate;
    int16_t raw_X_Accel, raw_Y_Accel, raw_Z_Accel;
    
    // Go to GYRO_XOUT[15:8]
    I2C_1_MasterSendStart(MPU, I2C_1_WRITE_XFER_MODE);
    I2C_1_MasterWriteByte(0x43);
    I2C_1_MasterSendStop();
    
    // Read 6 bytes and write it to a buffer
    I2C_1_MasterSendStart(MPU, I2C_1_READ_XFER_MODE);
    buffer[0] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);   // ACK so PSoC5 wants more data
    buffer[1] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
    buffer[2] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
    buffer[3] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
    buffer[4] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
    buffer[5] = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
    I2C_1_MasterSendStop();
    
    raw_X_Rate = (int16_t)((buffer[0] << 8) | buffer[1]);
    raw_Y_Rate = (int16_t)((buffer[2] << 8) | buffer[3]);
    raw_Z_Rate = (int16_t)((buffer[4] << 8) | buffer[5]);
    
    // Go to ACCEL_XOUT[15:8]
    I2C_1_MasterSendStart(MPU, I2C_1_WRITE_XFER_MODE);
    I2C_1_MasterWriteByte(0x3B);
    I2C_1_MasterSendStop();
    
    // Read 6 bytes
    I2C_1_MasterSendStart(MPU, I2C_1_READ_XFER_MODE);
    buffer[0] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
    buffer[1] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
    buffer[2] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
    buffer[3] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
    buffer[4] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
    buffer[5] = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
    I2C_1_MasterSendStop();
    
    raw_X_Accel = (int16_t)((buffer[0] << 8) | buffer[1]);
    raw_Y_Accel = (int16_t)((buffer[2] << 8) | buffer[3]);
    raw_Z_Accel = (int16_t)((buffer[4] << 8) | buffer[5]);
    
    // To get rate per second eliminating LSB
    X_Rate = (float)raw_X_Rate / 65.5;
    Y_Rate = (float)raw_Y_Rate / 65.5;
    Z_Rate = (float)raw_Z_Rate / 65.5;
    
    // To get g's eliminating LSB
    X_Accel = (float)raw_X_Accel / 8192;
    Y_Accel = (float)raw_Y_Accel / 8192;
    Z_Accel = (float)raw_Z_Accel / 8192;
}

//========================================================================================================================//
//                                             Function Complementary filter                                              //                                                                 
//========================================================================================================================//

void Read_Angle(void) {
    Get_Gyro_Data();
    
    X_Rate -= X_Rate_Calibration;
    Y_Rate -= Y_Rate_Calibration;
    Z_Rate -= Z_Rate_Calibration;
    
    Roll_Angle_Previous = Angle_Roll;
    Pitch_Angle_Previous = Angle_Pitch;
    Yaw_Angle_Previous = Angle_Yaw;
    
    Gyro_Angle_Roll += X_Rate * 0.004;
    Gyro_Angle_Pitch += Y_Rate * 0.004;
    Gyro_Angle_Yaw += Z_Rate * 0.004;
    
    Accel_Angle_Roll = atan2(Y_Accel, sqrt((X_Accel*X_Accel)+(Z_Accel*Z_Accel))) * 180.0 / M_PI;
    Accel_Angle_Pitch = atan2(X_Accel, sqrt((Y_Accel*Y_Accel)+(Z_Accel*Z_Accel))) * 180.0 / M_PI;
    
    Angle_Roll = 0.02 * Accel_Angle_Roll + 0.98 * (X_Rate * 0.004 + Roll_Angle_Previous);
    Angle_Pitch = 0.02 * Accel_Angle_Pitch + 0.98 * (Y_Rate * 0.004 + Pitch_Angle_Previous);
}