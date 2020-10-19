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
void button_setup_gpio();
void setup_pwm();
void update_flashing_button();
void update_button_history();
int8_t get_history(int i);
void setup_Timer6();
void turn_on_display();
void turn_off_display();
void turn_on_light_display();
void turn_off_light_display();
void turn_on_speaker();
void turn_off_speaker();
void setup_display();


#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

//variables
int8_t history[3] = {0};
int alarm_state_on; //button 1
int time_since_fish_fed_seconds;
int display_on;
int lights_on;
int sound_on;
int water_temp;
int water_ph;

int main(void)
{
    setup_GPIO_LED(); //sets up GPIOC
    button_setup_gpio();
    setup_pwm();
    setup_Timer6();
    NVIC_SetPriority(TIM6_DAC_IRQn, 1); // alarm state gets priority

    for(int i = 0; i < 100; i++) {
        turn_on_led(i % 10); //turns on pc0 - 9 in order
        micro_wait(100000);
    }
}

//christine's functions

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

void turn_on_light_display()
{
	//needed function
}

void turn_off_light_display()
{
	//needed function
}

void turn_on_speaker()
{
	//needed function
}

void turn_off_speaker()
{
	//needed function
}

//marlee's functions

//button implementation
void button_setup_gpio()
{
    // going to use pa0-3
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//set them as inputs //00
	GPIOA->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER2 | GPIO_MODER_MODER1| GPIO_MODER_MODER0);
	//going to use pa0 as the alarm button - need to enable pulse width modulation
	//tim 1 ch 1 for pwm = pa 8
    GPIOA->MODER |= (2 << (2*8));
    //formerly 0x222
    GPIOA->AFR[1] |= 0x2;
}

//using timer 1 for pwm
void setup_pwm()
{
	//just need ch1 set up
    RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;
    //upcounting
    TIM1->CR1 &= ~TIM_CR1_DIR;
    //enable channels
    TIM1->CCER |= TIM_CCER_CC1E;
    //110 is pulse width modulation
    //channel 1
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
    //arr and psc
    TIM1 -> ARR = 99; //100
    TIM1 -> PSC = 479; //480
    TIM1->BDTR |= TIM_BDTR_MOE;
    TIM1->CR1 |= TIM_CR1_CEN;
}

int brightness = 0;
int direction = 0; // 0 is up

void update_flashing_button()
{
    // Update Brightness Value to have it flash
    if (brightness < 100 && direction == 0) {
      brightness = brightness + 10;
    }
    if (brightness < 100 && direction == 1) {
      brightness = brightness -10;
    }
    if(brightness == 100) {
       direction = 1;
       brightness = 90;
    }
    if(brightness == 0) {
        direction = 0;
        brightness = 10;
    }

    // Update Brightness of Button
    if(alarm_state_on == 1) {
        TIM1 -> CCR1 = brightness;
    }
    else {
        TIM1 -> CCR1 = 0;
    }
}

void update_button_history() {
    for(int i = 0; i < 4; i++) {
        history[i] = (history[i] << 1);
        history[i] |= (((GPIOA->IDR >> i) & 0x1) == 0);
    }
}

int8_t get_history(int i) {
    return history[i];
}

//use timer 6 for updating the display + getting button inputs -- once a second
void setup_Timer6(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    TIM6->PSC = 48000 - 1;      // 48000000/48000 = 1000 hz
    TIM6->ARR = 100 - 1;        // 0.001s * 10 = 0.1s

    TIM6->DIER |= TIM_DIER_UIE;
    TIM6->CR1 |= TIM_CR1_CEN;

    NVIC->ISER[0] |= 1<<TIM6_DAC_IRQn;
}

void TIM6_DAC_IRQHandler(void) {
    TIM6->SR &= ~TIM_SR_UIF;

    //check all thresholds to see if alarm state should be on
    //water shoudl be 75-82 degrees
    water_temp = get_temperature();
    if((water_temp < 75) | (water_temp > 82))
    {
    	alarm_state_on = 1;
    }
    //ph should be from 6.5 to 7.5
    water_ph = get_ph();
    if((water_ph < 6.5)|(water_ph > 7.5))
    {
    	alarm_state_on = 1;
    }
    //make sure the fish has been fed in the past 24 hours
    if(time_since_fish_fed_seconds > 86400)
    {
    	alarm_state_on = 1;
    }

    // Update button brightness every cycle if alarm is on
    update_button_history();
    if(alarm_state_on == 1)
    {
    	update_button_brightness();
    	turn_on_speaker();
    }

    // take in button inputs
    // see if any were pressed
    //button 0 is feed_fish + alarm state
    if(get_history(0) == 1)
	{
    	if(alarm_state_on == 1)
    	{
    		alarm_state_on = 0;
    	}
    	else
		{
    		time_since_fish_fed_seconds = 0;
		}
	}
    if(get_history(1) == 1)
    {
    	if(display_on == 1)
    	    {
    	    	turn_off_display();
    	    }
    	else
    		{
    	    	turn_on_display();
    		}
    }
    if(get_history(2) == 1)
    {
    	if(lights_on == 1)
    	{
    		turn_off_light_display();
    	}
    	else
    	{
    		turn_on_light_display();
    	}
    }
    if(get_history(3) == 1)
    {
    	if(sound_on == 1)
    	{
    		turn_off_speaker();
    	}
    	else
    	{
    		turn_on_speaker();
    	}
    }

    time_since_fish_fed_seconds++;

    //writing to the display will also happen in the interrupt

}

void setup_display()
{
	//placeholder function
}

void turn_off_display()
{
	//placeholder function
}

void turn_on_display()
{
	// placeholder function
}

void get_temperature()
{
	//placeholder function
}

void get_ph()
{
	//placeholder functions
}










