/*
 * voltmeter.h
 *
 *  Created on: Jul 2, 2024
 *      Author: Youssif Hossam
 */

#ifndef HAL_VOLTMETER_VOLTMETER_H_
#define HAL_VOLTMETER_VOLTMETER_H_

#include "../../MCAL/GPIO/gpio.h"
#include "../../HAL/ADS1015/ads1015.h"
#include "../../LIB/std_types.h"

#define VOLTMETER_RES_RATIO			11
#define ADS_STEP_MV					3

float VOLTMETER_getReading(ADS1015 *ads);

#endif /* HAL_VOLTMETER_VOLTMETER_H_ */
