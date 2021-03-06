// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 8, 2014

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

// Global Variables //eja
unsigned long in;
#include "TExaS.h"

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

void Delay100ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 1333333;  // this number means 100ms     
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms
  }
}
// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

int main(void){ unsigned long volatile delay;
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
  // initialization goes here
	SYSCTL_RCGC2_R = SYSCTL_RCGC2_R | SYSCTL_RCGC2_GPIOF; // eja :::: Turn on the clock for PortF.
	delay = SYSCTL_RCGC2_R; // eja :::: give clock time to stablize.
	GPIO_PORTF_AMSEL_R = GPIO_PORTF_AMSEL_R & ~0x14; // eja :::: clear bits 4(switch 1) and 2(blue led) in the AMSEL registor. 
	GPIO_PORTF_PCTL_R = GPIO_PORTF_PCTL_R & ~0x000F0F00; // eja :::: clear pins PF4 and PF2 in the P control registor to configure as GPIO.
	GPIO_PORTF_DIR_R = 0x0E; // eja :::: set bits 4 and 0 and clear bits 3, 2, and 1 in the direction registor.
	GPIO_PORTF_AFSEL_R = 0x0000; // eja :::: clear all bits in the AFSEL registor.
	GPIO_PORTF_DEN_R = 0x1F; // eja :::: enable digital for pins PF4, PF3, PF2, PF1, PF0. 
	GPIO_PORTF_PUR_R = 0x11; //eja :::: enable pullup resistors for PF4, PF3, PF2, PF1, PF0.
	GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R | 0x04; // eja :::: set bit 2 in the data registor to turn led on.

  EnableInterrupts();           // enable interrupts for the grader
  while(1){
    // body goes here
		Delay100ms(1); //eja :::: waits 100ms.
		in = GPIO_PORTF_DATA_R&0x10; //eja :::: reads value of PF4 into in.
		if (in == 0) { //eja :::: conditional if statement testing if the value of in is 0.
			GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R ^ 0x04; //eja :::: toggle bit 2(blue led)
		} //eja :::: ending of if statement and test.
		if (in == 1) { //eja :::: conditional if statement testing if the value of in is 1.
			GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R | 0x04; // eja :::: set bit 2 so as to turn led on.
		} //eja :::: end of if statement and test.
  }
}
