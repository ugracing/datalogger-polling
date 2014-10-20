/* DASH display example as following:

#include "mbed.h"

AnalogOut rpm_pin(p18);
DigitalOut gear_pins[] = {p5, p6, p7, p8}; //p5-2^3 p6 -2^2 p7 - 2^1 p8- 2^0

 
int display_rpm(int rpm){
    rpm_pin = (float)rpm/(float)10000;//Check engine max rpm
    return rpm;
}

int display_gear(int gear){ 
    for(int i=2;i>=0;i--){ 
        gear_pins[2-i+1]=(gear>>i)&1;
    }
    return gear;
}

int main() {

   Serial usb_serial(USBTX, USBRX);
    usb_serial.baud(9600);

   int i=0;
   while(1){
        
        int q=0;
        while(q<=10000){
            display_rpm(q);
            q=q+1000;   
            wait(0.1);
        }
        q=0;
        usb_serial.printf("Gear-->%i\r\n",display_gear(i));
        i++;
        if(i==6){
            i=0;
        }
    }
}*/

#include "mbed.h"
#include "DataLoggerRS232.h"
#include "MPU6050.h"
#include "SDFileSystem.h"
 
Serial pc (USBTX, USBRX); // tx, rx
DataLoggerRS232 dataLogger (p9,p10);  // tx, rx

int readnum = 1;
char DLcommand;
char MS;

//*************************************//
// Match Timer 2 for precise interrupt // 
//*************************************//

void TIMER2_IRQHandler(void) {
        
    if (((LPC_TIM2->IR) & 0x00000001 ) == 0x00000001 ) {      // TIM2 MR0 interrupt service routine !!
        
        LPC_TIM2->IR |= 0x00000001;     // clear MR0 interrupt register by setting 
    
    }
    
    if (((LPC_TIM2->IR) & 0x00000002 ) == 0x00000002 ) {    // TIM2 MR1 interrupt service routine !! 
        
        LPC_TIM2->IR |= 0x00000002;     // clear MR1 interrupt register by setting 
    
    }
}

void tim2_MAT_Init() {
 // default peripheral clock period = 24MHz
    const float pclkperiod = 0.041667;  // in us
 // Disable all PLL
    LPC_SC->PLL0CON = 0;
    LPC_SC->PLL1CON = 0;
 // Power on Timer2
    LPC_SC->PCONP |= (1 << 22);

 // set up Peripheral clock, default divide by 4
 // LPC_SC->PCLKSEL1 |= (1 << 12);  // Divide CCLK by 1 for Timer2

 // Select Timer pin
 //   LPC_PINCON->PINSEL0  |= (1 << 12) | (1 << 13);   // set P0.6 to MAT2.0
 //   LPC_PINCON->PINSEL0  |= (1 << 14) | (1 << 15);   // set P0.7 to MAT2.1
 //   LPC_PINCON->PINSEL0  |= (1 << 16) | (1 << 17);   // set P0.8 to MAT2.2
    LPC_PINCON->PINSEL4  &= ~(1 << 24);   // set P4.28 to MAT2.0
    LPC_PINCON->PINSEL4  |= (1 << 25);    
    LPC_PINCON->PINSEL4  &= ~(1 << 26);   // set P4.29 to MAT2.1
    LPC_PINCON->PINSEL4  |= (1 << 27);   
    
// set P0.6, P0.7, P0.8, P4.28, P4.29 to have neither pull-up nor pull-down resistors, set bit 13, clear bit 12 etc.
 //    LPC_PINCON->PINMODE0 |= (1 << 13);      // P0.6
 //   LPC_PINCON->PINMODE0 &= ~(1 << 12);
 //   LPC_PINCON->PINMODE0 |= (1 << 15);      // P0.7
 //   LPC_PINCON->PINMODE0 &= ~(1 << 14);
 //   LPC_PINCON->PINMODE0 |= (1 << 17);      // P0.8
 //   LPC_PINCON->PINMODE0 &= ~(1 << 16);
    
    LPC_PINCON->PINMODE9 |= (1 << 25);      // P4.28
    LPC_PINCON->PINMODE9 &= ~(1 << 24);
    LPC_PINCON->PINMODE9 |= (1 << 27);      // P4.29
    LPC_PINCON->PINMODE9 &= ~(1 << 26);

 // Set up Vector for Timer2 Interrupt Routine & enable Timer2 interrupts 
    NVIC_SetVector(TIMER2_IRQn, uint32_t(TIMER2_IRQHandler));
    NVIC_EnableIRQ(TIMER2_IRQn);
    
 // Configure Timer2 registers 
    // LPC_TIM2->TC = 0;                   // clear timer counter
    // LPC_TIM2->PC = 0;                   // clear prescale counter
    // LPC_TIM2->PR = 0;                   // clear prescale register
    LPC_TIM2->IR |= 0xFFFFFFFF;          // clear interrupt register by setting
    LPC_TIM2->TCR |= (1 << 1);          // reset timer
    LPC_TIM2->TCR &= ~0xFFFFFFFF;       // release reset
    LPC_TIM2->MR0 = 6000000;           // 24MHz x 6000000 = 200ms for Serial communication of ECU 
    LPC_TIM2->MR1 = 240000;          // 24MHz x 240000 = 10ms for MPU6050 (Gyro & accel)
    LPC_TIM2->MCR |= 0x00000019;       // interrupt on MR0 and MR1, reset TC on MR1 
    
    LPC_TIM2->TCR |= (1 << 0);          // start Timer2 counting
    pc.printf("Tim2 MATCH Interrupt Setup complete !!\n\r");
}


int main() {    

    pc.baud(115200);
    dataLogger.baud(115200);
    
    while(1) {
        
        if (pc.readable()) {
            readnum = 1;
            dataLogger.count = 0;
            DLcommand = pc.getc();
            pc.printf("\n\r%c\n\r",DLcommand);
            dataLogger.putc(DLcommand);
             
            dataLogger.get_ECU_databyte();
        }
        dataLogger.display_ECU_databyte();     
    }
}
