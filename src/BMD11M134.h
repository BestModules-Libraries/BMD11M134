/*************************************************
File:       	    BM_Alphanumeric_Display.h
Author:            	BESTMODULE
Description:        Define classes and required variables
History：			
V1.0.1	 -- initial version；2023-04-14；Arduino IDE : v1.8.16
**************************************************/
#ifndef _BMD11M134_H
#define _BMD11M134_H

#include <Arduino.h>
#include <Wire.h>
#include <stdlib.h>

#define DEFAULT_ADDRESS         0x2c 
#define MODULE_MID              0x2c
class BMD11M134
{
	public:
	  BMD11M134(TwoWire *theWire = &Wire);
    void begin();
		uint8_t getNumber();
        //Display configuration functions
    uint8_t clear();
        // Decimal functions
    uint8_t decimalOnSingle(uint8_t displayNumber);
    uint8_t decimalOffSingle(uint8_t displayNumber);
    uint8_t decimalOn();
    uint8_t decimalOff();
        // Colon functions
    uint8_t colonOnSingle(uint8_t displayNumber);
    uint8_t colonOffSingle(uint8_t displayNumber);        
    uint8_t colonOn();
    uint8_t colonOff();
        // Shifting
    uint8_t shiftRight(uint8_t shiftAmt = 1);
    uint8_t shiftLeft(uint8_t shiftAmt = 1);    
        // Light up function
    void printChar(uint8_t displayNumber, uint8_t displayChar, uint8_t digit);
    uint8_t updateDisplay();
    uint8_t printStr(char buffer[]);




	private:
	    //CMD
		const uint8_t _CMD_CHECK_MODULE = 0x01;
    const uint8_t _CMD_UPDATE_DISPLAY = 0x02;
		//
		const uint8_t COLON = 0x01;
		const uint8_t DECIMAL = 0x02;
		const uint8_t UNKNOWN_CHAR = 95;
		TwoWire *_i2cPort = NULL;             //The generic connection to user's chosen I2C hardware
    uint8_t _SumOfModules = 1;
    uint8_t _displayArray[4 * 5] = {0};
		char _displayContent[4 * 5] = {0};
		uint8_t _tx_buf[10] = {0};
		uint8_t _rx_buf[10] = {0};

		uint8_t setDecimalOnOff(uint8_t displayNumber, bool turnOnDecimal);
		uint8_t setColonOnOff(uint8_t displayNumber, bool turnOnColon);
    void writeData(uint8_t id, uint8_t len, uint8_t par[]); 
    void writeBytes(uint8_t wlen, uint8_t wbuf[]);		   
    bool readBytes(int len, uint8_t buffer[]);
};
#endif
