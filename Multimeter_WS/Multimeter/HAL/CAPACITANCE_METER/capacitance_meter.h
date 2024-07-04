/*	____________________________________________________________________________
  |																			    |
  | Driver: Capacitance Meter												    |
  |																			    |
  | File Name: capacitance_meter.h											    |
  |																			    |
  | Description: Header file for capacitance meter driver					    |
  |																			    |
  | Author: Youssif Hossam													    |
  |_____________________________________________________________________________|
*/

#ifndef HAL_CAPACITANCE_METER_CAPACITANCE_METER_H_
#define HAL_CAPACITANCE_METER_CAPACITANCE_METER_H_

#include "../../MCAL/ADC/adc.h"
#include "../../MCAL/GPIO/gpio.h"
#include "../../MCAL/TIMER1/timer1.h"
#include "../../LIB/std_types.h"

/*______________________________________________________________________________
 |                               Definitions                                    |
 |______________________________________________________________________________|
 */

#define CAPACITANCE_METER_CHARGING_RES_1_PORT_ID		PORTB_ID
#define CAPACITANCE_METER_CHARGING_RES_1_PIN_ID			PIN4_ID

#define CAPACITANCE_METER_CHARGING_RES_2_PORT_ID		PORTB_ID
#define CAPACITANCE_METER_CHARGING_RES_2_PIN_ID			PIN6_ID

#define CAPACITANCE_METER_DISCHARGING_RES_PORT_ID		PORTB_ID
#define CAPACITANCE_METER_DISCHARGING_RES_PIN_ID		PIN5_ID

#define CAPACITANCE_METER_POSITIVE_CAP_PORT_ID			PORTA_ID
#define CAPACITANCE_METER_POSITIVE_CAP_PIN_ID			PIN1_ID

#define CAPACITANCE_METER_NEGATIVE_CAP_PORT_ID			PORTA_ID
#define CAPACITANCE_METER_NEGATIVE_CAP_PIN_ID			PIN2_ID

#define CAPACITANCE_METER_ADC_MAX_VALUE					1023

/*______________________________________________________________________________
 |                               Types Definitions                              |
 |______________________________________________________________________________|
 */

typedef enum {
	PICO_FARAD, NANO_FARAD, MICRO_FARAD, MILLI_FARAD, FARAD, NO_SCALE
} CAPACITANCE_unitType;

/*______________________________________________________________________________
 |                               Functions Prototypes                           |
 |______________________________________________________________________________|
 */

/*
 * Description :
 * -> Function responsible for getting the capacitance reading.
 */
float CAPACITANCE_METER_getReading(CAPACITANCE_unitType * capUnit);

#endif /* HAL_CAPACITANCE_METER_CAPACITANCE_METER_H_ */