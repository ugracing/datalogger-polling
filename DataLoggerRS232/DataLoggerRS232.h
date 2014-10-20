/* Example
    @code
#include "mbed.h"
#include "DataLoggerRS232.h"
 
Serial pc (USBTX, USBRX); // tx, rx
DataLoggerRS232 dataLogger (p9,p10);  // tx, rx

int readnum = 1;
char DLcommand;
char MS;

int main() {    

    pc.baud(115200);
    dataLogger.baud(115200);
    pc.printf("PC and Datalogger serial set up complete !!\n\r");
    
    while(1) {
        
        if (pc.readable()) {
            readnum = 1;
            dataLogger.count = 0;
            DLcommand = pc.getc();
            pc.printf("\n\r%c\n\r",DLcommand);
            dataLogger.putc(DLcommand);
            
            //while (dataLogger.readable()) {
              //MS = dataLogger.getc();
              //pc.printf("%X\n\r",dataLogger.getc());  
              //pc.printf("%d,\n\r",readnum);  
              //readnum++;
              //  }
            //pc.printf("%d,\n\r",readnum);
             
            dataLogger.get_ECU_databyte();
        }
        dataLogger.display_ECU_databyte();     
    }
}
@code end
*/

#include "mbed.h"

#ifndef DataLogger_RS232_H_
#define DataLogger_RS232_H_

class DataLoggerRS232 : public Serial
{
public:
    
    DataLoggerRS232(PinName tx,PinName rx, const char* name=NULL);
    
    virtual ~DataLoggerRS232();
    
    int count;
    void get_ECU_databyte();
    void display_ECU_databyte();
   
private:
    
    char c;
    char MSserial;
    
    char* serialRingBuffer;
    char* serialRHead;
    char* serialRPos;
    char* serialRDisplay;
    

};

#endif // DataLogger_RS232_H_ 