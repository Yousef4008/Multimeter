/*
 * OHMMETER.c
 *
 *  Created on: Jun 30, 2024
 *      Author: Youssif Hossam
 */

#include "ohmmeter.h"


float OHMMETER_getReading(ADS1015 *ads,uint8 * resUnit){
	float resistance, adc2;
	GPIO_setupPinDirection(OHMMETER_RES_1_PORT_ID, OHMMETER_RES_1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(OHMMETER_RES_2_PORT_ID, OHMMETER_RES_2_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(OHMMETER_RES_3_PORT_ID, OHMMETER_RES_3_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(OHMMETER_RES_4_PORT_ID, OHMMETER_RES_4_PIN_ID, PIN_INPUT);
	GPIO_writePin(OHMMETER_RES_1_PORT_ID, OHMMETER_RES_1_PIN_ID, LOGIC_LOW);
	adc2 = ADS1015_readADC_SingleEnded(ads, OHMMETER_ADC_CHANNEL);
	resistance = OHMMETER_RES_1_VALUE*((1666/adc2)-1);

	if(resistance < 1000){
		*resUnit = OHMS;
	}
	else if(resistance < 2000){
		*resUnit = K_OHMS;
		GPIO_setupPinDirection(OHMMETER_RES_1_PORT_ID, OHMMETER_RES_1_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(OHMMETER_RES_2_PORT_ID, OHMMETER_RES_2_PIN_ID, PIN_OUTPUT);
		GPIO_setupPinDirection(OHMMETER_RES_3_PORT_ID, OHMMETER_RES_3_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(OHMMETER_RES_4_PORT_ID, OHMMETER_RES_4_PIN_ID, PIN_INPUT);
		GPIO_writePin(OHMMETER_RES_2_PORT_ID, OHMMETER_RES_2_PIN_ID, LOGIC_LOW);
		adc2 = ADS1015_readADC_SingleEnded(ads, OHMMETER_ADC_CHANNEL);
		resistance = OHMMETER_RES_2_VALUE*((1666/adc2)-1);	}
	else if(resistance < 20000){
		*resUnit = K_OHMS;
		GPIO_setupPinDirection(OHMMETER_RES_1_PORT_ID, OHMMETER_RES_1_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(OHMMETER_RES_2_PORT_ID, OHMMETER_RES_2_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(OHMMETER_RES_3_PORT_ID, OHMMETER_RES_3_PIN_ID, PIN_OUTPUT);
		GPIO_setupPinDirection(OHMMETER_RES_4_PORT_ID, OHMMETER_RES_4_PIN_ID, PIN_INPUT);
		GPIO_writePin(OHMMETER_RES_3_PORT_ID, OHMMETER_RES_3_PIN_ID, LOGIC_LOW);
		adc2 = ADS1015_readADC_SingleEnded(ads, OHMMETER_ADC_CHANNEL);
		resistance = OHMMETER_RES_3_VALUE*((1666/adc2)-1);	}
	else{
		GPIO_setupPinDirection(OHMMETER_RES_1_PORT_ID, OHMMETER_RES_1_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(OHMMETER_RES_2_PORT_ID, OHMMETER_RES_2_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(OHMMETER_RES_3_PORT_ID, OHMMETER_RES_3_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(OHMMETER_RES_4_PORT_ID, OHMMETER_RES_4_PIN_ID, PIN_OUTPUT);
		GPIO_writePin(OHMMETER_RES_4_PORT_ID, OHMMETER_RES_4_PIN_ID, LOGIC_LOW);
		adc2 = ADS1015_readADC_SingleEnded(ads, OHMMETER_ADC_CHANNEL);
		resistance = OHMMETER_RES_4_VALUE*((1666/adc2)-1);		if(resistance > 1000){
			resistance /= 1000;
			*resUnit = M_OHMS;
		}
		else{
			*resUnit = K_OHMS;
		}
	}

	return resistance;
}
