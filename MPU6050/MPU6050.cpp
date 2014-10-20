#include "mbed.h"
#include "MPU6050.h"

MPU6050::MPU6050(PinName sda, PinName scl):i2c(sda,scl){
    
    // set up ring buffer
    MPURingBuffer = new float[700];
    // pointer to first sample 
    MPURHead = MPURingBuffer;
    // pointer to indicate position in the Ring Buffer
    MPURPos = MPURingBuffer;
    // pointer to indicate display position in the Ring Buffer
    MPURDisplay = MPURingBuffer;     
    
    // initialize range options
    accelerometer_range=0;
    gyroscope_range=0;     
    
    // initialize all addresses to 0x00     
    write(MPU6050_RA_SELF_TEST_X,0x00);
    write(MPU6050_RA_SELF_TEST_Y,0x00);
    write(MPU6050_RA_SELF_TEST_Z,0x00);
    write(MPU6050_RA_SELF_TEST_A,0x00);
    write(MPU6050_RA_SMPLRT_DIV,0x07);
    write(MPU6050_RA_CONFIG,0x00);
    write(MPU6050_RA_GYRO_CONFIG,gyroscope_range);
    write(MPU6050_RA_ACCEL_CONFIG,accelerometer_range);
    write(MPU6050_RA_MOT_THR,0x00);
    write(MPU6050_RA_FIFO_EN,0x00);
    write(MPU6050_RA_I2C_MST_CTRL,0x00);
    write(MPU6050_RA_I2C_SLV0_ADDR,0x00);
    write(MPU6050_RA_I2C_SLV0_REG,0x00);
    write(MPU6050_RA_I2C_SLV0_CTRL,0x00);
    write(MPU6050_RA_I2C_SLV1_ADDR,0x00);
    write(MPU6050_RA_I2C_SLV1_REG,0x00);
    write(MPU6050_RA_I2C_SLV1_CTRL,0x00);
    write(MPU6050_RA_I2C_SLV2_ADDR,0x00);
    write(MPU6050_RA_I2C_SLV2_REG,0x00);
    write(MPU6050_RA_I2C_SLV2_CTRL,0x00);
    write(MPU6050_RA_I2C_SLV3_ADDR,0x00);
    write(MPU6050_RA_I2C_SLV3_REG,0x00);
    write(MPU6050_RA_I2C_SLV3_CTRL,0x00);
    write(MPU6050_RA_I2C_SLV4_ADDR,0x00);
    write(MPU6050_RA_I2C_SLV4_REG,0x00);
    write(MPU6050_RA_I2C_SLV4_DO,0x00);
    write(MPU6050_RA_I2C_SLV4_CTRL,0x00);
    write(MPU6050_RA_INT_PIN_CFG,0x00);
    write(MPU6050_RA_INT_ENABLE,0x00);
    write(MPU6050_RA_I2C_SLV0_DO,0x00);
    write(MPU6050_RA_I2C_SLV1_DO,0x00);
    write(MPU6050_RA_I2C_SLV2_DO,0x00);
    write(MPU6050_RA_I2C_SLV3_DO,0x00);
    write(MPU6050_RA_I2C_MST_DELAY_CTRL,0x00);
    write(MPU6050_RA_SIGNAL_PATH_RESET,0x00);
    write(MPU6050_RA_MOT_DETECT_CTRL,0x00);
    write(MPU6050_RA_USER_CTRL,0x00);
    write(MPU6050_RA_PWR_MGMT_1,0x02);
    write(MPU6050_RA_PWR_MGMT_2,0x00);
    write(MPU6050_RA_FIFO_COUNTH,0x00);
    write(MPU6050_RA_FIFO_COUNTL,0x00);
    write(MPU6050_RA_FIFO_R_W,0x00);
    
    // calculate gyro offset for calibration
    char temp_data[6];
        
    read(MPU6050_RA_GYRO_XOUT_H,temp_data, 6);
    
    gyroscope_offset[0] = (int)(short)((temp_data[0]<<8) + temp_data[1]);
    gyroscope_offset[1] = (int)(short)((temp_data[2]<<8) + temp_data[3]);
    gyroscope_offset[2] = (int)(short)((temp_data[4]<<8) + temp_data[5]);
    
    std::printf("Gyroscope X offset: %i\r\n",gyroscope_offset[0]);
    std::printf("Gyroscope Y offset: %i\r\n",gyroscope_offset[1]);
    std::printf("Gyroscope Z offset: %i\r\n",gyroscope_offset[2]);
    
    std::printf("Gyroscope X offset: %f\r\n",(float)gyroscope_offset[0]/131.0);
    std::printf("Gyroscope Y offset: %f\r\n",(float)gyroscope_offset[1]/131.0);
    std::printf("Gyroscope Z offset: %f\r\n",(float)gyroscope_offset[2]/131.0);
    
    // calculate accelerometer offset for calibration
    read(MPU6050_RA_ACCEL_XOUT_H,temp_data, 6);
    
    accelerometer_offset[0] = (int)(short)((temp_data[0]<<8) + temp_data[1]);
    accelerometer_offset[1] = (int)(short)((temp_data[2]<<8) + temp_data[3]);
    accelerometer_offset[2] = (int)(short)((temp_data[4]<<8) + temp_data[5]);
    
    std::printf("Accelerometer X offset: %i\r\n",accelerometer_offset[0]);
    std::printf("Accelerometer Y offset: %i\r\n",accelerometer_offset[1]);
    std::printf("Accelerometer Z offset: %i\r\n",accelerometer_offset[2]);
    
    std::printf("Accelerometer X offset: %f\r\n",(float)accelerometer_offset[0]/16384.0);
    std::printf("Accelerometer Y offset: %f\r\n",(float)accelerometer_offset[1]/16384.0);
    std::printf("Accelerometer Z offset: %f\r\n",(float)accelerometer_offset[2]/16384.0);
    
    std::printf("MPU6050 setup complete!\r\n");
}

