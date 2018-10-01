#ifndef libreriasda_h
#define libreriasda_h

#include <Arduino.h>

class LibreriaSDA {
    public:
        LibreriaSDA();
        LibreriaSDA(uint8_t Device_ID, uint8_t Device_PIN);            //Constructor ingresar  ID (00-99), tipo 0-Actuador 1-Sensor, numero pin
        uint16_t doCommand(uint8_t cmd, uint16_t payload);
        uint16_t getVal();
        uint8_t getID();
        uint8_t getPin();
        bool setState(bool st);

    private:
        uint8_t state;
        uint8_t pos;
        uint8_t ID;
        uint8_t pin;   
};

#endif