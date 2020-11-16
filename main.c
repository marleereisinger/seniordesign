#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <math.h>
#include <stdint.h>
#include "midi.h"

/*void micro_wait(unsigned int);

// The DAC rate
#define RATE 20000
// The wavetable size
#define N 100
short int sine[N];
short int sawtooth[N];
short int square[N];
short int hybrid[N];
short int hybrid2[N];
// The number of simultaneous voices to support.
#define VOICES 15

// N powers of the the 12th-root of 2.
#define STEP1 1.05946309436
#define STEP2 (STEP1*STEP1)
#define STEP3 (STEP2*STEP1)
#define STEP4 (STEP3*STEP1)
#define STEP5 (STEP4*STEP1)
#define STEP6 (STEP5*STEP1)
#define STEP7 (STEP6*STEP1)
#define STEP8 (STEP7*STEP1)
#define STEP9 (STEP8*STEP1)

// Macros for computing the fixed point representation of
// the step size used for traversing a wavetable of size N
// at a rate of RATE to produce tones of various doublings
// and halvings of 440 Hz.  (The "A" above middle "C".)
 *
 */
//#define A14    ((13.75   * N/RATE) * (1<<16)) /* A0 */
///#define A27    ((27.5    * N/RATE) * (1<<16)) /* A1 */
//#define A55    ((55.0    * N/RATE) * (1<<16)) /* A2 */
//#define A110   ((110.0   * N/RATE) * (1<<16)) /* A3 */
//#define A220   ((220.0   * N/RATE) * (1<<16)) /* A4 */
//#define A440   ((440.0   * N/RATE) * (1<<16)) /* A5 */
//#define A880   ((880.0   * N/RATE) * (1<<16)) /* A6 */
//#define A1760  ((1760.0  * N/RATE) * (1<<16)) /* A7 */
//#define A3520  ((3520.0  * N/RATE) * (1<<16)) /* A8 */
//#define A7040  ((7040.0  * N/RATE) * (1<<16)) /* A9 */
//#define A14080 ((14080.0 * N/RATE) * (1<<16)) /* A10 */


// A table of steps for each of 128 notes.
// step[60] is the step size for middle C.
// step[69] is the step size for 440 Hz.
//const int step[] = {
 /*       A14 / STEP9,    // C                         C-1
        A14 / STEP8,    // C# / Db
        A14 / STEP7,    // D
        A14 / STEP6,    // D# / Eb
        A14 / STEP5,    // E
        A14 / STEP4,    // F
        A14 / STEP3,    // F# / Gb
        A14 / STEP2,    // G
        A14 / STEP1,    // G# / Ab
        A14,            // A27                       A0
        A14 * STEP1,    // A# / Bb
        A14 * STEP2,    // B
        A14 * STEP3,    // C                         C0
        A14 * STEP4,    // C# / Db
        A14 * STEP5,    // D
        A27 * STEP6,    // D# / Eb
        A27 / STEP5,    // E
        A27 / STEP4,    // F*/
       // A27 / STEP3,    // F# / Gb
