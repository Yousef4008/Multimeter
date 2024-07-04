/*
 * voltmeter.c
 *
 *  Created on: Jul 2, 2024
 *      Author: Youssif Hossam
 */


#include "voltmeter.h"


float VOLTMETER_getReading(ADS1015 *ads){
	float adc, volt;
	adc = ADS1015_readADC_Differential_0_1(ads);;
	volt = (adc*ADS_STEP_MV*VOLTMETER_RES_RATIO/1000);
	return volt;
}
