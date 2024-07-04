/*	____________________________________________________________________________
  |																			    |
  | Module: I2C																    |
  |																			    |
  | File Name: i2c.c														    |
  |																			    |
  | Description: Source file for the I2C AVR driver				  		        |
  |																			    |
  | Author: Youssif Hossam													    |
  |_____________________________________________________________________________|
 */

#include "i2c.h"

#include "../../LIB/common_macros.h"
#include <avr/io.h>

/*______________________________________________________________________________
 |                               Functions Definitions                          |
 |______________________________________________________________________________|
 */

/*
 * Description :
 -> Initiate the I2C with its configurable baud rate and slave address
 */
void I2C_init(const I2C_ConfigType * Config_Ptr)
{
	/* Bit Rate: using zero pre-scaler TWPS=00 and F_CPU=8Mhz and configurable TWBR*/

	TWBR = Config_Ptr->bit_rate;
	TWSR = 0x00;

	/* Two Wire Bus address my address if any master device want to call me: 0x1 (used in case this MC is a slave device)
       General Call Recognition: Off */

	TWAR = ((Config_Ptr->address)<<1);

	TWCR = (1<<TWEN); /* enable I2C */
}

/*
 * Description :
 -> Function to send the start bit
 */
uint8 I2C_start(uint8 address)
{
	uint8_t status;		/* Declare variable */
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT); /* Enable TWI, generate START */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;		/* Read TWI status register */
	if(status!=0x08)		/* Check weather START transmitted or not? */
		return 0;			/* Return 0 to indicate start condition fail */
	TWDR=address;		/* Write SLA+W in TWI data register */
	TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI & clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;		/* Read TWI status register */
	if(status==0x18)		/* Check for SLA+W transmitted &ack received */
		return 1;			/* Return 1 to indicate ack received */
	if(status==0x20)		/* Check for SLA+W transmitted &nack received */
		return 2;			/* Return 2 to indicate nack received */
	else
		return 3;			/* Else return 3 to indicate SLA+W failed */
}

/*
 * Description :
 -> Function to send the stop bit
 */
void I2C_stop(void)
{
	/*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1 
	 */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

/*
 * Description :
 -> Function to send a single byte over I2C
 */
void I2C_writeByte(uint8 data)
{
	/* Put data On TWI data Register */
	TWDR = data;
	/*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */ 
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* Wait for TWINT flag set in TWCR Register(data is send successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*
 * Description :
 -> Function to read a single byte with acknowledgment
 */
uint8 I2C_readByteWithACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1 
	 */ 
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
	/* Read Data */
	return TWDR;
}

/*
 * Description :
 -> Function to read a single byte without acknowledgment
 */
uint8 I2C_readByteWithNACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
	/* Read Data */
	return TWDR;
}

/*
 * Description :
 -> Function to get the status of the communication
 */
uint8 I2C_getStatus(void)
{
	uint8 status;
	/* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	status = TWSR & 0xF8;
	return status;
}
