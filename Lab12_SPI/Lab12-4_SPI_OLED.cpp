
// Lab 12-4_SPI_OLED

#include "mbed.h"
#include "Adafruit_SSD1306.h"

#define HTU21D_TEMP_READ (0xF3) // no hold command 
#define HTU21D_HUMI_READ (0xF5)
#define HTU21G_USER_WRITE (0xE6)
#define HTU21D_ADDR (0x40)
I2C i2c(PB_9, PB_8); // SDA , SCL
//SPI gSPI(ARDUINO_UNO_D11, NC, ARDUINO_UNO_D13); // MOSI, SCK
BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);
char buffer[80];
volatile bool read_HumTemp, crc_err1,crc_err2;
volatile float temp, hum;
int main(){
    int tmp;

    // Adafruit_SSD1306_Spi(PinName MOSI, PinName CLK, PinName CS, PinName DC, PinName RST, uint8_t rawHeight = 32, uint8_t rawWidth = 128);
    Adafruit_SSD1306_Spi goLED(ARDUINO_UNO_D11, ARDUINO_UNO_D13, ARDUINO_UNO_D10, ARDUINO_UNO_D4, ARDUINO_UNO_D7, 64,128);
    

    ThisThread::sleep_for(5000ms);
    goLED.clearDisplay();
    goLED.setTextSize(2);
    goLED.printf("YooJuno\r\n\n");
    goLED.setTextSize(1);
    goLED.printf("Temperature:\r\n\n");
    goLED.printf("Humidity:");
    
    while(1){


        

        


        char tx_temp[1];
        char rx_temp[3];
        tx_temp[0] = HTU21D_TEMP_READ; 
        i2c.write((HTU21D_ADDR << 1) , tx_temp, 1); // Soft Reset
        ThisThread::sleep_for(50ms); // 14bits
        i2c.read((HTU21D_ADDR << 1) , rx_temp, 3);
        
        ThisThread::sleep_for(1ms);
        unsigned int temperature = ((unsigned int) rx_temp[0] << 8) | (unsigned int) rx_temp[1];
        temperature &= 0xFFFC; // 1111 1111 1111 1100
        float Temp = -46.85 + (175.72 * (temperature / 65536.0));




        char tx_humid[1];
        char rx_humid[3];
        tx_humid[0] = HTU21D_HUMI_READ;
        i2c.write((HTU21D_ADDR << 1) , tx_humid, 1);
        ThisThread::sleep_for(16ms); // 12bits
        i2c.read((HTU21D_ADDR << 1), rx_humid, 3);
        ThisThread::sleep_for(1ms);
        unsigned int humidity = ((unsigned int) rx_humid[0] << 8) | (unsigned int) rx_humid[1];
        humidity &= 0xFFFE; // 1111 1111 1111 1110
        float RH = -6 + (125 * (humidity / 65536.0));
        
        
        
        
        
        goLED.setTextCursor(13*6 , 4*8);
        goLED.printf("%4.1f c", Temp);

        goLED.setTextCursor(13*6, 6*8);
        goLED.printf("%4.1f %%", RH);
        
        goLED.display();
        // ThisThread::sleep_for(3000ms);


      
        ThisThread::sleep_for(3s);  
    }
}