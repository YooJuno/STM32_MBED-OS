#include "srf05.h"

srf05::srf05(PinName trigPin, PinName echoPin)
    : _trig(trigPin), _echo(echoPin), _newMeasurement(false) {
    _echo.rise(callback(this, &srf05::_startT));
    _echo.fall(callback(this, &srf05::_endT));
}

void srf05::measure_once() {
    _ticker.detach();
    _ticker_cb();
}

void srf05::measure_repeat(float period) {
    _ticker.attach(callback(this, &srf05::_ticker_cb), period);
}

void srf05::measure_stop() {
    _ticker.detach();
}

float srf05::getDistance() {
    _newMeasurement = false;
    return _distance;
}

int srf05::getPulseDuration() {
    return _pulseDuration;
}

bool srf05::is_available() {
    return _newMeasurement;
}

void srf05::_startT() {
    _timer.reset();
    _timer.start();
}

void srf05::_endT() {
    _timer.stop();
    _pulseDuration = _timer.read_us();
    _distance = _pulseDuration / 58.0;
    _newMeasurement = true;
}

void srf05::_ticker_cb() {
    _trig = 1;
    wait_us(10);
    _trig = 0;
}
