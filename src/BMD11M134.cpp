/*****************************************************************
File:          BMD11M134.cpp
Author:        BEST MODULES CORP.
Description:   IIC communication with BMD11M134, let BMD11M134 display numbers, characters, etc
Version:       V1.0.2   -- 2024-08-30
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
Parameters:  void            
Return:      void     
Others:       
**********************************************************/
void BMD11M134::begin()
{
	delay(500);
  _i2cPort->begin();
  _SumOfModules = getNumber();
}

/********************************************************************
Description: Obtain the number of expansion modules
Parameters:  void       
Return:      Number of expansion modules
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

/********************************************************************
Description: Clear display
Parameters:  void             
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			       0x80: INSTRUCTION_NOT_SUPPOR
			       0xA0: SLAVE_NO_RESPONSE    
Others:         
*********************************************************************/
uint8_t BMD11M134::clear()
{
	// Clear the _displayArray
	for (uint8_t i = 0; i < 5 * _SumOfModules; i++)
	{
		_displayArray[i] = 0;
	}
	return(updateDisplay());
}

/****************************************************************
Description: Displays the decimal point on the specified nixie
Parameters:  displayNumber: Module Number        
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
Description: The decimal point on the specified nixie is not displayed
Parameters:  displayNumber: Module Number       
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
Description: Displays decimal points on all nixie tubes
Parameters:  void            
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
Description: Do not display decimal points on all nixie tubes
Parameters:  void            
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
Description: Displays the colon on the specified digit tube
Parameters:  displayNumber: Module Number         
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
Description: Does not display colons on the specified digit tube
Parameters:  displayNumber: Module Number        
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
Description: Displays colons on all nibs
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
Description: Do not Displays colons on all nibs
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
Description: Display content moved right
Parameters:  shiftAmt: The number of bits per move         
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			       0x80: INSTRUCTION_NOT_SUPPOR
			       0xA0: SLAVE_NO_RESPONSE    
Others:         
**********************************************************/
uint8_t BMD11M134::shiftRight(uint8_t shiftAmt)
{
	volatile uint8_t Len;
	Len = strlen(_displayContent);
	for (uint8_t x = (5 * _SumOfModules) - shiftAmt; x >= shiftAmt; x--)
	{
		if(x != Len)
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
Description: Display content moved left.
Parameters:  shiftAmt: The number of bits per move        
Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			       0x80: INSTRUCTION_NOT_SUPPOR
		       	 0xA0: SLAVE_NO_RESPONSE    
Others:         
**********************************************************/
uint8_t BMD11M134::shiftLeft(uint8_t shiftAmt)
{
	volatile uint8_t Len;
	Len = strlen(_displayContent);
//	_displayContent[Len] = '\0';
	for (uint8_t x = 0; x < Len; x++)
	{
		if (x + shiftAmt > Len)
		{
			break; // Error check
		}
		if(x != Len)
			_displayContent[x] = _displayContent[x + shiftAmt];
	}
	// Clear the trailing characters
	for (uint8_t x = 0; x < shiftAmt; x++)
	{
		if (Len - 1 - x < 0)
		{
			break; //Error check
		}
		_displayContent[Len - 1 - x] = '\0';
	}
	return (printStr(_displayContent));
}


/******************************************************************
Description: Specifies the digit display character for the module
Parameters:  displayNumber: Module Number
             displayChar: character
             digit: Digit number     
Return:      void
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
Parameters:  void            
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
Description: Display string
Parameters:  buffer[]: Character array 
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
             bightness:Brightness range: 0~2
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
Description: Get version number
Parameters:  displayNumber: Module Number 	 
Return:      version number     
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
Description: Select whether the nixie decimal point is displayed
Parameters:  displayNumber: Module Number
             turnOnDecimal:
                         true: Display decimal point
                         false: The decimal point is not displayed     
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
Description: Select whether the colon on the digitube is displayed
Parameters:  displayNumber: Module Number
             turnOnDecimal:
                         true: Displays colons on nibs
                         false: Does not display colons on nibs   
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

/*******************************************************************
Description: Write data 
Parameters:  id: The module number
             len:Length of data to be written
			       par[]: Write to an array of data        
Return:      void
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
Return:      void  
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
Parameters:  rlen: The length of the data read
			       rbuf[]: Store the read data        
Return:      true: Obtaining data successfully 
             false: Data acquisition failure   
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
