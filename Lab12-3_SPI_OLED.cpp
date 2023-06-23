// 12-3

#include "Adafruit_SSD1306.h"
#include "mbed.h"
// #include "Adafruit_GFX.h"

// a class to pre-initialize a SPI
class SPIPreInit: public SPI
{
public:
    SPIPreInit(PinName mosi, PinName miso , PinName clk) : SPI(mosi, miso, clk)
    {
        format(8,3);
        frequency(2000000);
    };
};

SPIPreInit gSPI(ARDUINO_UNO_D11 , NC , ARDUINO_UNO_D13);

// Adafruit_SSD1306_Spi(SPI &spi , PinName DC , PinName RST , PinName CS , uint8_t rawHeight=32 , uint8_t rawWidth=128)
Adafruit_SSD1306_Spi goLED(gSPI, ARDUINO_UNO_D4, ARDUINO_UNO_D7, ARDUINO_UNO_D10, 64);

int main(){
    int tmp;

    ThisThread::sleep_for(5000ms);
    goLED.clearDisplay();

    goLED.printf("Handong Univ. CSEE\r\n\n\n\n");
    goLED.printf("Temperature:\r\n\n");
    goLED.printf("Humidity:");

    while(1){
        goLED.setTextCursor(13*6 , 4*8);
        tmp = rand() % 1000;
        goLED.printf("%4.1f c", tmp/10.0);

        goLED.setTextCursor(13*6, 6*8);
        tmp = rand() % 1000;
        goLED.printf("%4.1f %%", tmp/10.0);
        
        goLED.display();
        ThisThread::sleep_for(3000ms);
    }
}

