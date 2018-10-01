#include "protocolosda.h"

ProtocoloSDA::ProtocoloSDA(){
    cursor = 0;
}

bool ProtocoloSDA::packData(uint8_t ID, byte command, byte i_msb, byte i_lsb){
    if(cursor<76){
        buffer[cursor] = ID; cursor++;
        buffer[cursor] = command; cursor++;
        buffer[cursor] = i_msb; cursor++;
        buffer[cursor] = i_lsb; cursor++;
        return true;
    }
    return false;
}

bool ProtocoloSDA::packData(uint8_t ID, uint8_t command, uint16_t instruction){
    byte instruction_msb = byte(instruction >> 8);
    byte instruction_lsb = byte(instruction);
    return packData(ID, command, instruction_msb, instruction_lsb);
}

void ProtocoloSDA::clear(){
    for(int i=0; i<80; i++) {
        buffer[i] = 0;
    }
    cursor = 0;
}

int8_t ProtocoloSDA::calCheckSum(){
    byte sum = 0;
    for(int i=0;i<cursor; i++){
        sum+=buffer[i];
    }
    return byte(0xFF-sum);
}

void ProtocoloSDA::send(Stream& inp, uint8_t ori, uint8_t dest){
    inp.write(0x7E);
    inp.write(ori);
    inp.write(dest);
    inp.write(cursor);
    for(int i=0;i<cursor;i++){
        inp.write(buffer[i]);
    }
    inp.write(calCheckSum());
    clear();
}

bool ProtocoloSDA::available(Stream& inp){
    if(inp.available()){
        uint8_t header = inp.read();                        //Lectura primer byte del stream
        if(header == 0x7E){                                 //Condicional para verificar header correcto
            uint8_t head[3];                                //Creación y lectura de encabezado
            inp.readBytes(head, 3);
            uint8_t temp_origin = head[0];                
            uint8_t temp_destination = head[1];           
            uint8_t len = head[2];                          //Lectura de longitud recibida
            uint8_t temp_buffer[len+1];                     //Creación y almacenamiento de paquete de datos
            inp.readBytes(temp_buffer, len+1);
            uint8_t rec_checksum = temp_buffer[len];
            uint16_t sum = 0;
            for(int i=0;i<len; i++){                        //Proceso de calculo de Checksum
                sum += temp_buffer[i];
            }
            sum = 0xFF - byte(sum);
            if(rec_checksum == sum){                        //Condicional para verificar Checksum correcto
                origin = temp_origin;
                destination = temp_destination;
                for(int i=0; i<len; i++){
                    inp_buffer[i] = temp_buffer[i];
                }
                inp_data = len/4;                           //Calculo cantidad de paquetes recibidos
                return true;
            }
        }
    }
    return false;
}

uint16_t ProtocoloSDA::getCommand(uint8_t ID){                          
  for(int i = 0; i < inp_data*4 ; i+=4){                    //Recorrido buffer de datos recibidos verificando ID
    uint8_t ID_comp = inp_buffer[i];
    if(ID_comp == ID){
        if (inp_buffer[i+1] == 0x01){
            uint16_t send = (inp_buffer[i+1] << 8) | (inp_buffer[i+3]);
            return send;
        }
        if (inp_buffer[i+1] == 0x10){
            uint16_t send = (inp_buffer[i+1] << 8) | (inp_buffer[i+2]);
            return send;
        }       
    }
  }
  return 0xFFFF;
}

bool ProtocoloSDA::getData(uint8_t id, uint8_t& cmd, uint16_t& payload){
  for(int i = 0; i < inp_data*4 ; i+=4){
    uint8_t ID = inp_buffer[i];
    if(ID == id){
        cmd = inp_buffer[i+1];
        payload = (inp_buffer[i+2] << 8) | (inp_buffer[i+3]);
        return true;
    }
    return false;
  }
}
