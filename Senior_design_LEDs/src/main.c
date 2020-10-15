/**
  ******************************************************************************
  * @file    main.c

  * @author  Christine Moravek
  * @version V1.0
  * @date    01-December-2013
  * @brief   Turn on LEDs
  ******************************************************************************
*/

void setup_GPIO_LED();
void test_leds();
void turn_on_led(int number);
void micro_wait(unsigned int);


#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
			

int main(void)
{
    setup_GPIO_LED(); //sets up GPIOC
    for(int i = 0; i < 100; i++) {
        turn_on_led(i % 10); //turns on pc0 - 9 in order
        micro_wait(100000);
    }
}

//Turns off other led's and turns on led corresponding to segment of tank fish is in.
void turn_on_led(int number) {
    GPIOC->ODR &= ~0x3ff; //turns off all LEDs
    GPIOC->ODR |= 1 << number; //turns on LED based on the number
}

//Enables clock to GPIOC and sets pins 0-9 to output for LEDS
void setup_GPIO_LED() {
    //enable clock to GPIOC
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~0xfffff; //clear moder for pc0 - 9
    GPIOC->MODER |= 0x55555; //sets pc0-9 to output
    GPIOC->OTYPER &= ~0xfffff; //makes sure in push/pull config
}

//should turn on all leds
void test_leds() {
    GPIOC->ODR |= 0x3ff; //turn on leds
}
