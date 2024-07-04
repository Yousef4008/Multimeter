/*	____________________________________________________________________________
  |																			    |
  | Driver: Inductance Meter												    |
  |																			    |
  | File Name: inductance_meter.h											    |
  |																			    |
  | Description: Header file for inductance meter driver					    |
  |																			    |
  | Author: Youssif Hossam													    |
  |_____________________________________________________________________________|
 */

#ifndef HAL_INDUCTANCE_METER_INDUCTANCE_METER_H_
#define HAL_INDUCTANCE_METER_INDUCTANCE_METER_H_

#include "../../MCAL/GPIO/gpio.h"
#include "../../MCAL/TIMER1/timer1.h"
#include "../../LIB/std_types.h"

/*______________________________________________________________________________
 |                               Definitions                                    |
 |______________________________________________________________________________|
 */

#define INDUCTANCE_METER_CHARGING_PORT_ID               PORTD_ID
#define INDUCTANCE_METER_CHARGING_PIN_ID                PIN0_ID

#define INDUCTANCE_METER_COMPARATOR_PORT_ID             PORTA_ID
#define INDUCTANCE_METER_COMPARATOR_PIN_ID              PIN3_ID

#define INDUCTANCE_METER_CAPACITOR_MICROFARAD_VALUE     2
#define F_CPU_MEGA_HZ                                   (F_CPU / 1000000)

/*______________________________________________________________________________
 |                               Functions Prototypes                           |
 |______________________________________________________________________________|
 */

/*
 * Description :
 * -> Function responsible for getting the inductance reading.
 * -> This function measures the inductance by charging the inductor, measuring the
 *    oscillation period, and calculating the inductance based on the period.
 */
float INDUCTANCE_METER_getReading();

#endif /* HAL_INDUCTANCE_METER_INDUCTANCE_METER_H_ */
