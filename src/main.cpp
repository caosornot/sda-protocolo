#include <Arduino.h>
#include <ProtocoloSDA.h>
#include <LibreriaSDA.h>

ProtocoloSDA PortTest;

LibreriaSDA ACTUADOR1 = LibreriaSDA(10,2);
LibreriaSDA SENSOR1 = LibreriaSDA(20,A0);

LibreriaSDA devices[2] = {ACTUADOR1, SENSOR1};

void setup() {
    Serial.begin(9600);
}

void loop() {
    if(PortTest.available(Serial)){

        uint8_t cmd = 0;
        uint16_t payload = 0;
        uint16_t ans = 0;

        for(int i=0; i<2; i++){
            if(PortTest.getData(devices[i].getID(), cmd, payload)){
                ans = devices[i].doCommand(cmd, payload);
                if (cmd == SET){
                    PortTest.packData(devices[i].getID(), ACK, ans);
                }
                else if (cmd == GET){
                    PortTest.packData(devices[i].getID(), RES, ans);
                }       
            }
        }
        PortTest.send(Serial, PortTest.destination, PortTest.origin);
    }
}  