void MPU6050::set_sleep_mode(int mode) {
    
    char temp_data[1];
    
    read(MPU6050_RA_PWR_MGMT_1,temp_data,1);

    if (mode == 1){
        temp_data[0] |= 1<<6;
    }
    if (mode == 0){
        temp_data[0] &= ~(1<<6);
    }
    write(MPU6050_RA_PWR_MGMT_1,temp_data[0]);
    std::printf("MPU6050 sleep mode set to %i!\r\n",mode);
}

void MPU6050::set_accelerometer_range(int range){
    accelerometer_range=range;
    write(MPU6050_RA_ACCEL_CONFIG,accelerometer_range);
    std::printf("MPU6050 accelerometer range set to %i!\r\n",range);
}

void MPU6050::set_gyroscope_range(int range){
    gyroscope_range=range;
    write(MPU6050_RA_GYRO_CONFIG,gyroscope_range);
    std::printf("MPU6050 gyroscope range set to %i!\r\n",range);  
}

void MPU6050::write(char address, char data) {
    
    char write_buffer[2];
    
    write_buffer[0]=address;
    write_buffer[1]=data;
    
    i2c.write(MPU6050_ADDRESS,write_buffer,2);
}

void MPU6050::read(char address, char *data, int length) {
    i2c.write(MPU6050_ADDRESS, &address, 1, true);
    i2c.read(MPU6050_ADDRESS, data, length);
}

