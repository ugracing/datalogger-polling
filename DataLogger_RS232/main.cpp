#include "mbed.h"

DigitalOut myled(LED1);
 
Serial pc (USBTX, USBRX); // tx, rx
Serial dataLogger (p9, p10);  // tx, rx

char c;
char MSserial;
int count;
//commented!
int main() {    
    
    dataLogger.baud(38400);
    pc.baud(57600);
    
    pc.printf("PC and Datalogger serial ready !!\n\r");
    while(1) {
        if(pc.readable()) {
            printf("pc is readable!");
            count=0;
            c = pc.getc();
            pc.printf("\n\r%c\n\r",c);
        
            dataLogger.printf("A\0\0");
   
        }
        while (dataLogger.readable()) {
            //printf ("datalogger readable ");
            MSserial = dataLogger.getc();
            pc.printf("%d, %X\n\r",count,(MSserial));
            count++;
        };
     
    }
}