/*        A27 / STEP2,    // G            //note for seg 0
       // A27 / STEP1,    // G# / Ab
       // A27,            // A27                       A1
        A27 * STEP1,    // A# / Bb
        A27 * STEP2,    // B            //note seg 1
        A27 * STEP3,    // C            //note seg 2                       C1
        //A27 * STEP4,    // C# / Db
        A27 * STEP5,    // D            //note seg 3
        //A27 * STEP6,    // D# / Eb
        A55 / STEP5,    // E            //note seg4
        A55 / STEP4,    // F            //note seg5
        //A55 / STEP3,    // F# / Gb
        A55 / STEP2,    // G            //note seg6
        //A55 / STEP1,    // G# / Ab
        A55,            // A55          //note seg7                       A2
        //A55 * STEP1,    // A# / Bb
        A55 * STEP2,    // B            //note seg 8
        A55 * STEP3,    // C            //note seg 9             C2
      /*  A55 * STEP4,    // C# / Db
        A55 * STEP5,    // D
        A55 * STEP6,    // D# / Eb
        A110 / STEP5,   // E
        A110 / STEP4,   // F
        A110 / STEP3,   // F# / Gb
        A110 / STEP2,   // G
        A110 / STEP1,   // G# / Ab
        A110,           // A110                     A3
        A110 * STEP1,   // A# / Bb
        A110 * STEP2,   // B
        A110 * STEP3,   // C                        C3
        A110 * STEP4,   // C# / Db
        A110 * STEP5,   // D
        A110 * STEP6,   // D# / Eb
        A220 / STEP5,   // E
        A220 / STEP4,   // F
        A220 / STEP3,   // F# / Gb
        A220 / STEP2,   // G
        A220 / STEP1,   // G# / Ab
        A220,           // A220                     A4
        A220 * STEP1,   // A# / Bb
        A220 * STEP2,   // B
        A220 * STEP3,   // C (middle C)             C4 (element #60)
        A220 * STEP4,   // C# / Db
        A220 * STEP5,   // D
        A220 * STEP6,   // D# / Eb
        A440 / STEP5,   // E
        A440 / STEP4,   // F
        A440 / STEP3,   // F# / Gb
        A440 / STEP2,   // G
        A440 / STEP1,   // G# / Ab
        A440,           // A440                     A5
        A440 * STEP1,   // A# / Bb
        A440 * STEP2,   // B
        A440 * STEP3,   // C                        C5
        A440 * STEP4,   // C# / Db
        A440 * STEP5,   // D
        A440 * STEP6,   // D# / Eb
        A880 / STEP5,   // E
        A880 / STEP4,   // F
        A880 / STEP3,   // F# / Gb
        A880 / STEP2,   // G
        A880 / STEP1,   // G# / Ab
        A880,           // A880                     A6
        A880 * STEP1,   // A# / Bb
        A880 * STEP2,   // B
        A880 * STEP3,   // C                        C6
        A880 * STEP4,   // C# / Db
        A880 * STEP5,   // D
        A880 * STEP6,   // D# / Eb
        A1760 / STEP5,  // E
        A1760 / STEP4,  // F
        A1760 / STEP3,  // F# / Gb
        A1760 / STEP2,  // G
        A1760 / STEP1,  // G# / Ab
        A1760,          // A1760                   A7
        A1760 * STEP1,  // A# / Bb
        A1760 * STEP2,  // B
        A1760 * STEP3,  // C                       C7
        A1760 * STEP4,  // C# / Db
        A1760 * STEP5,  // D
        A1760 * STEP6,  // D# / Eb
        A3520 / STEP5,  // E
        A3520 / STEP4,  // F
        A3520 / STEP3,  // F# / Gb
        A3520 / STEP2,  // G
        A3520 / STEP1,  // G# / Ab
        A3520,          // A3520                   A8
        A3520 * STEP1,  // A# / Bb
        A3520 * STEP2,  // B
        A3520 * STEP3,  // C                       C8
        A3520 * STEP4,  // C# / Db
        A3520 * STEP5,  // D
        A3520 * STEP6,  // D# / Eb
        A7040 / STEP5,  // E
        A7040 / STEP4,  // F
        A7040 / STEP3,  // F# / Gb
        A7040 / STEP2,  // G
        A7040 / STEP1,  // G# / Ab
        A7040,          // A7040                   A9
        A7040 * STEP1,  // A# / Bb
        A7040 * STEP2,  // B
        A7040 * STEP3,  // C                       C9
        A7040 * STEP4,  // C# / Db
        A7040 * STEP5,  // D
        A7040 * STEP6,  // D# / Eb
        A14080 / STEP5, // E
        A14080 / STEP4, // F
        A14080 / STEP3, // F# / Gb
        A14080 / STEP2, // G*/
//};

