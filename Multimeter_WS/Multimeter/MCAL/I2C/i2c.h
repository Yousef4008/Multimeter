 /*	____________________________________________________________________________
  |																			    |
  | Module: I2C																    |
  |																			    |
  | File Name: i2c.h														    |
  |																			    |
  | Description: Header file for the I2C AVR driver				  		        |
  |																			    |
  | Author: Youssif Hossam													    |
  |_____________________________________________________________________________|
*/


#ifndef TWI_H_
#define TWI_H_

#include "../../LIB/std_types.h"

/*______________________________________________________________________________
 |                       	   Preprocessor Macros                   		   	|
 |______________________________________________________________________________|
 */

/* I2C Status Bits in the TWSR Register */
#define I2C_START         0x08 /* start has been sent */
#define I2C_REP_START     0x10 /* repeated start */
#define I2C_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define I2C_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define I2C_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define I2C_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define I2C_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*______________________________________________________________________________
 |                       	   Types Definations                   		     	|
 |______________________________________________________________________________|
 */
typedef enum { 	BAUDRATE_500KHZ = 0	 , BAUDRATE_400KHZ = 2  , BAUDRATE_250KHZ = 8 ,
				BAUDRATE_200KHZ = 12 , BAUDRATE_160KHZ = 17 , BAUDRATE_125KHZ = 24
			 }I2C_BaudRate;

typedef struct {
	uint8 address;
	I2C_BaudRate bit_rate;
}I2C_ConfigType;

/*______________________________________________________________________________
 |                               Functions Prototypes                           |
 |______________________________________________________________________________|
 */
void I2C_init(const I2C_ConfigType * Config_Ptr);
uint8 I2C_start(uint8 address);
void I2C_stop(void);
void I2C_writeByte(uint8 data);
uint8 I2C_readByteWithACK(void);
uint8 I2C_readByteWithNACK(void);
uint8 I2C_getStatus(void);


#endif /* TWI_H_ */
