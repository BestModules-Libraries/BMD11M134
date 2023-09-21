/*****************************************************************
File:          BMD11M134.cpp
Author:        BESTMODULES
Description:   I2C communicates with the module so that the module displays values or letters, etc.
Version:        V1.0.1   -- 2023-09-21
******************************************************************/
#include "BMD11M134.h"
/***************************************************************************************
Description: Constructor
Parameters:       *theWire : Wire object if your board has more than one I2C interface        
Return:          
Others:     
***************************************************************************************/
BMD11M134::BMD11M134(TwoWire *theWire)
{	
	_i2cPort = theWire; // Remember the user's setting
}
/**********************************************************
Description: Module Initial
Parameters:              
Return:      true or false.     
Others:       
**********************************************************/
void BMD11M134::begin()
{
	delay(500);
  _i2cPort->begin();
}

/********************************************************************
Description: Gets the number of modules
Parameters:         
Return:     Sum Of Modules
Others:         
*********************************************************************/
uint8_t BMD11M134::getNumber()
{
	uint8_t status;
	
	_tx_buf[3] = _CMD_CHECK_MODULE;
   delay(3);
   writeData(1, 5, _tx_buf);
   delay(5);
   if(readBytes(6, _rx_buf))
   {
    status=_rx_buf[3];
    if(status==0)
    {
      _SumOfModules = _rx_buf[4];
      return _rx_buf[4];
    }
    else
    {
      return 0;
    }
   }
    else
   {
    return 0;
   }
}
/*-------------------------- Display configuration functions ---------------------------*/
/********************************************************************
Description: Turn off all segments of all displays connected to bus
Parameters:               
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE    
Others:         
*********************************************************************/
uint8_t BMD11M134::clear(void)
{
	// Clear the _displayArray
	for (uint8_t i = 0; i < 5 * _SumOfModules; i++)
	{
		_displayArray[i] = 0;
	}
	return(updateDisplay());
}
/****************************************************************
Description: Turn the decimal point on for a single display
Parameters:       displayNumber: Module Number        
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE.    
Others:         
****************************************************************/
uint8_t BMD11M134::decimalOnSingle(uint8_t displayNumber)
{
	return setDecimalOnOff(displayNumber, true);
}
/***************************************************************
Description: Turn the decimal point off for a single display
Parameters:       displayNumber: Module Number       
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE   
Others:         
***************************************************************/
uint8_t BMD11M134::decimalOffSingle(uint8_t displayNumber)
{
	return setDecimalOnOff(displayNumber, false);
}

/**********************************************************
Description: Turn the decimal on for all displays on bus
Parameters:              
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE    
Others:         
**********************************************************/
uint8_t BMD11M134::decimalOn()
{
	uint8_t status;	
	for (uint8_t i = 1; i <= _SumOfModules; i++)
	{
		status = decimalOnSingle(i);
		if(status != 0)
		{
			return status;
		}
	}	
	return status;	
}
/*****************************************************************
Description: Turn the decimal point off for all displays on bus
Parameters:               
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE    
Others:         
*****************************************************************/
uint8_t BMD11M134::decimalOff()
{
	uint8_t status;	
	for (uint8_t i = 1; i <= _SumOfModules; i++)
	{
		status = decimalOffSingle(i);
		if(status != 0)
		{
			return status;
		}
	}	
	return status;	
}
/**********************************************************
Description: Turn the colon on for a single display
Parameters:       displayNumber: Module Number         
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE   
Others:         
**********************************************************/
uint8_t BMD11M134::colonOnSingle(uint8_t displayNumber)
{
	return setColonOnOff(displayNumber, true);
}
/**********************************************************
Description: Turn the colon off for a single display
Parameters:       displayNumber: Module Number        
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE    
Others:         
**********************************************************/
uint8_t BMD11M134::colonOffSingle(uint8_t displayNumber)
{
	return setColonOnOff(displayNumber, false);
}

