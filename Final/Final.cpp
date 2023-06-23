#include "mbed.h"
#include <cstring>
#define MAXIMUM_BUFFER_SIZE 80

// Set SPI
SPI spi(D11, NC, D13); // (MOSI, MISO, SCLK)
PwmOut buzzer(PB_3);
float freq[] = {261.63,293.66,329.63,349.23,392.00,440.00,493.88, 523.25}; //음계별 주파수
Timer timer;
DigitalIn switchInputs[] = {D2, D4, D5, D6, D7, D8, D9}; // Array of switch inputs
UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);
char buf[MAXIMUM_BUFFER_SIZE];
volatile bool RGB_ON[3]={true, false, false};

const int numPixels = 8; // LED 개수
uint8_t pixels[numPixels * 3] = {0,}; // LED 데이터 배열
uint8_t TX_buf[192];

void GPIOInit(){
    // GPIO 설정
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // GPIOA 클럭 활성화 ``
    GPIOA->MODER |= (0b10 << (2 * 7));   // PA7를 AF 모드로 설정 (SPI1 MOSI) ``
    GPIOA->AFR[0] |= (0b0101 << (4 * 7)); // PA7의 AF 설정 (AF5 - SPI1) ``
}
void DMAInit() {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; // DMA2 클럭 활성화

    // DMA2 설정
    DMA2_Stream3->CR &= ~DMA_SxCR_EN;
    while((DMA2_Stream3->CR) & DMA_SxCR_EN){;}                                                                                                                                


    DMA2->LIFCR = DMA_LISR_TCIF3  // 전송 완료 플래그 (Transfer Complete)
                | DMA_LISR_HTIF3  // 전송 중단 플래그 (Half Transfer)
                | DMA_LISR_TEIF3  // 전송 오류 플래그 (Transfer Error)
                | DMA_LISR_DMEIF3 // 직접 모드 오류 플래그 (Direct Mode Error)
                | DMA_LISR_FEIF3; // FIFO 오버플로우 또는 언더플로우 플래그 (FIFO Error)
            

    DMA2_Stream3->CR &= 0x0000;
    DMA2_Stream3->CR |= (0b011U << 25); // CHSEL[27:25] ``
    // DMA2_Stream3->CR |= (0b10 << 23); // MBURST [24:23] 
    // DMA2_Stream3->CR |= (0b10 << 21); // PBURST [22:21]
    // DMA2_Stream3->CR |= (0b0 << 20); // Reserved [20]
    // DMA2_Stream3->CR |= (0b0 << 19); // CT [19]
    // DMA2_Stream3->CR |= (0b0 << 18); // DBM [18]
    DMA2_Stream3->CR |= (0b11U  << 16); // PL [17:16] `` 
    // DMA2_Stream3->CR |= (0b0 << 15); // PINCOS [15]
    // DMA2_Stream3->CR |= (0b00 << 13); // MSIZE [14:13] - 8bit
    // DMA2_Stream3->CR |= (0b00 << 11); // PSIZE [12:11] - 8bit
    DMA2_Stream3->CR |= (0b1U << 10); // MINC [10] ``
    // DMA2_Stream3->CR |= (0b1 << 9); // PINC [9]
    DMA2_Stream3->CR |= (0b1 << 8); // CIRC [8]
    DMA2_Stream3->CR |= (0b01U << 6); // DIR [7:6] `` 
    // DMA2_Stream3->CR |= (0b0 << 5); // PFCTRL [5]
    DMA2_Stream3->CR |= (0b1U << 4); // TCIE [4] `` 
    // DMA2_Stream3->CR |= (0b1 << 3); // HTIE [3]
    // DMA2_Stream3->CR |= (0b1 << 2); // TEIE [2]
    // DMA2_Stream3->CR |= (0b1 << 1); // DMEIE [1]
    DMA2_Stream3->CR |= (0b0U << 0); // EN [0] `` 

    DMA2_Stream3->FCR &= ~DMA_SxFCR_DMDIS;

    DMA2_Stream3->PAR = (uint32_t)&SPI1->DR; // 주변장치 주소 설정
}

// SPI 및 DMA 설정을 수행하는 함수
void SPIInit() {

    // SPI 설정
    RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;  // SPI1 클럭 활성화 ``

    SPI1->CR1 = 0x0000; 
    // bit[15:14] - BIDIMODE=0, BIDIOE=0 : unidirectional transmit-only mode 
    // bit[13] - CRCEN=0
    // bit[12] - CRCNEXT=0
    // bit[11] - DFF=0
    // bit[10] - RXONLY=0 : Full duplex (Transmit and receive). [1로하면 receive-only mode]
    SPI1->CR1 |= (0b1U << 9); //  bit[9] - SSM=1
    SPI1->CR1 |= (0b1U << 8); //  bit[8] - SSI=1
    //  bit[7] - LSBFIRST=0 : MSB first
    SPI1->CR1 |= (0b1U << 6); //  bit[6] - SPE=1 : SPI enable
    SPI1->CR1 |= (0b011U << 3); // bit[5:3] (= BR [2:0]) : BR=0b011로 설정하여 클럭 분주 비율을 f_PCLK/16로 설정
    SPI1->CR1 |= (0b1U << 2); //  bit[2] - MSTR=1: 마스터 모드로 설정
    //  CPOL=0 -> bit[1:0] : SPI mode1
    SPI1->CR1 |= 0b1U; //  CPHA=1 

    SPI1->CR2 = 0x0000; 
    SPI1->CR2 |= (0b1U << 7); //  TXEIE=1 : TXE interrupt not masked. Used to generate an interrupt request when the TXE flag is set
    // RXNEIE=0
    // ERRIE=0
    // FRF=0
    // Reserved=0
    // SSOE=0
    SPI1->CR2 |= (0b1U << 1); // TXDMAEN=1 : Tx buffer DMA enabled -> the DMA request is made whenever the TXE flag is set.
    // RXDMAEN=0
}


