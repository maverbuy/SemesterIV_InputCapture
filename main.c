//**********************************************************************
//main.c
//
//Embedded Systems Software - Lab #8
//Modified by: Mitch Verbuyst
//						  April 5, 2018
//
//
//This lab configures TIM4 for input capture and TIM1 (channel 2) as 
//output for the operation of an ultrasonic sensor.
//
//PB6 = Echo
//PE11 = Trigger
//
//In addition to basic input capture, the ultrasonic sensor can be operated
//in order to read the distance to an object.  This distance is available
//to be displayed on the LCD screen.
//
//**********************************************************************

//Libraries
#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "TimerConfig.h"
#include <stdint.h>
#include "Timer.h"
#include "LCD.h"

//Protoypes
extern void TimerConfig(void);
void TIM4_IRQHandler(void);
void PortB(void);
void Clock_Init();
void PortE(void);

//Constants
volatile uint32_t pulse_width = 0;
volatile uint32_t last_captured = 0;
volatile uint32_t signal_polarity = 0;
#define AFR0_MASK 0xF0FFFFFF
#define AFR0_VAL 0x02000000
#define AFR1_MASK 0xFFFFFFFF
#define AFR1_VAL 0x00000000

int main(void){

	//Set clock to MSI 16 MHz
	Clock_Init();
	
	//Enable GPIO B clock and configure GPIO port B pin 6 as
	//AF function 2, with no pull-up/pull-down
	PortB();
	
	//Enable GPIO E clock and configure PE11 as
	//AF function 1, with no pull-up/pull-down
	PortE();
	
	//Initialize the LCD for display
	LCD_Initialization();
	LCD_Clear();
	
	//Configure timer 4 channel 1
	Timer4();
	
	//Configure timer 1 channel 2
	Time1Config();
	
	while(1);
	
}


//******************************************************************************************
//Configure GPIO B, pin #6
//******************************************************************************************
void PortB(void)
{
	//enable GPIO B clock
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOBEN;
	
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOB->MODER = ~(3U<<(2*6));  
	GPIOB->MODER |= 2U<<(2*6);      //  Alternative
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOB->PUPDR   &= ~(3U<<(2*6));  // No pull-up, no pull-down
	
	//set pin 6 as alternative function 2 (0010)
	GPIOB->AFR[1] &= AFR1_MASK;
	GPIOB->AFR[1] |= AFR1_VAL;
	
	GPIOB->AFR[0] &= AFR0_MASK;
	GPIOB->AFR[0] |= AFR0_VAL;
	
}

//******************************************************************************************
//Configure GPIO E, pin #11
//******************************************************************************************
void PortE(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOE->MODER = ~(3U<<(2*11));  
	GPIOE->MODER |= 2U<<(2*11);      //  Alternative
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOE->PUPDR   &= ~(3U<<(2*11));  // No pull-up, no pull-down
	
	//set pin 11 as alternative function 1 (0001)
	GPIOE->AFR[0] &= 0xFFFFFFFF;
	GPIOE->AFR[0] |= 0x00000000;
	
	GPIOE->AFR[1] &= 0xFFFF0FFF;
	GPIOE->AFR[1] |= 0x00001000;
}

//******************************************************************************************
//Interrupt handler for Timer 4
//******************************************************************************************
void TIM4_IRQHandler(void)
{
	
	uint32_t current_captured;
	
	if((TIM4->SR & TIM_SR_CC1IF) != 0 ) { //check intterupt flag is set
		
		//reading CCR1 clears CC1IF interrupt flag
		current_captured = TIM4->CCR1;
		
		//toggle the polarity flag
		signal_polarity = 1 - signal_polarity;
		
		if(signal_polarity == 0){ //calculate only when the current input is low
			
			pulse_width = current_captured - last_captured; //assume up-counting
			
			
		}
		
		last_captured = current_captured;
		
	}
	
	if((TIM4->SR & TIM_SR_UIF) != 0){ //check if overflow has taken place
		
		TIM4->SR &= ~TIM_SR_UIF;
		
	
	}
	
	
	
	LCD_DisplayString((uint8_t*)pulse_width);

}



//******************************************************************************************
//Set up the MSI clock for 16 MHz
//******************************************************************************************
void Clock_Init()
{

	//set MSI as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	
	//Wait for MSIRDY to be set
	while((RCC->CR & RCC_CR_MSIRDY) == 0);
	
	//Set MSIRGSEL to 1 so that MSIRANGE bits in RCC_CR are used
	RCC->CR &= ~RCC_CR_MSIRGSEL;
	RCC->CR |= RCC_CR_MSIRGSEL;
	
	//Set MSIRANGE to 16 MHz
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_8;
	
	//Wait for MSIRDY to be set
	while((RCC->CR & RCC_CR_MSIRDY) == 0);
	
}