/**************************************************************
Description: Turn the colon on for all displays on the bus
Parameters:               
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE   
Others:         
***************************************************************/
uint8_t BMD11M134::colonOn()
{
	uint8_t status;
	
	for (uint8_t i = 1; i <= _SumOfModules; i++)
	{
		status = colonOnSingle(i);
		if(status != 0)
		{
			return status;
		}
	}
	
	return status;
}
/***************************************************************
Description: Turn the colon off for all displays on the bus
Parameters:              
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE    
Others:         
***************************************************************/
uint8_t BMD11M134::colonOff()
{
	uint8_t status;
	
	for (uint8_t i = 1; i <= _SumOfModules; i++)
	{
		status = colonOffSingle(i);
		if(status != 0)
		{
			return status;
		}
	}
	
	return status;
}
/**********************************************************
Description: The character shifts to the right.
Parameters:       shiftAmt: The number of bits per move         
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE    
Others:         
**********************************************************/
uint8_t BMD11M134::shiftRight(uint8_t shiftAmt)
{
	for (uint8_t x = (5 * _SumOfModules) - shiftAmt; x >= shiftAmt; x--)
	{
		_displayContent[x] = _displayContent[x - shiftAmt];
	}
	// Clear the leading characters
	for (uint8_t x = 0; x < shiftAmt; x++)
	{
		if (x + shiftAmt > (5 * _SumOfModules))
		{
			break; // Error check
		}
		_displayContent[0 + x] = ' ';
	}
	return (printStr(_displayContent));	
}
/**********************************************************
Description: The character shifts to the left.
Parameters:       shiftAmt: The number of bits per move        
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE    
Others:         
**********************************************************/
uint8_t BMD11M134::shiftLeft(uint8_t shiftAmt)
{
	uint8_t Len;
	Len = strlen(_displayContent);
	for (int x = 0; x < Len; x++)
	{
		if (x + shiftAmt > Len)
		{
			break; // Error check
		}
		_displayContent[x] = _displayContent[x + shiftAmt];
	}
	// Clear the trailing characters
	for (int x = 0; x < shiftAmt; x++)
	{
		if (Len - 1 - x < 0)
		{
			break; //Error check
		}
		_displayContent[Len - 1 - x] = '\0';
	}
	return (printStr(_displayContent));
}
/*---------------------------- Light up functions ---------------------------------*/

