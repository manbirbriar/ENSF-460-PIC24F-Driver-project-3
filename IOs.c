      #include "TimeDelay.h"
#include "IOs.h"

uint16_t PB3_event;
uint16_t PB2_event;
uint16_t PB1_event;



void IOinit(){
    AD1PCFG = 0xFFFF; /* keep this line as it sets I/O pins that can also be analog to be digital */
    
    newClk(500);
    
    // Configure RB8 as an Output and Initialize It Low
    TRISBbits.TRISB8 = 0;   // Set RB8 as an output
    LATBbits.LATB8 = 0;     // Initialize RB8 to low (0)
    
    // Configure RA4 as an Input with Pull-Up and Change Notification
    TRISAbits.TRISA4 = 1;   // Set RA4 as an input
    CNPU1bits.CN0PUE = 1;   // Enable pull-up resistor on CN0 (RA4)
    CNEN1bits.CN0IE = 1;    // Enable Change Notification Interrupt on CN0
    
     // Configure RB4 as an Input with Pull-Up and Change Notification
    TRISBbits.TRISB4 = 1;   // Set RB4 as an input
    CNPU1bits.CN1PUE = 1;   // Enable pull-up resistor on CN1 (RB4)
    CNEN1bits.CN1IE = 1;    // Enable Change Notification Interrupt on CN1

    // Configure RA2 as an Input with Pull-Up and Change Notification
    TRISAbits.TRISA2 = 1;   // Set RA2 as an input
    CNPU2bits.CN30PUE = 1;  // Enable pull-up resistor on CN30 (RA2)
    CNEN2bits.CN30IE = 1;   // Enable Change Notification Interrupt on CN30
    
    // Clear Event Flags and Reset Variables
    PB3_event = 0;
    PB2_event = 0;
    PB1_event = 0;
    
    // Configure Interrupt Priorities and Enable Interrupts
    IPC4bits.CNIP = 6;  // Set Change Notification Interrupt Priority to 6
    IFS1bits.CNIF = 0;  // Clear the Change Notification Interrupt Flag
    IEC1bits.CNIE = 1;  // Enable the Change Notification Interrupt
}

void IOcheck(){
    Disp2String("---------------");
    //delay_ms(500); // delay for multiple button press
    
    // All PBs
    if (PB1_event && PB2_event && PB3_event){
        Disp2String("All PBs pressed");     //display message once
        while (PB1_event && PB2_event && PB3_event){    //only continue while PBs pressed 
            LATBbits.LATB8 = 1;     //turn light on             
        }
        LATBbits.LATB8 = 0;     //turn light off 
        PB1_event = 0;          //Reset PB events to 0  
        PB2_event = 0;
        PB3_event = 0;
        
    }
    // PB1 and PB2 pressed
    else if (PB1_event && PB2_event && !PB3_event){
        Disp2String("PB1 and PB2 are pressed");     //display message once
        while (PB1_event && PB2_event && !PB3_event){   //only continue while PBs pressed 
            LATBbits.LATB8 = 1;     //turn light on 
        }
        PB1_event = 0;
        PB2_event = 0;
    }
    // PB1 and PB3 pressed
    else if (PB1_event && PB3_event && !PB2_event){
        Disp2String("PB1 and PB3 are pressed");     //display message once
        while (PB1_event && PB3_event && !PB2_event){   //only continue while PBs pressed 
            LATBbits.LATB8 = 1;     //turn light on 
        }
        LATBbits.LATB8 = 0;     //turn light off 
        PB1_event = 0;          //Reset PB events to 0
        PB3_event = 0;
    }
    // PB3 and PB3 pressed
    else if (PB2_event && PB3_event && !PB1_event){
        Disp2String("PB2 and PB3 are pressed");     //display message once
        while (PB2_event && PB3_event && !PB1_event){   //only continue while PBs pressed 
            LATBbits.LATB8 = 1;
        }
        LATBbits.LATB8 = 0;     //turn light off 
        PB2_event = 0;          //Reset PB events to 0
        PB3_event = 0;
    }
    // PB1 pressed
    else if (PB1_event && !PB2_event && !PB3_event) {   
        LATBbits.LATB8 = 1;
        Disp2String("PB1 event");     //display message once
        while (PB1_event && !PB2_event && !PB3_event){    //only continue while PB pressed 
            delay_ms(500); // 0.5 second delay
            LATBbits.LATB8 = ~LATBbits.LATB8; //toggle light on or off for flashing
        }
    }
    // PB2 pressed
    else if (PB2_event && !PB1_event && !PB3_event) {
        LATBbits.LATB8 = 1;
        Disp2String("PB2 event");     //display message once
        while (PB2_event && !PB1_event && !PB3_event){    //only continue while PB pressed 
            delay_ms(1000); // 1 second delay
            LATBbits.LATB8 = ~LATBbits.LATB8; //toggle light on or off for flashing
        }
    }
    // PB3 pressed
    else if (PB3_event && !PB2_event && !PB1_event) {
        LATBbits.LATB8 = 1;
        Disp2String("PB3 event");     //display message once
        while (PB3_event && !PB2_event && !PB1_event){    //only continue while PB pressed 
            delay_ms(4000);// 4 second delay
            LATBbits.LATB8 = ~LATBbits.LATB8; //toggle light on or off for flashing
        }
    }
    
    // Nothing pressed
    else{
        LATBbits.LATB8 = 0; //light off
        Disp2String("Nothing pressed"); //display message once
    }
    
        
    
}


void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    //Don't forget to clear the CN interrupt flag!
    IFS1bits.CNIF = 0;
    
    // Check the state of each button
    if (PORTAbits.RA4 == 0) { // PB1 is active low (pressed)
        PB1_event = 1;
    }
    if (PORTAbits.RA4 == 1) { // PB1 is not pressed
        PB1_event = 0;
    }
    
    if (PORTBbits.RB4 == 0) { // PB2 is active low (pressed)
        PB2_event = 1;
    }
    if (PORTBbits.RB4 == 1) { // PB2 is not pressed
        PB2_event = 0;
    }
    
    if (PORTAbits.RA2 == 0) { // PB3 is active low (pressed)
        PB3_event = 1;
    }
    if (PORTAbits.RA2 == 1) { // PB3 is not pressed
        PB3_event = 0;
    }
}


