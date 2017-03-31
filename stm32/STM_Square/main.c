#include "stm32f05xxx.h"

void ToggleLED(void);
volatile int Count = 0;
int delay_time = 0;

// Interrupt service routines are the same as normal
// subroutines (or C funtions) in Cortex-M microcontrollers.
// The following should happen at a rate of 1kHz.
// The following function is associated with the TIM1 interrupt 
// via the interrupt vector table defined in startup.s
void delay(int dly){

	while(dly--);

}
/*
* Each function sends a signal in the form of 4 bits to the
* robot. Before signal is sent, the main square wave is disabled.
* Main square wave re-enabled when signal is finished sending
*	Generates 4 bits in series at pin 6
*	GPIOA_ODR ^= BIT0; = compliment digital output
*/

/*
* Sends 0000
*/
void stop(void){
	//disable timer
	TIM1_DIER &= ~BIT0;    // enable update event (reload event) interrupt  
	//GPIOA_ODR &= ~BIT8; // Toggle PA8
}

/*
* Sends 0001
*/
void go(void){
//disable timer

	TIM1_DIER |= BIT0;
//wait x time
//re-enable timer

}

/*
* Sends 0010
*/
void reverse(void){
	TIM1_DIER &= ~BIT0;    // enable update event (reload event) interrupt  
	GPIOA_ODR &= ~BIT8; // Toggle PA8
	delay(10000000);
	TIM1_DIER |= BIT0;
}

/*
* Sends 0011
*/
void turnleft(void){
	TIM1_DIER &= ~BIT0;    // enable update event (reload event) interrupt  
	GPIOA_ODR &= ~BIT8; // Toggle PA8
	delay(416667);
	TIM1_DIER |= BIT0;
}

/*
* Sends 0100
*/
void turnright(void){
	TIM1_DIER &= ~BIT0;    // enable update event (reload event) interrupt  
	GPIOA_ODR &= ~BIT8; // Toggle PA8
	delay(4000000);
	TIM1_DIER |= BIT0;    // enable update event (reload event) interrupt  
	

}

/*
* Sends 0101
*/
void turnaround(void){
	TIM1_DIER &= ~BIT0;    // enable update event (reload event) interrupt  
	GPIOA_ODR &= ~BIT8; // Toggle PA8
	delay(1000000);
	TIM1_DIER |= BIT0;

}

void Timer1ISR(void) 
{
	TIM1_SR &= ~BIT0; // clear update interrupt flag
	Count++;
	if (Count > 9)
	{ 
		Count = 0;
		ToggleLED(); // toggle the state of the LED every second
	}  
	if (GPIOA_IDR &0b1){
		delay(50000); 
		if (GPIOA_IDR &0b1){
			stop();
		}
	}
	//Button 2 START
	if (GPIOA_IDR &0b100){
		delay(50000); 
		if (GPIOA_IDR &0b100){
			go();
		}
	}
	
	//Button 3 REVERSE
	if (GPIOA_IDR &0b1000){
		delay(50000); 
		if (GPIOA_IDR &0b1000){
			reverse();
		}
	}
	

	
	//Button 5 RIGHT
	if (GPIOA_IDR &0b10000000){
		delay(50000); 
		if (GPIOA_IDR &0b10000000){
			turnright();
		}
	}
	
	//Button 6 TURNAROUND
	if (GPIOA_IDR &0b1000000){
		delay(50000); 
		if (GPIOA_IDR &0b1000000){
			turnaround();
		}
	}
	
	
}

void SysInit(void)
{
	// Set up output port bit for blinking LED
	RCC_AHBENR |= 0x00020000;  // peripheral clock enable for port A
	GPIOA_MODER |= 0x00000001; // Make pin PA0 output
	GPIOA_MODER |= BIT16;
	GPIOA_MODER |= BIT4;
	GPIOA_MODER |= BIT6;
	GPIOA_MODER |= BIT10;
	GPIOA_MODER |= BIT14;
	
	// Set up timer
	RCC_APB2ENR |= BIT11; // turn on clock for timer1
	TIM1_ARR = 32;      // reload counter with 8000 at each overflow (equiv to 1ms)
	ISER |= BIT13;        // enable timer interrupts in the NVIC
	TIM1_CR1 |= BIT4;     // Downcounting    
	TIM1_CR1 |= BIT0;     // enable counting    
	TIM1_DIER |= BIT0;    // enable update event (reload event) interrupt  
	enable_interrupts();
}

void ToggleLED(void) 
{    
	GPIOA_ODR ^= BIT8; // Toggle PA8
}

int main(void)
{
	SysInit();
	
	//delay = 0;
/*
*For 7.435kHz
* Count    -> 9
* TIM1_ARR -> 32
*
*
*
* Outputs a square wave at 7.135kHz on pin 6
*
*	6 buttons for 6 commands
*	on button push - disable square wave
*	output 0 for n time period
*	then set output for n time * 4 bits
*	eg 1 for x sec, 0 for 3x sec
*	-> 4bit signal 0b1000
*
*	each button gets an if()
*		set timer parameters/call routine 
*
*	TODO set up a second timer for the signal routine
*	OR use delay funk
*	delay(int); ->test how long 1 int gets
	for(i=0;i<4;i++){
		set bit 
		delay n time
	}
*
*/
while(1){
	
	//Button 4 LEFT
	if (GPIOA_IDR &0x20){
		delay(50000); 
		while (GPIOA_IDR &0x20);
		turnleft();
		
	}

}
/*
	while(1)
	{    
		
		if(1){
		//0000
			stop();
		}
		if(1){
		//0001
			go();
		}
		if(1){
		//0010
			reverse();
		}
		if(1){
		//0011
			turnleft();
		}
		if(1){
		//0100
			turnright();
		}
		if(1){
		//0101
			turnaround();
		}
		if(1){
		//0110
		
		//add'l functionality
		}
	

		
	}*/
	return 0;
}
