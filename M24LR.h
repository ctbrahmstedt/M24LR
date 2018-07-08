/*
  M24LR.h - Library for STMicroelectronics M24LR RFID/NFC Energy harvesting chip.
  Created by Colin Brahmstedt, July 8, 2018.
  Released into the public domain.
*/
#ifndef M24LR_h
#define M24LR_h

#if defined(__AVR_ATtiny45__) || defined(__AVR_ATmega2560__)
#include <TinyWireM.h>
#elif defined(__AVR_ATmega328P__)
#include <Wire.h>
#endif

#include "Arduino.h"
//#include <Wire.h>
class M24LR
{
public:
  M24LR(int M24LRaddress);
  byte read_byte(unsigned int eeaddress );
  void write_byte(unsigned int eeaddress, byte data);
  byte write_page(unsigned int eeaddresspage, byte* data, byte length);
  byte write_cont(unsigned int eeaddresspage, byte* data, byte length);

  void EH_enable();
  void EH_disable();
  void EH_config(byte eh_config); // bXXXXX[EH][mode1][mode2] EH set to 1 for EH enabled on power up
  boolean FIELD_PRESENT();
//  byte NDEF_text[];
  byte NDEF(String ndef_msg);
private:
  int _deviceaddress;
  //string NDEF_text[]={0xE1, 0x40, 0x40, 0x05, 0x03, 0x00, 0xD1, 0x01, 0x00, 0x54, 0x02, 0x65, 0x6E};
//  void write_system_byte(unsigned int eeaddress, byte data);
};
#endif
