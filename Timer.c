//**********************************************************************
//Timer.c
//
//Embedded Systems Software - Lab #8
//Modified by: Mitch Verbuyst
//						  March 20, 2018
//
//A .c file that configures the timers required for this lab.
//
//
//**********************************************************************


//Libraries
#include <stdint.h>
#include "stm32l476xx.h"


//**************************************************************************
//Configure Timer 4
//*************************************************************************
void Timer4(void)
{
	//enable the clock to timer 4
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	//set up an appropriate prescaler to slow down the timers input clock
	TIM4->PSC = 127;
	
	//set auto reload to max value (16 bit value)
	TIM4->ARR = 0xFFFF;
	
	//set direction of channel 1 as input, and select the active input
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S; //clear capture/compare 1 selection bits
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0; //CC1S {1:0} for channel 1
	//00 = output
	//01 = input, CC1 is mapped on timer 1 input
	//10 = input, CC1 is mapped on timer 2 input
	//11 = input, CC1 is mapped on slave timer
	
	//Program input filter duration
	TIM4->CCMR1 &= ~TIM_CCMR1_IC1F; //no filtering
	
	//set active transition as both rising and falling edges
	TIM4->CCER |= TIM_CCER_CC1P|TIM_CCER_CC1NP;
	
	//Program the input prescaler - clear prescaler to capture each transition
	TIM4->CCMR1 &= ~(TIM_CCMR1_IC1PSC);
	
	//Enable capture for channel 1
	//CC1E 0 = disabled, 1 = enabled
	TIM4->CCER |= TIM_CCER_CC1E;
	
	//Allow channel 1 of timer 4 to generate interrupts
	TIM4->DIER |= TIM_DIER_CC1IE;
	
	//Allow channel 1 of timer 4 to generate DMA requests
	TIM4->DIER |= TIM_DIER_CC1DE;
	
	//Enable the timer counter
	TIM4->CR1 |= TIM_CR1_CEN;
	
	//set priority of timer 4 interrupt to 0 (highest)
	NVIC_SetPriority(TIM4_IRQn, 0);
	
	//enable timer 4 interrupt in the interrupt controller (NVIC)
	NVIC_EnableIRQ(TIM4_IRQn);
	
}


//**************************************************************************
//Configure Timer 1, channel 2
//*************************************************************************
void Time1Config(void)
{	
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	//Counting Direction up
	TIM1->CR1 &= ~TIM_CR1_DIR;
	
	//Prescaler
	TIM1->PSC = 159;
	
	//Auto Reload
	TIM1->ARR = 0xFFF;
	
	//Clear output compare mode bits for channel 1
	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M;
	
	//Select PWM mode 1 output on channel 1
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	
	//Output 1 preload enable
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;
	
	//Select output polarity
	TIM1->CCER &= ~TIM_CCER_CC2NP;
	
	//output enable
	TIM1->CCER |= TIM_CCER_CC2E;
	
	//enable complementary output of channel 1
	TIM1->CCER |= TIM_CCER_CC2NE;
	
	//Main output enable (MOE)
	TIM1->BDTR |= TIM_BDTR_MOE;
	
	//Output compare register for channel 1
	TIM1->CCR2 = 13;
	
	//Enable Counter
	TIM1->CR1 |= TIM_CR1_CEN;

	
}
