// #ifndef MBED_ULTRASONIC_H 
// #define MBED_ULTRASONIC_H 
// #include "mbed.h"

// class SRF05 {
//     public:
//         /**iniates the class with the specified trigger pin, echo pin,
//         * measurement period [sec] and operation mode (single or repeated)
//         */
//         SRF05(PinName trigPin, PinName echoPin, float period = 1.0, bool repeat = false);

//         ~SRF05();

//         // start measure measurement
//         void start(float period, bool repeat); 
//         void start(void);

//         void stop(void); // stop measuring

//         // return distance in cm and clear the done status 
//         float getDistance(void);
//         // return the echo pulse duration in us 
//         int getPulseDuration(void);
//         // get a status whether measurement is done or not 
//         int getStatus(void);
//         // set measurement mode (repeated or once) 
//         void setMode(bool mode);
//         // set measurement period 
//         void setPeriod(float period);

//         private:
//             DigitalOut _trig; 
//             InterruptIn _echo;

//             Timer _timer; 
//             Ticker _ticker;

//             float _period;  //timeout value in sec, or retriig time in sec fi repeat == true 
//             bool _repeat;   // if true, measure repeatedly with the time interval timeout
          
//             float _distance;
//             int _pulseDuration;

//             void _startT(void); // ISR for the ECHO rising edge 
//             void _endT(void); // ISR for the ECHO falling edge 
//             void _ticker_cb(void);// ISR for ticker
//             int _done; // end of measure
// }; 

// #endif

#ifndef MBED_ULTRASONIC_H
#define MBED_ULTRASONIC_H
#include "mbed.h"

class srf05 {
    public:
        srf05(PinName trigPin, PinName echoPin);

        void measure_once();
        void measure_repeat(float period);
        void measure_stop();

        float getDistance();
        int getPulseDuration();
        bool is_available();

    private:
        DigitalOut _trig;
        InterruptIn _echo;

        Timer _timer;
        Ticker _ticker;

        float _distance;
        int _pulseDuration;
        bool _newMeasurement;

        void _startT();
        void _endT();
        void _ticker_cb();
};

#endif
