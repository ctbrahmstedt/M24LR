/*
  M24LR.c - Library for STMicroelectronics M24LR RFID/NFC Energy harvesting chip.
  Created by Colin Brahmstedt, July 8, 2018.
  Released into the public domain.
*/

//I had this thing working nicely with the UNO, but it borked when trying to use the Wire
//library for the ATtiny. This is an initial hack to get it working, definitely needs to be cleaned up.


#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__)
#include <TinyWireM.h>
#define lib TinyWireM
#define _i2c TinyWireM
void i2cwrite(uint8_t data){
  TinyWireM.send(data);
}
static uint8_t i2cread(void){
return TinyWireM.receive();
}

#elif defined(__AVR_ATmega328P__)
#include <Wire.h>
#define lib Wire
#define _i2c Wire
void i2cwrite(byte data){
  Wire.write(data);
}
static uint8_t i2cread(void){
return  Wire.read();
}
#endif
#include "Arduino.h"
#include "M24LR.h"


M24LR::M24LR(int M24LRaddress)
{
  _deviceaddress = M24LRaddress;
    _i2c.begin();
}

byte M24LR::read_byte(unsigned int eeaddress){

  byte rdata = 0xFF;
  _i2c.beginTransmission(_deviceaddress);
  i2cwrite((int)(eeaddress >> 8)); // MSB
  //return 0xFF;
  i2cwrite((int)(eeaddress & 0xFF)); // LSB
//return 0xFF;
      _i2c.endTransmission();

  _i2c.requestFrom(_deviceaddress,1);
  if (_i2c.available()) {
    rdata = i2cread();
    return rdata;
  }

}

void M24LR::write_byte(unsigned int eeaddress, byte data){
  int rdata = data;
  _i2c.beginTransmission(_deviceaddress);
  i2cwrite((int)(eeaddress >> 8)); // MSB
  i2cwrite((int)(eeaddress & 0xFF)); // LSB
  i2cwrite(rdata);
  _i2c.endTransmission();
  delay(50);
}

byte M24LR::write_page(unsigned int eeaddresspage, byte* data, byte length){
  unsigned int eeaddressblock = eeaddresspage*4;
  _i2c.beginTransmission(_deviceaddress);
  i2cwrite((int)(eeaddressblock >> 8)); // MSB
  i2cwrite((int)(eeaddressblock & 0xFF)); // LSB
  byte c;
  for ( c = 0; c < length; c++){
      i2cwrite(data[c]);
    //  i2cwrite(0x11);
    }
  //    i2cwrite(0x11);
  delay(50);
return( _i2c.endTransmission());

}

byte read_system_byte(unsigned int eeaddress){

  byte rdata = 0xFF;
  _i2c.beginTransmission(0x57);
  i2cwrite((int)(eeaddress >> 8)); // MSB
  //return 0xFF;
  i2cwrite((int)(eeaddress & 0xFF)); // LSB
//return 0xFF;
      _i2c.endTransmission();

  _i2c.requestFrom(0x57,1);
  if (_i2c.available()) {
    rdata = i2cread();
    return rdata;
  }
  //delay(50);
}

void write_system_byte(unsigned int eeaddress, byte data){
  int rdata = data;
  _i2c.beginTransmission(0x57);
  i2cwrite((int)(eeaddress >> 8)); // MSB
  i2cwrite((int)(eeaddress & 0xFF)); // LSB
  i2cwrite(rdata);
  _i2c.endTransmission();
  delay(50);
}

void M24LR::EH_enable()
{
    byte temp=read_system_byte(2336);
     write_system_byte(2336, temp|0x01);
}


void M24LR::EH_disable()
{
    byte temp=read_system_byte(2336);
     write_system_byte(2336, temp&0xFE);
}

void M24LR::EH_config(byte config)
{
     write_system_byte(2320, config);
}

boolean M24LR::FIELD_PRESENT(){
  //return(0);
    return((read_system_byte(2336)>>1)&0x01);
}

byte M24LR::NDEF(String ndef_msg){
  byte NDEF_text[]={0xE1, 0x40, 0x40, 0x05, 0x03, 0x00, 0xD1, 0x01, 0x00, 0x54, 0x02, 0x65, 0x6E};
  NDEF_text[5]= ndef_msg.length()+7;
  NDEF_text[8]= ndef_msg.length()+3;
//  write_page(0,NDEF_text, 13);
  for(int x=0; x<12; x++){
    write_byte(x, NDEF_text[x]);
  }
  for(int x=0; x<ndef_msg.length(); x++){
    write_byte(x+13, ndef_msg.charAt(x));
  }
  write_byte(ndef_msg.length()+13, 0xFE);
//NDEF_text[1]=0x01, 0x02;
//return ndef_msg.charAt(1);
//  return NDEF_text[1];
  return 0;
}
