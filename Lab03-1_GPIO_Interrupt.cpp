// 3-1
#include "mbed.h" 

volatile int interval;

void led2_init(void); 
void button_init(void); 
void led2_toggle(void);

void button_Handler(void);

// main() runs in its own thread in the OS 
int main() {
    led2_init(); 
    button_init();
    
    NVIC_SetVector(EXTI15_10_IRQn, (uint32_t)button_Handler); 
    
    interval = 500;
    
    while (true) {
        led2_toggle();
        ThisThread::sleep_for((std::chrono::milliseconds)(interval)); 
    }
}

void button_init(void){ 
    __HAL_RCC_SYSCFG_CLK_ENABLE();
      
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    SYSCFG->EXTICR[3] &= ~(0b1111<<4);
    SYSCFG->EXTICR[3] |= (0b0010<<4);  
    EXTI->RTSR |= 0b1<<13;    
    EXTI->IMR |= 0b1<<13;     
}

void button_Handler(void){
    __HAL_RCC_GPIOC_CLK_ENABLE();  
    interval = interval << 1;
    if(interval>2000){
        interval = 500;
    }
    EXTI->PR = 0b1<<13;  
}

void led2_init(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    GPIOA->MODER &= ~(0b11<<10);
    GPIOA->MODER |= (0b01<<10);
    
}

void led2_toggle(void){
    if(!(GPIOA->ODR | 0b0<<5))
        GPIOA->ODR |= (0b1<<5);
    else if(GPIOA->ODR & 0b1<<5)
        GPIOA->ODR &= (0b0<<5);
}