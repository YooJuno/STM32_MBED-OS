// 6-3
#include "mbed.h"

// Used for blinking the RED
Timer t;
char buf[80];

static BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);

int main() {

    t.start();
    int i=0;
    while(i<=1000000){
        i = i+1;
    }
    t.stop();

    // sprintf(buf, "The time taken was %llu ms\r\n", duration_cast<std::chrono::milliseconds>(t.elapsed_time()).count());
    sprintf(buf, "The time taken was %llu us\r\n", duration_cast<std::chrono::microseconds>(t.elapsed_time()).count());
    pc.write(buf, strlen(buf));

    return 0;
}