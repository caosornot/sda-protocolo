#include "libreriasda.h"

LibreriaSDA::LibreriaSDA(){
    ;;
}

LibreriaSDA::LibreriaSDA(uint8_t Device_ID, uint8_t Device_PIN){
state = 1;
ID = Device_ID;
pin = Device_PIN;  
} 

uint16_t LibreriaSDA::doCommand(uint8_t cmd, uint16_t payload){
    byte payload_msb = byte(payload >> 8);
    byte payload_lsb = byte(payload);

        if (cmd == 0x01){
            int PWM = payload_lsb;
            analogWrite(pin,map(PWM,0,100,0,255));
            return payload;
        }
        if (cmd == 0x10){
            if(payload_msb== 0x10){
                return state;
            }            
            else if(payload_msb== 0x11){
                int val = analogRead(pin);
                return map(val,0,1023,0,100);
            }
        }   
    return 0xFF;
}

uint8_t LibreriaSDA::getID(){
    return ID;
}

uint8_t LibreriaSDA::getPin(){
    return pin;
}

bool LibreriaSDA::setState(bool st){
    state = st;
    return true;        
}
