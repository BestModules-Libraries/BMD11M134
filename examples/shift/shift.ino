/*****************************************************************************************************
File:             shift.ino
Description:      Display characters and achieve left and right shift function .
Note:              
******************************************************************************************************/
#include "BMD11M134.h"

BMD11M134 Display(&Wire);
char cdata[100];
String sdata;

void setup()
{
  Serial.begin(9600);
  Display.begin();
}
 
void loop()
{
  sdata="BEST";
  sdata.toCharArray(cdata,5);
  Display.printStr(cdata);
  for(uint8_t i = 0; i < 6; i++)
  {
    delay(300);
    Display.shiftRight();
  }
  sdata="BEST";
  sdata.toCharArray(cdata,5);
  Display.printStr(cdata);
    for(uint8_t i = 0; i < 6; i++)
  {
    delay(300);
    Display.shiftLeft();
  }
}