void MPU6050::get_acceleration(){
    
    char temp_data[6];
    int raw_data[3];
    
    read(MPU6050_RA_ACCEL_XOUT_H,temp_data, 6);
    
    raw_data[0] = (int)(short)((temp_data[0]<<8) + temp_data[1]);
    raw_data[1] = (int)(short)((temp_data[2]<<8) + temp_data[3]);
    raw_data[2] = (int)(short)((temp_data[4]<<8) + temp_data[5]);
    
    if(accelerometer_range == MPU6050_ACCEL_RANGE_2G){
        acceleration[0]=((float)raw_data[0]-(float)accelerometer_offset[0]) / 16384.0;
        acceleration[1]=((float)raw_data[1]-(float)accelerometer_offset[1]) / 16384.0;
        acceleration[2]=((float)raw_data[2]-(float)accelerometer_offset[2]) / 16384.0;
    }
    if(accelerometer_range == MPU6050_ACCEL_RANGE_4G){
        acceleration[0]=(float)raw_data[0] / 8192.0;
        acceleration[1]=(float)raw_data[1] / 8192.0;
        acceleration[2]=(float)raw_data[2] / 8192.0;
    }
    if(accelerometer_range == MPU6050_ACCEL_RANGE_8G){
        acceleration[0]=(float)raw_data[0] / 4096.0;
        acceleration[1]=(float)raw_data[1] / 4096.0;
        acceleration[2]=(float)raw_data[2] / 4096.0;
    }
    if(accelerometer_range == MPU6050_ACCEL_RANGE_16G){
        acceleration[0]=(float)raw_data[0] / 2048.0;
        acceleration[1]=(float)raw_data[1] / 2048.0;
        acceleration[2]=(float)raw_data[2] / 2048.0; 
    }
    
    // store acceleration array into MPU Ring Buffer
    for(int i=0; i<3; i++) {
        if( MPURPos == &MPURingBuffer[700] )
            MPURPos = MPURingBuffer;
        else 
            MPURPos++;
                    
        *MPURPos = acceleration[i];
            }
}

void MPU6050::get_rotation(){

    char temp_data[6];
    int raw_data[3];
    
    read(MPU6050_RA_GYRO_XOUT_H,temp_data, 6);
    
    raw_data[0] = (int)(short)((temp_data[0]<<8) + temp_data[1]);
    raw_data[1] = (int)(short)((temp_data[2]<<8) + temp_data[3]);
    raw_data[2] = (int)(short)((temp_data[4]<<8) + temp_data[5]);

    if (gyroscope_range == MPU6050_GYRO_RANGE_250) {
        rotation[0]=((float)raw_data[0]-(float)gyroscope_offset[0]) / 131.0;
        rotation[1]=((float)raw_data[1]-(float)gyroscope_offset[1]) / 131.0;
        rotation[2]=((float)raw_data[2]-(float)gyroscope_offset[2]) / 131.0;
    }
    if (gyroscope_range == MPU6050_GYRO_RANGE_500){
        rotation[0]=(float)raw_data[0] / 65.5;
        rotation[1]=(float)raw_data[1] / 65.5;
        rotation[2]=(float)raw_data[2] / 65.5;
    }
    if (gyroscope_range == MPU6050_GYRO_RANGE_1000){
        rotation[0]=(float)raw_data[0] / 32.8;
        rotation[1]=(float)raw_data[1] / 32.8;
        rotation[2]=(float)raw_data[2] / 32.8;
    }
    if (gyroscope_range == MPU6050_GYRO_RANGE_2000){
        rotation[0]=(float)raw_data[0] / 16.4;
        rotation[1]=(float)raw_data[1] / 16.4;
        rotation[2]=(float)raw_data[2] / 16.4;
    }
    
    // store gyro array into MPU Ring Buffer
    for(int i=0; i<3; i++) {
        if( MPURPos == &MPURingBuffer[700] )
            MPURPos = MPURingBuffer;
        else 
            MPURPos++;
        *MPURPos = rotation[i];
            }
}

void MPU6050::get_temperature(){
    
    char temp_data[2];
    int raw_data;
    
    read(MPU6050_RA_TEMP_OUT_H,temp_data, 2);
    
    raw_data = (int)(short)((temp_data[0]<<8) + temp_data[1]);
    temperature = ((float)raw_data+521.0)/340.0+35.0;
    
    // store temperature float into MPU Ring Buffer
        if( MPURPos == &MPURingBuffer[700] )
            MPURPos = MPURingBuffer;
        else 
            MPURPos++;
        *MPURPos = temperature;
}

void MPU6050::display_MPU_data() {
        int num = 0;
        while (MPURDisplay != MPURPos) {
        if( MPURDisplay == &MPURingBuffer[700] )
                MPURDisplay = MPURingBuffer;
        else 
                MPURDisplay++;
        std::printf("%d, %f\n\r",num,*MPURDisplay);
        num++;   
        }
}

MPU6050::~MPU6050() { 
}