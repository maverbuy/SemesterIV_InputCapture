//**********************************************************************//
//LED.c
//
//Embedded Systems Software - Lab #8
//Modified by: Mitch Verbuyst
//						  March 20, 2018
//
//
//A .c file that configures the LED for lab #8 (if needed).
//
//**********************************************************************//




//Libraries
#include "LED.h"

//Constants
#define AFR0_MASK 0xFFFFFFFF
#define AFR0 0x00000000
#define AFR1_MASK 0xFFFFFFF0
#define AFR1 0x00000001


//******************************************************************************************
// User LEDs: 
//   LD4 Red = PB2    LD5 Green = PE8
// Note: The Green LED is yellow on my board.
//       PE8 is also the TIM1_CH1N for ADC Triggers.
//******************************************************************************************
void LED_Init(void){
	/* Enable GPIOs clock */ 	
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOEEN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// LD4 Red = PB2
	///////////////////////////////////////////////////////////////////////////////////////////////
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOB->MODER = ~(3U<<(2*2));  
	GPIOB->MODER |= 2U<<(2*2);      //  Alternative
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOB->OSPEEDR &= ~(3U<<(2*2));
	GPIOB->OSPEEDR |=   3U<<(2*2);  // High speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOB->OTYPER &= ~(1U<<2);       // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOB->PUPDR   &= ~(3U<<(2*2));  // No pull-up, no pull-down
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// LD5 Green = PE8
	///////////////////////////////////////////////////////////////////////////////////////////////
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOE->MODER = ~(3U<<(2*8));  
	GPIOE->MODER |= 2U<<(2*8);      //  Alternative (10)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOE->OSPEEDR &= ~(3U<<(2*8));
	GPIOE->OSPEEDR |=   3U<<(2*8);  // High speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOE->OTYPER &= ~(1U<<8);       // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOE->PUPDR   &= ~(3U<<(2*8));  // No pull-up, no pull-down
	
	
	//AFR
	
	GPIOE->AFR[0] &= AFR0_MASK;
	GPIOE->AFR[0] |= AFR0;
	
	GPIOE->AFR[1] &= AFR1_MASK;
	GPIOE->AFR[1] |= AFR1;
	
	
	
	
}

//******************************************************************************************
// Turn Red LED On
//******************************************************************************************
void Red_LED_On(void){
	GPIOB->ODR |= GPIO_ODR_ODR_2;
}

//******************************************************************************************
// Turn Red LED Off
//******************************************************************************************
void Red_LED_Off(void){
	GPIOB->ODR &= ~GPIO_ODR_ODR_2;
}

//******************************************************************************************
// Toggle Red LED 
//******************************************************************************************
void Red_LED_Toggle(void){
	GPIOB->ODR ^= GPIO_ODR_ODR_2;
}

//******************************************************************************************
// Turn Green LED On
//******************************************************************************************
void Green_LED_On(void){
	GPIOE->ODR |= GPIO_ODR_ODR_8;
}

//******************************************************************************************
// Turn Green LED Off
//******************************************************************************************
void Green_LED_Off(void){
	GPIOE->ODR &= ~GPIO_ODR_ODR_8;
}

//******************************************************************************************
// Toggle Green LED
//******************************************************************************************
void Green_LED_Toggle(void){
	GPIOE->ODR ^= GPIO_ODR_ODR_8;
}