void DMASend(){ 

    DMA2->LIFCR |= DMA_LIFCR_CTCIF3 | DMA_LIFCR_CHTIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CFEIF3; // clear flags

    // disable EN bit
    DMA2_Stream3->CR &= ~DMA_SxCR_EN;
    while((DMA2_Stream3->CR)&DMA_SxCR_EN){;}
    sprintf(buf, "Off %x\r\n", DMA2_Stream3->CR);
    pc.write(buf, strlen(buf));  

    // WS2812 LED로 데이터 전송을 위한 한 줄 세우기
    int cnt = 0;
    for (int i = 0; i < numPixels * 3; i++) {
        for (int j = 7; j >= 0; j--) {
            TX_buf[cnt++] = (pixels[i] >> j) & 1 ? 0b11111000 : 0b11000000;
        }
    }
    
    DMA2_Stream3->M0AR = (uint32_t)TX_buf;
    DMA2_Stream3->NDTR=(192);

    // set EN bit
    DMA2_Stream3->CR |= DMA_SxCR_EN;
    while(!(DMA2->LISR & DMA_LISR_TCIF3)){}
    sprintf(buf, "ENbit enabled! %x\r\n\n",DMA2_Stream3->CR);
    pc.write(buf, strlen(buf));

}


// WS2812 LED control function
void setPixel(uint8_t *pixels, int numPixels, int pixelIndex, uint8_t r, uint8_t g, uint8_t b) {
    pixels[pixelIndex * 3] = g; // Green
    pixels[pixelIndex * 3 + 1] = r; // Red
    pixels[pixelIndex * 3 + 2] = b; // Blue
}

// Transmit WS2812 data via SPI
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
    const int numPixels = 8; // Number of LEDs
    int on_idx=0;
    float period_us; //PWM신호의 주기
    float intensity_adjVal[3]={0,};


     // LED 데이터 초기화
    memset(pixels, 0, sizeof(pixels));

    GPIOInit();
    SPIInit();
    DMAInit();

    while (1) {
        on_idx=-1;

        //스위치 배열에서 ON된 것 확인
        for(int i=0; i<sizeof(switchInputs)/sizeof(switchInputs[0]); i++){
            if((float)switchInputs[i].read()>0.95f){ 
                on_idx=i;
                if(timer.elapsed_time().count()==0)
                    timer.start();
            }
        }

        //스위치 떼면 timer reset
        if(switchInputs[on_idx].read()!=1){
            timer.reset();
            RGB_ON[2]=RGB_ON[1]=!(RGB_ON[0]=true);
        }

        float time=(float)timer.elapsed_time().count()/10000; //intensity조절 변수
        
        if(RGB_ON[0] && time>100){
            RGB_ON[0]=false;
            RGB_ON[1]=true;
            timer.reset();
        }
        else if(RGB_ON[1] && time>100){
            RGB_ON[1]=false;
            RGB_ON[2]=true;
            timer.reset();
        }
        else if(RGB_ON[2] && time>100)
            time=100;
        
        sprintf(buf,"sec : %f\r\n on_idx : %d", time, on_idx); //sec
        pc.write(buf,strlen(buf));

        //LED색상 설정
        for (int j = 0; j < numPixels; j++){
            if(j == on_idx){
                setPixel(pixels, numPixels, j, RGB_ON[0]*time, RGB_ON[1]*time, RGB_ON[2]*time); // R/G/B
                //부저 소리 재생
                buzzer =0.1; //write() duty cycle 90%를 의미
                period_us = 1000000/freq[j]; //주기 T = 1s/freq
                buzzer.period_us(period_us);
                ThisThread::sleep_for(chrono::milliseconds(100));
            }
            else{
                setPixel(pixels, numPixels, j, 0, 0, 0); // R/G/B
            }
        }
        //아무 스위치도 눌리지 않았을 때
        if(on_idx==-1) buzzer=0;
        // sprintf(buf, "D2 switch value: %d\r\n", (int)switchInputs[0]);
        // pc.write(buf, strlen(buf));
        // sprintf(buf, "D3 switch value: %d\r\n", (int)switchInputs[1]);
        // pc.write(buf, strlen(buf));
        
        // Send data to WS2812 LED
        DMASend();

        wait_us(60);
        memset(pixels, 0, sizeof(pixels));
    }
}