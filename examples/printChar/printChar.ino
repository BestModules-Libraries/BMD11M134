/*****************************************************************************************************
File:             printChar.ino
Description:      This example tests illuminating whole 4 char strings on the 14-segment display.
Note:              
******************************************************************************************************/
#include "BMD11M134.h"

BMD11M134 Display(&Wire);

void setup()
{
  Serial.begin(9600);
  Display.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Display.printChar(1, 'H', 0);
  Display.printChar(1, 'O', 1);
  Display.printChar(1, 'L', 2);
  Display.printChar(1, 'T', 3);
  Display.printChar(2, 'B', 0);
  Display.printChar(2, 'E', 1);
  Display.printChar(2, 'S', 2);
  Display.printChar(2, 'T', 3);  
  Display.updateDisplay();
}
