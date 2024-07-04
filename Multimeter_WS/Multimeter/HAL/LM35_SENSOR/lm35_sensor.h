 /*	______________________________________________________________________________
   |																			  |
   | Module: LM35																  |
   |																			  |
   | File Name: lm35_sensor.h													  |
   |																			  |
   | Description: Header file for the AVR LM35 SENSOR driver					  |
   |																			  |
   | Author: Youssif Hossam														  |
   |______________________________________________________________________________|
   */

#ifndef LM35_SENSOR_H_
#define LM35_SENSOR_H_

#include "../../LIB/std_types.h"

/*______________________________________________________________________________
 |                               		Definitions                        		|
 |______________________________________________________________________________|
 */

#define SENSOR_CHANNEL_ID         2
#define SENSOR_MAX_VOLT_VALUE     1.5
#define SENSOR_MAX_TEMPERATURE    150

/*______________________________________________________________________________
 |                               Functions Prototypes                           |
 |______________________________________________________________________________|
 */

/*
 * Description :
 -> Function responsible for calculate the temperature from the ADC digital value.
 */
uint8 LM35_getTemperature(void);

#endif /* LM35_SENSOR_H_ */
