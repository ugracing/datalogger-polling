#include "mbed.h"
#include <string>
DigitalOut myled(LED1);
AnalogOut LED_driver(p18);
DigitalOut LED_CLT(p21);
Serial pc (USBTX, USBRX); // tx, rx
Serial dataLogger (p9, p10); // tx, rx
char c;
char MSserial;
int count;
string str;

typedef struct car_data
{
    float time;
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

void flushSerialBuffer(void) { char char1 = 0; while (dataLogger.readable()) { char1 = dataLogger.getc(); } return; }


car_data get_data()
{
    //pc.printf("getting DTAT\n\r");
    car_data car_data;
    //pc.printf("made car_data object\n\r");
    dataLogger.baud(57600);
    pc.baud(57600);
    //pc.printf("set bauds\n\r");
    
    //if(pc.readable())
    //{
    //pc.printf("pc is readable!");
    pc.printf("\n\r %d \n\r", count);
    count=0;
    // c = pc.getc();
    
    // pc.printf("\n\r%c\n\r",c);
    flushSerialBuffer();
    //wait(0.1);
    dataLogger.printf("A");
    
    //    }
    //pc.printf("after if ");
    count = 0;
    str = "";
    
    if (!dataLogger.readable())
    {
        pc.printf("AAAAA!!!!!");
    }
    else
    {
        pc.printf(" ");
    }
    //wait(0.01);
    while (dataLogger.readable())
    {
        //wait(0.02);
        str += dataLogger.getc();
        count++;
        if (dataLogger.readable() == false)
        {
            wait(0.002);
        };
        
    }
    flushSerialBuffer();
    if (count > 0)
    {
        pc.printf("\n\r %d \n\r", count);
        union
        {
            uint8_t b[2];
            int i;
        };
        b[1] = str[22]; b[0] = str[23];
        car_data.coolant = (i-320) * 0.05555;
        
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
        for(offset = 0; offset<7; offset++)
        {
            car_data.injectors_status[offset] = ((b[0] >> offset) & 0x01);
            pc.printf("%d", car_data.injectors_status[offset]);
        } //exits the struct filling lop when the struct has been filled
    };
    return car_data;
}

void drive_dash(float RPM, float CLT)
{
    for(int i  = 0; i< 10000; i = i+ 100){
        RPM = i;
        wait(0.2);
        
        LED_driver = RPM/10000;
    }
    if(CLT>20)
    {
        LED_CLT = 1;
    }
    else LED_CLT = 0;
    
}

void log_data()
{
    //log the data is .tsv format
}
int main()
{
    dataLogger.baud(57600);
    pc.baud(57600);
    //pc.printf("");
    while(1)
    {   //wait(0.02);
        car_data car_data = get_data();
        // drive_dash(car_data.RPM, car_data.coolant);
        log_data();
    }
}