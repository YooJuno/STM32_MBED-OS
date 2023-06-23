// 2-1

#include "mbed.h"

void led2_init(void);
void led2_toggle(void);

void button_init(void); 
uint32_t button_input(void); 
volatile int flag;
// main() runs in its own thread in the OS 
int main() {
    int interval = 500; 
    uint32_t val;

    led2_init(); 
    button_init();

    while (true) {
        if (val == 0) {
            interval = interval << 1; 
            if (interval > 2000)
                interval = 500; 
            
        }
        led2_toggle();
        ThisThread::sleep_for((std::chrono::milliseconds)interval); 
        val = button_input();
    }
}


void led2_init(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIOA->MODER &= ~(0b11 << 10); //PA_5
    GPIOA->MODER |= 0b01 << 10; // Output mode

}

void led2_toggle(void){
    GPIOA->ODR = (!(GPIOA->ODR>>5)) << 5;
}

void button_init(void){
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIOC->MODER &= ~(0b11<< << 26); // PC_13
    GPIOC->MODER |= 0b00 << 26; // Input mode

    GPIOC->PUPDR &= ~(0b11<< << 26);
    GPIOC->PUPDR |= (0b10<<26);
}


uint32_t button_input(void){

    if( (!((GPIOC->IDR >> 13) & 0b1)) && (flag == 0)){
        ThisThread::sleep_for(10ms);
        flag = 1;
        return 0;
    }

    else
        flag = 0;
        return 1;
}

