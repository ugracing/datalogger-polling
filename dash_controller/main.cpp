
#include "mbed.h"
#include <string>
DigitalOut myled(LED1);
Serial pc (USBTX, USBRX); // tx, rx
Serial dataLogger (p9, p10); // tx, rx
char c;
char MSserial;
int count;
string str;


typedef struct car_data{
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
    
} car_data;

car_data car_data;

void get_data() 
{
    dataLogger.baud(38400);
    pc.baud(57600);
    pc.printf("PC and Datalogger serial ready !!\n\r");
    while(1) 
	{
		if(pc.readable()) 
		{
            printf("pc is readable!");
            pc.printf("\n\r    %d  \n\r", count);
            count=0;
            c = pc.getc();          
            pc.printf("\n\r%c\n\r",c);
            dataLogger.printf("A");
        }
        count = 0;
        str = "";
        //if (!dataLogger.readable()) printf("AAAAA!!!!!");
        while (dataLogger.readable()) 
		{
            str += dataLogger.getc();
            count++;
            if (dataLogger.readable() == false)
            {
                wait(0.002);
            };
        }        
        if (count > 0)
        {
            pc.printf("\n\r    %d  \n\r", count);
            union
            {
                uint8_t b[2];
                int i;
            };
            b[1] = str[22]; b[0] = str[23];
            car_data.coolant = (i-320) * 0.05555;
            pc.printf("data %f \n\r",car_data.coolant);
            
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
            for(offset = 0; offset<7; offset++)
			{
                car_data.injectors_status[offset] = ((b[0] >> offset)  & 0x01);
                pc.printf("%d\n", car_data.injectors_status[offset]);
			}

			break;		//exits the struct filling lop when the struct has been filled
			
		};
        
	}
}

void drive_dash()
{
	//drive leds and set warning light
}

void log_data()
{
	//log the data is .tsv format
}

int main()
{
	while(1)
	{
		get_data();
		drive_dash();
		log_data();
	}
}