/*****************************************************************************************************
File:             printString.ino
Description:      This example tests illuminating whole 4 letter strings on the 14-segment display.
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
  Serial.print(Display.getNumber());
  Serial.print(" modules are ");
  Serial.println("Connected!");
  sdata="HOLTBEST";
  sdata.toCharArray(cdata,9);
  Display.printStr(cdata);
 ;
}
 
void loop()
{
  
}
