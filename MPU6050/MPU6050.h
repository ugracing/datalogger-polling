/*Example
@code
#include "mbed.h"
#include "MPU6050.h"


Serial serial(USBTX, USBRX);

int main() {
   
    serial.baud(57600);
    MPU6050 MPU6050(p28,p27);
    MPU6050.set_sleep_mode(0);
    MPU6050.set_accelerometer_range(0);
    MPU6050.set_gyroscope_range(0);

    while(1){
        MPU6050.get_rotation();
        MPU6050.get_acceleration();
        MPU6050.get_temperature();
        serial.printf("Acceleration X,Y,Z (m/s^2): %f,%f,%f\r\n",MPU6050.acceleration[0],MPU6050.acceleration[1],MPU6050.acceleration[2]);
        serial.printf("Rotation X,Y,Z (deg/s): %f,%f,%f\r\n",MPU6050.rotation[0],MPU6050.rotation[1],MPU6050.rotation[2]);
        serial.printf("Temerature : %f\r\n",MPU6050.temperature);
        wait(0.1);
    }  
}
@endcode
*/

#include "mbed.h"

#ifndef MPU6050_H_
#define MPU6050_H_

#define MPU6050_ADDRESS 0xD0

#define MPU6050_RA_SELF_TEST_X 0x0D
#define MPU6050_RA_SELF_TEST_Y 0x0E
#define MPU6050_RA_SELF_TEST_Z 0x0F
#define MPU6050_RA_SELF_TEST_A 0x10
#define MPU6050_RA_SMPLRT_DIV 0x19
#define MPU6050_RA_CONFIG 0x1A
#define MPU6050_RA_GYRO_CONFIG 0x1B
#define MPU6050_RA_ACCEL_CONFIG 0x1C
#define MPU6050_RA_MOT_THR 0x1F
#define MPU6050_RA_FIFO_EN 0x23
#define MPU6050_RA_I2C_MST_CTRL 0x24
#define MPU6050_RA_I2C_SLV0_ADDR 0x25
#define MPU6050_RA_I2C_SLV0_REG 0x26
#define MPU6050_RA_I2C_SLV0_CTRL 0x27
#define MPU6050_RA_I2C_SLV1_ADDR 0x28
#define MPU6050_RA_I2C_SLV1_REG 0x29
#define MPU6050_RA_I2C_SLV1_CTRL 0x2A
#define MPU6050_RA_I2C_SLV2_ADDR 0x2B
#define MPU6050_RA_I2C_SLV2_REG 0x2C
#define MPU6050_RA_I2C_SLV2_CTRL 0x2D
#define MPU6050_RA_I2C_SLV3_ADDR 0x2E
#define MPU6050_RA_I2C_SLV3_REG 0x2F
#define MPU6050_RA_I2C_SLV3_CTRL 0x30
#define MPU6050_RA_I2C_SLV4_ADDR 0x31
#define MPU6050_RA_I2C_SLV4_REG 0x32
#define MPU6050_RA_I2C_SLV4_DO 0x33
#define MPU6050_RA_I2C_SLV4_CTRL 0x34
#define MPU6050_RA_I2C_SLV4_DI 0x35
#define MPU6050_RA_I2C_MST_STATUS 0x36
#define MPU6050_RA_INT_PIN_CFG 0x37
#define MPU6050_RA_INT_ENABLE 0x38
#define MPU6050_RA_INT_STATUS 0x3A
#define MPU6050_RA_ACCEL_XOUT_H 0x3B
#define MPU6050_RA_ACCEL_XOUT_L 0x3C
#define MPU6050_RA_ACCEL_YOUT_H 0x3D
#define MPU6050_RA_ACCEL_YOUT_L 0x3E
#define MPU6050_RA_ACCEL_ZOUT_H 0x3F
#define MPU6050_RA_ACCEL_ZOUT_L 0x40
#define MPU6050_RA_TEMP_OUT_H 0x41
#define MPU6050_RA_TEMP_OUT_L 0x42
#define MPU6050_RA_GYRO_XOUT_H 0x43
#define MPU6050_RA_GYRO_XOUT_L 0x44
#define MPU6050_RA_GYRO_YOUT_H 0x45
#define MPU6050_RA_GYRO_YOUT_L 0x46
#define MPU6050_RA_GYRO_ZOUT_H 0x47
#define MPU6050_RA_GYRO_ZOUT_L 0x48
#define MPU6050_RA_EXT_SENS_DATA_00 0x49
#define MPU6050_RA_EXT_SENS_DATA_01 0x4A
#define MPU6050_RA_EXT_SENS_DATA_02 0x4B
#define MPU6050_RA_EXT_SENS_DATA_03 0x4C
#define MPU6050_RA_EXT_SENS_DATA_04 0x4D
#define MPU6050_RA_EXT_SENS_DATA_05 0x4E
#define MPU6050_RA_EXT_SENS_DATA_06 0x4F
#define MPU6050_RA_EXT_SENS_DATA_07 0x50
#define MPU6050_RA_EXT_SENS_DATA_08 0x51
#define MPU6050_RA_EXT_SENS_DATA_09 0x52
#define MPU6050_RA_EXT_SENS_DATA_10 0x53
#define MPU6050_RA_EXT_SENS_DATA_11 0x54
#define MPU6050_RA_EXT_SENS_DATA_12 0x55
#define MPU6050_RA_EXT_SENS_DATA_13 0x56
#define MPU6050_RA_EXT_SENS_DATA_14 0x57
#define MPU6050_RA_EXT_SENS_DATA_15 0x58
#define MPU6050_RA_EXT_SENS_DATA_16 0x59
#define MPU6050_RA_EXT_SENS_DATA_17 0x5A
#define MPU6050_RA_EXT_SENS_DATA_18 0x5B
#define MPU6050_RA_EXT_SENS_DATA_19 0x5C
#define MPU6050_RA_EXT_SENS_DATA_20 0x5D
#define MPU6050_RA_EXT_SENS_DATA_21 0x5E
#define MPU6050_RA_EXT_SENS_DATA_22 0x5F
#define MPU6050_RA_EXT_SENS_DATA_23 0x60
#define MPU6050_RA_I2C_SLV0_DO 0x63
#define MPU6050_RA_I2C_SLV1_DO 0x64
#define MPU6050_RA_I2C_SLV2_DO 0x65
#define MPU6050_RA_I2C_SLV3_DO 0x66
#define MPU6050_RA_I2C_MST_DELAY_CTRL 0x67
#define MPU6050_RA_SIGNAL_PATH_RESET 0x68
#define MPU6050_RA_MOT_DETECT_CTRL 0x69
#define MPU6050_RA_USER_CTRL 0x6A
#define MPU6050_RA_PWR_MGMT_1 0x6B
#define MPU6050_RA_PWR_MGMT_2 0x6C
#define MPU6050_RA_FIFO_COUNTH 0x72
#define MPU6050_RA_FIFO_COUNTL 0x73
#define MPU6050_RA_FIFO_R_W 0x74
#define MPU6050_RA_WHO_AM_I 0x75

#define MPU6050_ACCEL_RANGE_2G   0
#define MPU6050_ACCEL_RANGE_4G   1
#define MPU6050_ACCEL_RANGE_8G   2
#define MPU6050_ACCEL_RANGE_16G  3
 
#define MPU6050_GYRO_RANGE_250      0
#define MPU6050_GYRO_RANGE_500      1
#define MPU6050_GYRO_RANGE_1000     2
#define MPU6050_GYRO_RANGE_2000     3
 
class MPU6050 {
public:
    
    MPU6050(PinName,PinName);
    
    void set_sleep_mode(int);
    void set_accelerometer_range(int);
    void set_gyroscope_range(int);
    
    void get_acceleration();
    void get_rotation();
    void get_temperature();
    void display_MPU_data();
    
    float acceleration[3];
    float rotation[3];
    float temperature;
    
    virtual ~MPU6050();
   
private:

    I2C i2c;

    int accelerometer_range;
    int gyroscope_range;
    
    int gyroscope_offset[3];
    int accelerometer_offset[3];
    
    float* MPURingBuffer;
    float* MPURHead;
    float* MPURPos;
    float* MPURDisplay;
    
    void write(char,char);
    void read(char,char *,int);

};
 
#endif /* MPU6050_H_ */