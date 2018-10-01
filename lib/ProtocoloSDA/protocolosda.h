#ifndef protocolosda_h
#define protocolosda_h

#define SET 0x01
#define GET 0x10
#define ACK 0X20
#define RES 0x21

#include <Arduino.h>

class ProtocoloSDA{
public:
  ProtocoloSDA();
  bool packData(uint8_t ID, uint8_t command, byte i_msb, byte i_lsb);
  bool packData(uint8_t ID, uint8_t command, uint16_t instruction);
  void clear();
  void send(Stream& inp, uint8_t ori, uint8_t dest);
  bool available(Stream& inp);
  uint16_t getCommand(uint8_t ID);
  bool getData(uint8_t id, uint8_t& cmd, uint16_t& payload);
  uint8_t origin;
  uint8_t destination;
   
private:
  int8_t calCheckSum();
  uint8_t buffer[80];
  uint8_t cursor;
  uint8_t inp_buffer[80];
  uint8_t inp_data;
};

#endif