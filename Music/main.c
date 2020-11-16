
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdlib.h>

#define RATE 100000
#define N 1000
extern const short int wavetable[N];

// This function
// 1) enables clock to port A,
// 2) sets PA0, PA1, PA2 and PA4 to analog mode
void setup_gpio() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //enables clock to port A
    GPIOA->MODER |= 0x33f;             //sets pa0,1,2,4 to analog (mode 11)
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

}

// This function should enable the clock to the
// onboard DAC, enable trigger,
// setup software trigger and finally enable the DAC.
void setup_dac() {
    RCC->APB1ENR |= RCC_APB1ENR_DACEN; //enable clock to onboard DAC
    DAC->CR &= ~DAC_CR_EN1;  //Disable DAC channel 1
    DAC->CR &= ~DAC_CR_BOFF1; //Do not turn buffer off
    DAC->CR |= DAC_CR_TEN1; //Enable trigger
    DAC->CR |= DAC_CR_TSEL1; //select software trigger
    DAC->CR |= DAC_CR_EN1;   //enable DAC channel 1

}


// This function should,
// enable clock to timer6,
// setup pre scalar and arr so that the interrupt is triggered every
// 10us, enable the timer 6 interrupt, and start the timer.
void setup_timer6() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; //enable clock to timer 6

    //set up PSC and ARR for once per 10us
    TIM6->PSC = 48-1;
    TIM6->ARR = 10-1;
    TIM6->DIER |= 1;
    TIM6->CR1 |= TIM_CR1_CEN; //start timer

    NVIC->ISER[0] = 1<<(TIM6_DAC_IRQn); //enable timer 6 interrupt
}


// This function should enable the clock to ADC,
// turn on the clocks, wait for ADC to be ready.
void setup_adc() {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //enable clock for ADC
    RCC->CR2 |= RCC_CR2_HSI14ON;  //turn on Hi-speed internal 14 MHz clokc
    while(!(ADC1->ISR & ADC_ISR_ADRDY)); //wait for 14MHz clock to be ready
    ADC1->CR |= ADC_CR_ADEN; //Enable ADC
    while(!(ADC1->ISR & ADC_ISR_ADRDY)); //Wait for ADC to be ready
    while((ADC1->CR & ADC_CR_ADSTART)); // wait for ADCstart to be 0
}

// This function should return the value from the ADC
// conversion of the channel specified by the â€œchannelâ€ variable.
// Make sure to set the right bit in channel selection
// register and do not forget to start adc conversion.
int read_adc_channel(unsigned int channel) {
    ADC1->CHSELR = 0; //unselect all ADC Channels
    ADC1->CHSELR |= (1<<channel); //select given channel
    while(!(ADC1->ISR & ADC_ISR_ADRDY)); //wait for ADC ready
    ADC1->CR |= ADC_CR_ADSTART;  //start ADC
    while(!(ADC1->ISR & ADC_ISR_EOC));  //wait for conversion to end
    return ADC1->DR * 3 / 4095;
}


/*int offset = 0;
//COMMENT FOR 6.4
//start DAC conversion w/ software trigger, value to be converted already stored
//in holding register
void TIM6_DAC_IRQHandler() {
    while((DAC->SWTRIGR & DAC_SWTRIGR_SWTRIG1) == DAC_SWTRIGR_SWTRIG1);
    DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1; //trigger conversion


    //calculate offset
    int step = 554.365 * N / RATE * (1<<16); //C+
    offset += step;
    if((offset>>16) >= N) //went past end of array
        offset -= N<<16;  //wrap around with same offset as overshoot
    int sample = wavetable[offset>>16]; //get the sample
    sample = sample / 16 + 2048; //adjust for DAC range
    DAC->DHR12R1 = sample; //write sample to holding register
    TIM6->SR &= ~1; //acknowledge interrupt
}*/


int offset1 = 0;
int offset2 = 0;
int step1 = 554.365 * N / RATE * (1<<16); //C+
int step2 = 698.456 * N / RATE * (1<<16); //F
void TIM6_DAC_IRQHandler() {
    while((DAC->SWTRIGR & DAC_SWTRIGR_SWTRIG1) == DAC_SWTRIGR_SWTRIG1);
    DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1; //trigger conversion

    //acknowledge interrupt

    //calculate offset


    offset1 += step1;
    if((offset1>>16) >= N) //went past end of array
        offset1 -= N<<16;  //wrap around with same offset as overshoot
    offset2 += step2;
        if((offset2>>16) >= N) //went past end of array
            offset2 -= N<<16;  //wrap around with same offset as overshoot

    int sample = 0;
    sample += wavetable[offset1>>16]; //get the sample
    sample += wavetable[offset2>>16]; //get the sample
    sample = sample / 32 +2048;
    if (sample > 4095) sample = 4095; //clip
    else if (sample < 0) sample = 0;  //clip
    DAC->DHR12R1 = sample; //write sample to holding register
}


int main(void)
{
    setup_gpio();
    setup_dac();
    init_wavetable();
    setup_timer6();
    setup_adc();

   /* while(1==1) {
        int a1 = read_adc_channel(0);
        step1 = (200 + (800 * a1/4095)) * N / RATE * (1<<16);
        int a2 = read_adc_channel(1);
        step2 = (200 + (800 * a1/4095)) * N / RATE * (1<<16);
    }*/

    /*for(int i = 0; i < 100; i++) {
        int a1 = read_adc_channel(0);
                step1 = (200 + (800 * a1/4095)) * N / RATE * (1<<16);
         //       int a2 = read_adc_channel(1);
         //       step2 = (200 + (800 * a2/4095)) * N / RATE * (1<<16);
    }*/






}


