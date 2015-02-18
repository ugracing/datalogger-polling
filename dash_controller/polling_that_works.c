//
// polling_that_works.c
//
//
// Created by Laura on 17/02/15.
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
Timer SystemTime;
int byte_count;
string str;

struct car_datastruct
{
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

void drive_dash(float RPM, float CLT)
{
	for(int i = 0; i< 10000; i = i+ 100)
	{
		RPM = i;
		wait(0.2);
		LED_driver = RPM/10000;
	};

	if(CLT>20)
	{
		LED_CLT = 1;
	}
	else 
	{
		LED_CLT = 0;
	};
};

int IntMergeBytes(uint8_t b1, uint8_t b2)
{
    union
    {
        int i;
        uint8_t b[2];
    };
    
    b[1] = b1; b[2] = b2;
    return i;
};

void rx_data()
{
	int shiftoffset;
	uint8_t b;
	
	str = "";
	byte_count=0;
	while (1)
	{
		if (msquirt.readable())
		{
			str += msquirt.getc();
			byte_count++;
		};
		
		if (byte_count >= 209)
		{
			break;
		};
	};

	car_data.coolant = (IntMergeBytes(str[32], str[22]) -320) * 0.05555;	
	pc.printf("CLT = %f \r\n",car_data.coolant);
	
	car_data.RPM = IntMergeBytes(str[7], str[6]);
	pc.printf("rpm %f \n\r",car_data.RPM);
	
	//CHECKME!
	car_data.air_fuel_1 = IntMergeBytes(str[29], str[28]) * 0.1;
	pc.printf("data %f \n\r",car_data.air_fuel_1);

	car_data.map = IntMergeBytes(str[19], str[18]) * 0.1;
	pc.printf("data %f \n\r",car_data.map);

	car_data.mat = (IntMergeBytes(str[21], str[20]) - 320) * 0.05555;

	car_data.throttle = IntMergeBytes(str[25], str[24]) * 0.1;

	car_data.battery = IntMergeBytes(str[27], str[26]) * 0.1;
	
	//Redundant
	//car_data.air_fuel_1 = IntMergeBytes(str[29], str[28]) * 0.1;
	//car_data.air_fuel_2 = IntMergeBytes(str[29], str[28]) * 0.1;
	
	b = str[10];
	for(shiftoffset = 0; shiftoffset<7; shiftoffset++)
	{
		car_data.injectors_status[shiftoffset] = ((b >> shiftoffset) & 0x01);
		pc.printf("%d", car_data.injectors_status[shiftoffset]);
	};
	
	pc.printf("got %d bytes\r\n", byte_count);
};

void log_data()
{
	//fprintf(fp,"%.1f\370C\t%.1f\370C\n\r",temp(Tin), temp(Tout));
};

int main()
{
	SystemTime.start();
	wait(2); //Hang the mbed at start - useful to spot resets
	FILE *fp = fopen( "/msc/data.txt", "w");
	msquirt.baud(38400);
	pc.baud(57600);
	
	while(1)
	{
		printf("TIME IS: %f \r\n seconds", SystemTime.read());
		msquirt.putc('A');
		rx_data();
	};
};
