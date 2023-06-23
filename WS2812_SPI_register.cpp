#include "mbed.h"


// WS2812 LED 제어 함수
void setPixel(uint8_t *pixels, int numPixels, int pixelIndex, uint8_t r, uint8_t g, uint8_t b) {
    pixels[pixelIndex * 3] = g;    // Green
    pixels[pixelIndex * 3 + 1] = r;  // Red
    pixels[pixelIndex * 3 + 2] = b; // Blue
}

void spiInit(){
    // GPIO 설정
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // GPIOA 클럭 활성화 ``
    GPIOA->MODER |= (0b10 << (2 * 7));   // PA7를 AF 모드로 설정 (SPI1 MOSI) ``
    GPIOA->AFR[0] |= (0b0101 << (4 * 7)); // PA7의 AF 설정 (AF5 - SPI1) ``

    
    // SPI1 레지스터에 직접 접근하여 초기화
    SPI1->CR1 = 0x0000;
    SPI1->CR1 |= (0b0 << 15); //  BIDMODE
    SPI1->CR1 |= (0b0 << 14); //  BIDIOE
    SPI1->CR1 |= (0b0 << 13); //  CRCEN
    SPI1->CR1 |= (0b0 << 12); //  CRCNEXT
    SPI1->CR1 |= (0b0 << 11); //  DFF
    SPI1->CR1 |= (0b0 << 10); //  RXONLY
    SPI1->CR1 |= (0b1 << 9); //  SSM
    SPI1->CR1 |= (0b1 << 8); //  SSI
    SPI1->CR1 |= (0b0 << 7); //  LSBFIRST
    SPI1->CR1 |= (0b1 << 6); //  SPE
    SPI1->CR1 |= (0b011 << 3); //  BR [2:0] fPCLK/16
    SPI1->CR1 |= (0b1 << 2); //  MSTR
    SPI1->CR1 |= (0b0 << 1); //  CPOL
    SPI1->CR1 |= (0b1 << 0); //  CPHA


}

// WS2812 데이터 전송
void ws2812Send(uint8_t *pixels, int numPixels) {

    for (int i = 0; i < numPixels * 3; i++) {
        for (int j = 7; j >= 0; j--) {
            // WS2812 비트 전송
            while (!(SPI1->SR & (1 << 1)));  // TXE 비트가 1이 될 때까지 대기
            SPI1->DR = (pixels[i] >> j) & 1 ? 0b11111000 : 0b11000000;
        }
    }
}

int main() {

    // SPI1 클럭 활성화
    RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;;  // SPI1 클럭 활성화 ``

    const int numPixels = 8; // LED 개수
    uint8_t pixels[numPixels * 3] = {0}; // LED 데이터 배열

    // LED 데이터 초기화
    memset(pixels, 0, sizeof(pixels));
    spiInit();

    // 일시적으로 대기
    ThisThread::sleep_for(1000ms);

    while (1) {
        // LED 색상 설정
        for (int i = 0; i < numPixels; i++) {
            setPixel(pixels, numPixels, i, 0, 0, 20); // R / G / B
        }

        // WS2812 LED로 데이터 전송
        ws2812Send(pixels, numPixels);

        // 일시적으로 대기
        ThisThread::sleep_for(1000ms);

        // LED 데이터 초기화
        memset(pixels, 0, sizeof(pixels));

        // WS2812 LED로 데이터 전송하여 모든 LED 끄기
        ws2812Send(pixels, numPixels);

        // 일시적으로 대기
        ThisThread::sleep_for(1000ms);
    }
}
