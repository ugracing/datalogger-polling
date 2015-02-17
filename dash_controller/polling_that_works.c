//
//  polling_that_works.c
//  
//
//  Created by Laura on 17/02/15.
//
//
#include "cmath"
#include "mbed.h"
#include <stdio.h>
#include <string>
#define FSNAME "msc"



Serial pc(USBTX, USBRX); // tx, rx
Serial msquirt(p9, p10); // tx, rx
DigitalOut myled(LED1);
AnalogOut LED_driver(p18);
DigitalOut LED_CLT(p21);

//USB:
//MSCFileSystem msc("msc"); //USB file
//AnalogIn Tin(p16);
//AnalogIn Tout(p17);


Timer t;
int byte_count;
string str;
bool fetch;
int count;

struct car_datastruct{
    float RPM;
    float map;
    float mat;
    float coolant;
    float throttle;
    float battery;
    float air_fuel_1;
    float air_fuel_2;
    bool engine_status[8];
    bool injectors_status[7];
};

car_datastruct car_data;

void drive_dash(float RPM, float CLT){
    for(int i = 0; i< 10000; i = i+ 100){
        RPM = i;
        wait(0.2);
        LED_driver = RPM/10000;
    }
    if(CLT>20){
        LED_CLT = 1;
    }
    else LED_CLT = 0;
}


void rx_data(){
    //__disable_irq();    // Disable Interrupts
    
    count++;
    
    str = "";
    byte_count=0;
    while (/*msquirt.readable()*/ 1){
        if (msquirt.readable())
        {
            str += msquirt.getc();
            
            byte_count++;
        };
        //pc.printf("\r\n %d" , byte_count);
        /*if (!msquirt.readable()){
         wait_ms(2.5);
         }*/
        if (byte_count >= 209)
        {
            break;
        }
    }
    //wait(0.001);
    union{
        uint8_t b[2];
        int i;
    };
    b[1] = str[22]; b[0] = str[23];
    car_data.coolant = (i-320) * 0.05555;
    pc.printf("CLT = %f \r\n",car_data.coolant);
    b[1] = str[6]; b[0] = str[7];
    car_data.RPM = i;
    pc.printf("rpm %f \n\r",car_data.RPM);
    pc.printf("coolant %f \n\r",car_data.coolant);
    b[1] = str[28];b[0] = str[29];
    car_data.air_fuel_1 = i*0.1;
    pc.printf("data %f \n\r",car_data.air_fuel_1);
    b[1] = str[18];b[0] = str[19];
    car_data.map = i*0.1;
    pc.printf("data %f \n\r",car_data.map);
    b[1] = str[20];b[0] = str[21];
    car_data.mat = (i-320)*0.05555;
    b[1] = str[24];b[0] = str[25];
    car_data.throttle = i*0.1;
    b[1] = str[26];b[0] = str[27];
    car_data.battery = i*0.1;
    b[1] = str[28];b[0] = str[29];
    car_data.air_fuel_1 = i*0.1;
    b[1] = str[28];b[0] = str[29];
    car_data.air_fuel_2 = i*0.1;
    int offset;
    b[0] = str[10];
    for(offset = 0; offset<7; offset++){
        car_data.injectors_status[offset] = ((b[0] >> offset) & 0x01);
        pc.printf("%d", car_data.injectors_status[offset]);
    }
    pc.printf("got %d bytes\r\n", byte_count);
    str = "";
    byte_count = 0;
    //pc.printf("set to zer mofofoker");
    fetch = true;
    // __enable_irq();     // Enable Interrupts
    //pc.printf("fetch set true");
    //wait(0.2);
}

void log_data()
{
    //fprintf(fp,"%.1f\370C\t%.1f\370C\n\r",temp(Tin), temp(Tout));
}

int main(){
    //wait(1);
    
    FILE *fp = fopen( "/msc/data.txt", "w");
    msquirt.baud(38400);
    pc.baud(57600);
    pc.printf("main");
    //msquirt.attach(&rx_data, Serial::RxIrq);
    fetch = true;
    t.start();
    while(1){
        
        //wait_us(100);
        if(fetch)
        {
            printf("TIME IS: %f \r\n seconds", t.read());
            msquirt.putc('A');
            //wait(0.01);
            /// fetch = false;
            rx_data();
        }
        else if (!fetch)
        {
            count++;
        }
    }
}


