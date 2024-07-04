/*
 * voltmeter.h
 *
 *  Created on: Jun 30, 2024
 *      Author: Youssif Hossam
 */

#ifndef HAL_OHMMETER_H_
#define HAL_OHMMETER_H_

#include "../../MCAL/GPIO/gpio.h"
#include "../../HAL/ADS1015/ads1015.h"
#include "../../LIB/std_types.h"

#define OHMMETER_RES_1_VALUE		200
#define OHMMETER_RES_2_VALUE		2
#define OHMMETER_RES_3_VALUE		20
#define OHMMETER_RES_4_VALUE		470

#define OHMMETER_RES_1_PORT_ID		PORTB_ID
#define OHMMETER_RES_1_PIN_ID		PIN0_ID

#define OHMMETER_RES_2_PORT_ID		PORTB_ID
#define OHMMETER_RES_2_PIN_ID		PIN1_ID

#define OHMMETER_RES_3_PORT_ID		PORTB_ID
#define OHMMETER_RES_3_PIN_ID		PIN2_ID

#define OHMMETER_RES_4_PORT_ID		PORTB_ID
#define OHMMETER_RES_4_PIN_ID		PIN3_ID

#define OHMMETER_ADC_CHANNEL		2

typedef enum {
	OHMS, K_OHMS, M_OHMS
}OHMMETER_unitType;


float OHMMETER_getReading(ADS1015 *ads, OHMMETER_unitType * resUnit);

#endif /* HAL_OHMMETER_H_ */