/******************************************************************
Description: Print a character, for a given digit, on display
Parameters:       displayNumber: Module Number
             displayChar: Displayed letters
             digit: The bit number of a digit        
Return:    
Others:         
*******************************************************************/
void BMD11M134::printChar(uint8_t displayNumber, uint8_t displayChar, uint8_t digit)
{
	// moved alphanumeric_segs array to PROGMEM
	uint16_t characterPosition = 65532;

	// space
	if ((displayChar == ' ') || (displayChar == '\0'))
	{
		characterPosition = 0;
	}
	// Printable Symbols -- Between first character ! and last character ~
	else if (displayChar >= '!' && displayChar <= '~')
	{
		characterPosition = displayChar - '!' + 1;
	}
	
	if (characterPosition == 14) // '.'
	{
		_displayArray[4 + (displayNumber - 1) * 5] &=  0x01;
		_displayArray[4 + (displayNumber - 1) * 5] |= 0x02;
	}
	else if (characterPosition == 26) // ':'
	{
		_displayArray[4 + (displayNumber - 1) * 5] &=  0x02;
		_displayArray[4 + (displayNumber - 1) * 5] |= 0x01;
	}
	else if(characterPosition == 65532) // unknown character
	{
		characterPosition = UNKNOWN_CHAR;
	}
	
	_displayArray[(displayNumber - 1) * 5 + digit] = characterPosition;	
	
}
/**********************************************************
Description: Update the display
Parameters:              
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
       0x80: INSTRUCTION_NOT_SUPPOR
       0xA0: SLAVE_NO_RESPONSE   
Others:         
**********************************************************/
uint8_t BMD11M134::updateDisplay()
{
  uint8_t status;
  
  _tx_buf[3] = _CMD_UPDATE_DISPLAY;
  for(uint8_t id = 1; id <= _SumOfModules; id++)
  {
    for(uint8_t i = 0; i < 5; i++)
    {
      _tx_buf[i + 4] = _displayArray[(id - 1) * 5 + i];
    }
    
    delay(3);
    writeData(id, 10, _tx_buf);   //write module
    delay(5);
    readBytes(5, _rx_buf);            //read ack
  }
  
  status = _rx_buf[3];
  return status;
}
/**********************************************************
Description: Print string
Parameters:       buffer[]: Displays an array of characters        
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE   
Others:         
**********************************************************/
uint8_t BMD11M134::printStr(char buffer[])
{
	uint8_t i = 0;
	for(i = 0; i < strlen(buffer); i++)
	{
		_displayContent[i] = buffer[i];
	}
	i = 0;
	for(uint8_t id = 1; id <= _SumOfModules; id++)
	{
		for(uint8_t digit = 0; digit < 4; digit++)
	    {
			if(i < strlen(buffer))
			{
				if((buffer[i] == '.') || (buffer[i] == ':'))
			    {
				    while( (i < strlen(buffer)) && ((buffer[i] == '.') || (buffer[i] == ':')))
			        {
						printChar(id , buffer[i], digit);
					    i++;
			        }
				}
				printChar(id, buffer[i], digit);
				i++;
			}
			else
			{
				printChar(id, '\0', digit);
			}				
		}			
	}
	
    return updateDisplay();	
}
/**********************************************************
Description: Set brightness
Parameters:  displayNumber: Module Number 
             bightness:0~2;	 
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE   
Others:         
**********************************************************/
uint8_t BMD11M134::setBrightness(uint8_t displayNumber, uint8_t brightness)
{
  uint8_t status;
  
  _tx_buf[3] = _CMD_SET_BRIGHTNESS;
  _tx_buf[4] = brightness;
  
  delay(3);
  writeData(displayNumber, 6, _tx_buf);   //write module
  delay(5);
  readBytes(5, _rx_buf);        //read ack
  
  status = _rx_buf[3];
  return status;	
}
/**********************************************************
Description: Obtain the firmware version number
Parameters:  displayNumber: Module Number 	 
Return:   Module version number     
Others:         
**********************************************************/
uint16_t BMD11M134::getFWVer(uint8_t displayNumber)
{
  uint8_t status;
  uint16_t FWVer = 0;
  _tx_buf[3] = _CMD_GET_FWVER;
  
  delay(3);
  writeData(displayNumber, 5, _tx_buf);   //write module
  delay(5);
  readBytes(7, _rx_buf);        //read ack
  status = _rx_buf[3];
  if(status==0)
  {
    FWVer = (_rx_buf[4]<<8) + _rx_buf[5];
  }
  else
  {
    FWVer = 0;
  }
  return FWVer;	
}
/**********************************************************
Description: Set or clear the decimal on/off bit
Parameters:       displayNumber: Module Number
             turnOnDecimal:true or false.       
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
       0x80: INSTRUCTION_NOT_SUPPOR
       0xA0: SLAVE_NO_RESPONSE  
Others:         
**********************************************************/
uint8_t BMD11M134::setDecimalOnOff(uint8_t displayNumber, bool turnOnDecimal)
{
  uint8_t dat;
  if (turnOnDecimal == true)
  {
    dat = 0x02;
  }
  else
  {
    dat = 0x00;
  }
  _displayArray[4 + (displayNumber - 1) * 5] = (_displayArray[4 + (displayNumber - 1) * 5] & 0x01) | dat;
  return (updateDisplay());
}
/**********************************************************
Description: Set or clear the colon on/off bit
Parameters:       displayNumber: Module Number
             turnOnColon:true or false.         
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
       0x80: INSTRUCTION_NOT_SUPPOR
       0xA0: SLAVE_NO_RESPONSE    
Others:         
**********************************************************/
uint8_t BMD11M134::setColonOnOff(uint8_t displayNumber, bool turnOnColon)
{
  uint8_t dat;
  if (turnOnColon == true)
  {
    dat = 0x01;
  }
  else
  {
    dat = 0x00;
  }
  _displayArray[4 + (displayNumber - 1) * 5] = (_displayArray[4 + (displayNumber - 1) * 5] & 0x02) | dat;
  return (updateDisplay());
}
/*----------------------- Internal I2C Abstraction -----------------------------*/
/*******************************************************************
Description: Write data 
Parameters:  id: The module number
             len:Length of data to be written
			       par[]: Write to an array of data        
Return:   
Others:         
*******************************************************************/
void BMD11M134::writeData(uint8_t id, uint8_t len, uint8_t par[])
{  /* Array frame format：
    MID |ID | LEN | CMD/STATUS | DATA0~n | CHECHSUM*/
	par[0] = MODULE_MID;
	par[1] = id;
	par[2] = len - 3;
	par[len - 1] = 0;
	
	for(uint8_t i = 0; i < len - 1; i++)
	{
		par[len - 1] += par[i];
	}
  writeBytes(len, par);
}
/*******************************************************************
Description: writeBytes.
Parameters:
             wlen:Length of data to be written
             wbuf[]: Write to an array of data        
Return:    
Others:         
*******************************************************************/
void BMD11M134::writeBytes(uint8_t wlen, uint8_t wbuf[])
{    
  _i2cPort->beginTransmission(DEFAULT_ADDRESS);
  
  for (uint8_t i = 0; i < wlen; i++)
  {
    _i2cPort->write(wbuf[i]);
  }
  
  _i2cPort->endTransmission();

}
/**********************************************************
Description: readBytes.
Parameters:       rlen: The length of the data read
			            rbuf[]: Store the read data        
Return:      true or false.    
Others:         
**********************************************************/
bool BMD11M134::readBytes(int rlen, uint8_t rbuf[])
{
  uint8_t i = 0, checkSum = 0;
  _i2cPort->requestFrom(DEFAULT_ADDRESS, rlen);
  if (_i2cPort->available() == rlen)
  {
    for (i = 0; i < rlen; i++)
    {
      rbuf[i] = _i2cPort->read();
    }
  }
  else
  {
    return false;
  }
  /* Check Sum */
  for (i = 0; i < (rlen - 1); i++)
  {
    checkSum += rbuf[i];
  }
  if (checkSum == rbuf[rlen - 1])
  {
    return true; // Check correct
  }
  else
  {
    return false; // Check error
  }
}