// An array of "voices".  Each voice can be used to play a different note.
// Each voice can be associated with a channel (explained later).
// Each voice has a step size and an offset into the wave table.
/*struct {
    uint8_t in_use;
    uint8_t note;
    uint8_t chan;
    uint8_t volume;
    int     step;
    int     offset;
    int last_sample;
    int16_t *wavetable;
} voice[VOICES];

int16_t *channel_wavetable[20]; // It's zero, by default.

void program_change(int time, int chan, int prog)
{
    channel_wavetable[chan] = hybrid; // Use this by default.
    if (prog >= 71 && prog <= 79) // flutes, recorders, etc.
        channel_wavetable[chan] = sine;
    if (prog == 29) // overdriven guitar
        channel_wavetable[chan] = square;
    if (prog >= 52 && prog <= 54) // choir voices
        channel_wavetable[chan] = sine;
}

// Initialize the DAC so that it can output analog samples
// on PA4.  Configure it to either be software-triggered
// or triggered by TIM6.
void init_DAC(void) {
    DAC->CR |= DAC_CR_TEN1; //Enable trigger
    DAC->CR &= ~DAC_CR_TSEL1; //select Timer 6 TRGO event
    //setup GPIOA4
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //enables clock to port A
    GPIOA->MODER |= 0x300;             //sets pa4 to analog (mode 11)
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    RCC->APB1ENR |= RCC_APB1ENR_DACEN; //enable clock to onboard DAC
    DAC->CR &= ~DAC_CR_EN1;  //Disable DAC channel 1
    DAC->CR &= ~DAC_CR_BOFF1; //Do not turn buffer off
    DAC->CR |= DAC_CR_EN1;   //enable DAC channel 1

}

uint16_t queue[200]; // the circular buffer
void DMA1_Channel2_3_IRQHandler(void)
{
    uint16_t *arr;
    if (DMA1->ISR & DMA_ISR_HTIF3) {
        // At halfway mark.  Refill the first half of the buffer.
        arr = &queue[0]; // point to beginning of buffer
        // Need to clear both the half-transfer and global flags.
        DMA1->IFCR = DMA_IFCR_CHTIF3|DMA_IFCR_CGIF3;
    } else {
         // Total complete.  Refill the second half of the buffer.
        arr = &queue[100]; // point to second half of buffer
         // Need to clear both the transfer-complete and global flags.
        DMA1->IFCR = DMA_IFCR_CTCIF3|DMA_IFCR_CGIF3;
    }

    int i;
    for(i = 0; i < 100; i++) {


        // TODO: Copy the sample generation from TIM6_DAC_IRQHandler()
        int x;
        int sample = 0;
        for(x=0; x < sizeof(voice)/sizeof(voice[0]); x++) {
            if (voice[x].in_use) {
                voice[x].offset += voice[x].step;
                if (voice[x].offset >= N<<16)
                    voice[x].offset -= N<<16;
                voice[x].last_sample = voice[x].wavetable[voice[x].offset>>16];
                sample += (voice[x].last_sample * voice[x].volume) >> 4;
            }
            else if (voice[x].volume != 0) {
                sample += (voice[x].last_sample * voice[x].volume) >> 4;
                voice[x].volume --;
            }
        }
        sample = sample / 1024 + 2048;
        if (sample > 4095) sample = 2050;
        else if (sample < 0) sample = 0;

        arr[i] = sample;
    }
}

void init_DMA() {
    RCC->AHBENR |= RCC_AHBENR_DMA1EN; //ENABLE CLOCK TO DMA1
    DMA1_Channel3->CCR &= ~DMA_CCR_EN; //Make sure DMA is turned off
    DMA1_Channel3->CMAR = (uint32_t) queue; //Copy from address in CMAR
    DMA1_Channel3->CPAR = (uint32_t) &DAC->DHR12R1; //Copy to this address in CPAR
    DMA1_Channel3->CNDTR = sizeof(queue) / sizeof(queue[0]); //will size of array at time

    DMA1_Channel3->CCR |= DMA_CCR_DIR; //Memory to peripheral
    DMA1_Channel3->CCR |= DMA_CCR_MINC; //Increment memory after each transfer
    DMA1_Channel3->CCR &= ~DMA_CCR_PINC; //Don't increment peripheral
    //mem and peripheral element size = 16, circular transfer, transfer and half interrupt enabled
    DMA1_Channel3->CCR |= DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0 | DMA_CCR_CIRC | DMA_CCR_TCIE | DMA_CCR_HTIE; //Set peripheral and memory size to 16 bits
    NVIC->ISER[0] = 1<<(DMA1_Channel2_3_IRQn);
    NVIC_SetPriority(DMA1_Channel2_3_IRQn,0); //set to highest priority
     //enable timer 6 interrupt
    // DMA1_Channel3->CCR &= ~DMA_CCR_PL; //Priority low

   // SPI2->CR2 |= SPI_CR2_TXDMAEN; //DMA request made when transfer buffer is empty
    DMA1_Channel3->CCR |= DMA_CCR_EN; //Enable

}
// Initialize Timer 6 so that it calls TIM6_DAC_IRQHandler
// at exactly RATE times per second.  You'll need to select
// a PSC value and then do some math on the system clock rate
// to determine the value to set for ARR.
void init_TIM6(void) {

    // TODO: you fill this in.
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; //enable clock to timer 6

    TIM6->PSC = 1-1; //Set to lowest prescaler
    TIM6->ARR = 48000000 / ((TIM6->PSC + 1) * RATE) - 1; //calculate ARR based on PSC value
    TIM6->DIER |= TIM_DIER_UDE;
    TIM6->CR2 |= TIM_CR2_MMS_1;
    TIM6->CR1 |= TIM_CR1_CEN; //start timer
    NVIC_SetPriority(TIM6_DAC_IRQn,0); //set to highest priority
    NVIC->ISER[0] = 1<<(TIM6_DAC_IRQn); //enable timer 6 interrupt
}

// sine wave
void init_sine(void) {
    int x;
    for(x=0; x<N; x++) {
        sine[x] = 32767 * sin(2 * M_PI * x / N);
    }
}

// sawtooth wave
void init_sawtooth(void) {
    int x;
    for(x=0; x<N; x++)
        sawtooth[x] = 32767.0 * (x - N/2) / (1.0*N);
}

// square wave
void init_square(void) {
    int x;
    for(x=0; x<N; x++)
        if (x < N/2)
            square[x] = -32767;
        else
            square[x] = 32767;
}

// 1/2 amplitude sine wave added to 1/2 amplitude sawtooth wave
void init_hybrid(void) {
    int x;
    for(x=0; x<N; x++)
        hybrid[x] = 16383 * sin(2 * M_PI * x / N) + 16383.0 * (x - N/2) / (1.0*N);
}

// 3/4 amplitude sine wave added to 1/4 amplitude sawtooth wave
void init_hybrid2(void) {
    int x;
    for(x=0; x<N; x++)
        hybrid2[x] = 3*8191 * sin(2 * M_PI * x / N) + 8191.0 * (x - N/2) / (1.0*N);
}

// Find the voice current playing a note, and turn it off.
void note_off(int time, int chan, int key, int velo)
{
    int n;
    for(n=0; n<sizeof voice / sizeof voice[0]; n++) {
        if (voice[n].in_use && voice[n].note == key) {
            voice[n].in_use = 0; // disable it first...
            voice[n].chan = 0;   // ...then clear its values
            voice[n].note = key;
            voice[n].step = step[key];
            return;
        }
    }
}

// Find an unused voice, and use it to play a note.
void note_on(int time, int chan, int key, int velo)
{
    if (velo == 0) {
        note_off(time, chan, key, velo);
        return;
    }
    int n;
    for(n=0; n<sizeof voice / sizeof voice[0]; n++) {
        if (voice[n].in_use == 0) {
            if(channel_wavetable[chan] == 0) {
                voice[n].wavetable = hybrid;
            }
            else {
                voice[n].wavetable = channel_wavetable[chan];
            }

            voice[n].volume = velo; //save velvo to volume vield
            voice[n].note = key;
            voice[n].step = step[key];
            voice[n].offset = 0;
            voice[n].chan = chan;
            voice[n].in_use = 1;
            return;
        }
    }
}

void setup_timer2() {
    // Enable timer 2 here.
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    // Make it generate an interrupt 8 times per second.
    // Set the prescaler to 48 so that the free-running
    // counter goes from 0 ... 124999.
    TIM2->PSC = 48 - 1; //Set prescaler to 48
    TIM2->ARR = 125000 - 1; //Will interrupt 8 x per second
    TIM2->DIER |= TIM_DIER_UIE; //Update interrupt enabled
    TIM2->CR1 |= TIM_CR1_CEN; //Start the timer
    NVIC->ISER[0] = (1 << TIM2_IRQn); //Unmask interrupt
    NVIC_SetPriority(TIM2_IRQn, 2); //set to lower priority than USART1

    // Also, make sure TIM2 has lower priority than USART1
    // by using the NVIC_SetPriority() function.  Use a higher
    // number (lower priority) than you did for USART1.
}

//sets up TIM2 with
void init_TIM2(int n) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //Enable clock to TIM2
    TIM2->PSC = 48 - 1; //set psc value
    TIM2->ARR = n -1;  //set arr vaue
    TIM2->CR1 |= TIM_CR1_ARPE; //allows to change rate of timer dynamically
    TIM2->DIER |= TIM_DIER_UIE; //allow channel 1 to generate
    TIM2->CR1 |= TIM_CR1_CEN; //enable counter
    NVIC_SetPriority(TIM2_IRQn,3); //set to lower priority than tim6
    NVIC->ISER[0] = 1<<TIM2_IRQn; //Enable interrupt

}

void TIM2_IRQHandler(void)
{
    // TODO: Remember to acknowledge the interrupt right here!
    TIM2->SR &= ~0x1;
    midi_play();
}

void set_tempo(int time, int value, const MIDI_Header *hdr)
{
    // This assumes that the TIM2 prescaler divides by 48.
    // It sets the timer to produce an interrupt every N
    // microseconds, where N is the new tempo (value) divided by
    // the number of divisions per beat specified in the MIDI header.
    TIM2->ARR = value/hdr->divisions - 1;
}

void pitch_wheel_change(int time, int chan, int val)
{

    float multiplier = pow(1.05946309436, (val - 8192.0) / 8192.0); //multiplier calc
    int n;
    for(n=0; n<sizeof voice / sizeof voice[0]; n++) {
        if (voice[n].in_use == 0 && voice[n].chan == chan) {
            voice[n].step = step[voice[n].note] * multiplier;
        }
    }
}


extern uint8_t midifile[];

int main(void)
{
  //  init_sawtooth();
    init_hybrid();      // pick one wavetable form
   // init_hybrid2();
   // init_square();
   // init_sine();
    init_DAC();         // initialize the DAC
    init_DMA();
    init_TIM6();        // initialize TIM6
  //  MIDI_Player *mp = midi_init(midifile);
    init_TIM2(10417);  // initialize TIM2

    // Turn on one note at a time, each using 9 different voices.
    // Make sure that the following sounds good.
    // Alternately, you might use an AD2 to look at the output
    // while you turn on only note 69 (440 Hz).

    for(int i = 0; i < 100; i++) {
        note_on(0, 0, i % 10, 128); //turns on pc0 - 9 in order
        micro_wait(10);
    }*/

   // note_on(0,0, 1, 128); // middle "C"
 //   note_on(0,0, 1, 128);
 //   note_on(0,0, 2, 128);

 //   note_on(0,0, 9, 128);
//    note_on(0,0, 76, 128);
//    note_on(0,0, 79, 128);

 //   note_on(0,0, 84, 128);
 //   note_on(0,0, 88, 128);
 //   note_on(0,0, 91, 128);

  /*  for(;;)
        asm("wfi");
    return 0;
}*/
