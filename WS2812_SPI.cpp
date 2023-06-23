#include "mbed.h"

// SPI 설정
SPI spi(D11, NC, D13);  // (MOSI, MISO, SCLK)
DigitalOut led(LED1);

// WS2812 LED 제어 함수
void setPixel(uint8_t *pixels, int numPixels, int pixelIndex, uint8_t r, uint8_t g, uint8_t b) {
    pixels[pixelIndex * 3] = g;    // Green
    pixels[pixelIndex * 3 + 1] = r;  // Red
    pixels[pixelIndex * 3 + 2] = b; // Blue
}

// WS2812 데이터 전송
void ws2812Send(uint8_t *pixels, int numPixels) {
    spi.format(8, 3); // 8비트 데이터, 모드 3
    spi.frequency(100000000/pow(2,4)); // WS2812 전송 속도 100M/2^7=781.25K

    for (int i = 0; i < numPixels * 3; i++) {
        for (int j = 7; j >= 0; j--) {
            
            // WS2812 비트 전송
            spi.write((pixels[i] >> j) & 1 ? 0b11111000 : 0b11000000);

        }
    }
}

int main() {
    const int numPixels = 8; // LED 개수
    uint8_t pixels[numPixels * 3] = {0}; // LED 데이터 배열

    // LED 데이터 초기화
    memset(pixels, 0, sizeof(pixels));
    
    // WS2812 LED로 데이터 전송하여 모든 LED 끄기
    ws2812Send(pixels, numPixels);
    
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

