#include "mbed.h"
#define MAXIMUM_BUFFER_SIZE 80

const int numPixels = 8; // LED 개수
uint8_t pixels[numPixels * 3] = {0,}; // LED 데이터 배열
UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);
char buf[MAXIMUM_BUFFER_SIZE];
uint8_t rgb[192];

void GPIOInit(){
    // GPIO 설정
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // GPIOA 클럭 활성화 ``
    GPIOA->MODER |= (0b10 << (2 * 7));   // PA7를 AF 모드로 설정 (SPI1 MOSI) ``
    GPIOA->AFR[0] |= (0b0101 << (4 * 7)); // PA7의 AF 설정 (AF5 - SPI1) ``
}
void DMAInit() {

    // DMA2 설정
    DMA2_Stream3->CR &= ~DMA_SxCR_EN;
    while((DMA2_Stream3->CR) & DMA_SxCR_EN){;}


    DMA2->LIFCR = DMA_LISR_TCIF3  // 전송 완료 플래그 (Transfer Complete)
                | DMA_LISR_HTIF3  // 전송 중단 플래그 (Half Transfer)
                | DMA_LISR_TEIF3  // 전송 오류 플래그 (Transfer Error)
                | DMA_LISR_DMEIF3 // 직접 모드 오류 플래그 (Direct Mode Error)
                | DMA_LISR_FEIF3; // FIFO 오버플로우 또는 언더플로우 플래그 (FIFO Error)
            

    // DMA2_Stream3->CR &= 0x0000;
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;        // DMA2 클럭 활성화
    DMA2_Stream3->CR |= (0b011U << 25);          // CHSEL[27:25] ``
    DMA2_Stream3->CR |= (0b11U  << 16);              // PL [17:16] `` 
    DMA2_Stream3->CR |= (0b1U << 10);               // MINC [10] ``
    DMA2_Stream3->CR |= (0b1 << 8);              // CIRC [8]
    DMA2_Stream3->CR |= (0b01U << 6);               // DIR [7:6] `` 
    DMA2_Stream3->CR |= (0b1U << 4);                 // TCIE [4] `` 
    DMA2_Stream3->CR |= (0b0U << 0);                 // EN [0] `` 

    DMA2_Stream3->FCR &=~DMA_SxFCR_DMDIS;

    DMA2_Stream3->PAR = (uint32_t)&SPI1->DR; // 주변장치 주소 설정
}

// SPI 및 DMA 설정을 수행하는 함수
void SPIInit() {

    // SPI 설정

    SPI1->CR1 = 0x0000; 
    
    RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;   // SPI1 클럭 활성화 ``
    SPI1->CR1 |= (0b1U << 9);                //  bit[9] - SSM=1
    SPI1->CR1 |= (0b1U << 6);             //  bit[6] - SPE=1 : SPI enable
    SPI1->CR1 |= (0b011U << 3);                   // bit[5:3] (= BR [2:0]) : BR=0b011로 설정하여 클럭 분주 비율을 f_PCLK/16로 설정
    SPI1->CR1 |= (0b1U << 2);              //  bit[2] - MSTR=1: 마스터 모드로 설정
    SPI1->CR1 |= 0b1U;                        //  CPHA=1 


    SPI1->CR2 = 0x0000; 
    SPI1->CR2 |= (0b1U << 7);      //  TXEIE=1 : : TXE interrupt not masked. Used to generate an interrupt request when the TXE flag is set
    SPI1->CR2 |= (0b1U << 1);      // TXDMAEN=1 : Tx buffer DMA enabled -> the DMA request is made whenever the TXE flag is set.
    // RXDMAEN=0
}

void DMASend(){   

    // WS2812 LED로 데이터 전송을 위한 한 줄 세우기
    int cnt = 191;
    for (int i = 0; i < numPixels * 3; i++) {
        for (int j = 7; j >= 0; j--) {
            rgb[cnt--] = (pixels[i] >> j) & 1 ? 0b11111000 : 0b11000000;
        }
    }
    
        
    DMA2->LIFCR |=DMA_LIFCR_CTCIF3|DMA_LIFCR_CHTIF3|DMA_LIFCR_CTEIF3|DMA_LIFCR_CDMEIF3|DMA_LIFCR_CFEIF3;
    DMA2_Stream3->M0AR = (uint32_t)rgb;
    DMA2_Stream3->NDTR=(192);

    // set EN bit
    DMA2_Stream3->CR |= DMA_SxCR_EN;
    while(!(DMA2->LISR & DMA_LISR_TCIF3)){}
    DMA2_Stream3->CR &= ~DMA_SxCR_EN;
    while((DMA2_Stream3->CR)&DMA_SxCR_EN){;}
    
}

void setPixel(uint8_t *pixels, int numPixels, int pixelIndex, uint8_t r, uint8_t g, uint8_t b) { // G / B / R
    pixels[pixelIndex * 3] = g;    // Green -> blue
    pixels[pixelIndex * 3 + 1] = r;  // Red -> green
    pixels[pixelIndex * 3 + 2] = b; // Blue -> red
}





int main() {
    
    sprintf(buf, "\r\n[START SPI+DMA data tranfer]\r\n\n");
    pc.write(buf, strlen(buf));


    // SPI 및 DMA 설정 초기화
    GPIOInit();
    SPIInit();
    DMAInit();

    ThisThread::sleep_for(1000ms);


    while (1) {
        memset(pixels, 0, sizeof(pixels)); // LED 데이터 초기화

        for (int i = 0; i < numPixels; i++) {
            setPixel(pixels, numPixels, i, 0, 20, 200); // G / B / R
        }
        
        DMASend();
        
        ThisThread::sleep_for(1000ms);


        memset(pixels, 0, sizeof(pixels)); // LED 데이터 초기화
        for (int i = 0; i < numPixels; i++) {
            setPixel(pixels, numPixels, i, 0, 0, 0); // G / B / R
        }
        
        DMASend();

        ThisThread::sleep_for(1000ms);

    }
}