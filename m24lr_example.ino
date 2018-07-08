#include "M24LR.h"
#include <Wire.h>
//#include <TinyWireM.h> //Working on getting TinyWireM.h working for ATTiny45. Compiles and should work, but nothing is coming out of the bus. May need pullup resistors on the I2C lines. 
M24LR m24lr(0x53);  //setup your shiz. 0x53 is user memory address. 0x57 accesses system memory, tread carefully.
byte datas[4]; //random data array

void setup()
{

  Serial.begin(9600);
  Serial.println("M24LR example");
  m24lr.EH_enable(); //will temporarily enable/disable power. upon reboot, the EH state will return to it's default setting in the config below. 
  //m24lr.EH_disable();
  m24lr.EH_config(0xF3); //enable energy harvest pin output. BXXXXX[a][b][c] a: 0=EH enabled at power up/1 is disabled.  [b][c] = output current setting 00/01/10/11 -> 6mA, 3mA, 1mA, 300uA  The higher the current setting, the more RF power is needed to bootup. 
  // eg 0xF3 = default EH on, and in 300uA power state. 0xF4 = default EH off, in 6mA power config. 
  //EH_config setting does not need to be called at every setup. When called, the data is written into the M24LR EEPROM and will be stored even without power. 

}

void loop(){ 

  m24lr.NDEF("Max is dumb");  //write NDEF text message

  Serial.println(m24lr.read_byte(0x00), HEX); //read byte somewhere in memory, display it in HEX.  
  m24lr.write_byte(32, 0xA0);  //write byte somewhere in memory

  if (m24lr.FIELD_PRESENT())
    Serial.println("RF Field Present!");
  else
    Serial.println("RF Field NOT Present!"); 
  delay(50);  //let the I2C bus settle down, the M24LR can give erroneous values if you try to R/W too fast
  byte datas[4];
  datas[0]=0x30;
  datas[1]=0x31;
  datas[2]=0x32;
  datas[3]=0x33;
  m24lr.write_page(7, (byte *)datas, 4); //write 4 bytes to a page in memory. Do not write more than 4, otherwise the data block will roll over and write over itself. 
  delay(50); //let the I2C bus settle down
  for(int x=0; x<36; x++){
    byte b = m24lr.read_byte(x);
    Serial.print(b, HEX);
    Serial.print(" ");

  }
  Serial.println(" ");

  delay(1000);  //When writing to the M24LR, you are writing to EEPROM, not RAM. The EEPROM has limited write endurance, so best to not freewheel your sketches. 
}


