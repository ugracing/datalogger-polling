#include "mbed.h"
#include "DataLoggerRS232.h"

DataLoggerRS232::DataLoggerRS232(PinName tx, PinName rx, const char* name) : Serial(tx, rx, name) {
    
    // set up ring buffer
    serialRingBuffer = new char[2200];
    // pointer to first sample 
    serialRHead = serialRingBuffer;
    // pointer to indicate position in the Ring Buffer
    serialRPos = serialRingBuffer;
    // pointer to indicate display position in the Ring Buffer
    serialRDisplay = serialRingBuffer;
    
    std::printf("constructor complete\r\n");
}

void DataLoggerRS232::get_ECU_databyte() {
        int num = 0;
        for(int i=0;i<209;i++) {         // from the megasquirtii.ini file states that 208 bytes are returning from ECU for command "A"
            if( serialRPos == &serialRingBuffer[2200] )
            serialRPos = serialRingBuffer;
            else 
            serialRPos++;
                
        *serialRPos = Serial::getc();
        num++;
            }
        std::printf("total btye count: %d !!\r\n", num);
        //std::printf("get_ECU_databyte !!\r\n");
}

void DataLoggerRS232::display_ECU_databyte() {
        int num = 0;
        while (serialRDisplay != serialRPos) {
                
                if( serialRDisplay == &serialRingBuffer[2200] )
                    serialRDisplay = serialRingBuffer;
                else 
                    serialRDisplay++;
            
            std::printf("%d, %X\n\r",num,*serialRDisplay);
            num++;   
        }
}


DataLoggerRS232::~DataLoggerRS232() {
}