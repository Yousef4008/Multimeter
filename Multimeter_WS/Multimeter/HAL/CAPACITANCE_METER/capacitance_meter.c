/*	____________________________________________________________________________
  |																			    |
  | Driver: Capacitance Meter												    |
  |																			    |
  | File Name: capacitance_meter.c											    |
  |																			    |
  | Description: Source file for capacitance meter driver					    |
  |																			    |
  | Author: Youssif Hossam													    |
  |_____________________________________________________________________________|
 */

#include "capacitance_meter.h"
#include <util/delay.h>

/*______________________________________________________________________________
 |                           Functions Definitions                              |
 |______________________________________________________________________________|
 */

/*
 * Description :
 * -> Function responsible for getting the capacitance reading.
 * -> This function measures the capacitance by charging the capacitor through a
 *    resistor and measuring the time taken to reach a certain voltage level.
 * -> It also handles different ranges of capacitance and scales the result accordingly.
 */
float CAPACITANCE_METER_getReading(CAPACITANCE_unitType * capUnit) {
	float capacitance;
	uint16 adcValue;
	uint32 time;
	static uint8 cap_scale = 0;  // Scale flag for different capacitance ranges
	Timer1_ConfigType Timer1_Config = {0, 0, CLK_256_PRESCALER, NORMAL_MODE};

	if (!cap_scale) {
		// Setup pin directions for charging and discharging resistors and capacitor terminals
		GPIO_setupPinDirection(CAPACITANCE_METER_CHARGING_RES_1_PORT_ID, CAPACITANCE_METER_CHARGING_RES_1_PIN_ID, PIN_OUTPUT);
		GPIO_setupPinDirection(CAPACITANCE_METER_DISCHARGING_RES_PORT_ID, CAPACITANCE_METER_DISCHARGING_RES_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(CAPACITANCE_METER_CHARGING_RES_2_PORT_ID, CAPACITANCE_METER_CHARGING_RES_2_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(CAPACITANCE_METER_POSITIVE_CAP_PORT_ID, CAPACITANCE_METER_POSITIVE_CAP_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(CAPACITANCE_METER_NEGATIVE_CAP_PORT_ID, CAPACITANCE_METER_NEGATIVE_CAP_PIN_ID, PIN_OUTPUT);

		// Initial discharge of the capacitor
		GPIO_writePin(CAPACITANCE_METER_NEGATIVE_CAP_PORT_ID, CAPACITANCE_METER_NEGATIVE_CAP_PIN_ID, LOGIC_LOW);
		GPIO_writePin(CAPACITANCE_METER_CHARGING_RES_1_PORT_ID, CAPACITANCE_METER_CHARGING_RES_1_PIN_ID, LOGIC_LOW);
		_delay_ms(50);

		// Start charging the capacitor
		GPIO_writePin(CAPACITANCE_METER_CHARGING_RES_1_PORT_ID, CAPACITANCE_METER_CHARGING_RES_1_PIN_ID, LOGIC_HIGH);
		Timer1_Config.prescaler = NO_PRESCALER;
		Timer1_init(&Timer1_Config);

		// Wait until the capacitor voltage reaches a certain level
		while (ADC_readChannel(CAPACITANCE_METER_POSITIVE_CAP_PIN_ID) < 648);

		// Calculate the capacitance based on the charging time
		capacitance = (float)Timer1_getCount() / 160000;
		Timer1_deInit();

		// Scale the capacitance value and set the appropriate unit
		if (capacitance > 1.0F) {
			*capUnit = MICRO_FARAD;
		} else if (capacitance > 0.1F) {
			*capUnit = NANO_FARAD;
			capacitance *= 1000;
		} else {
			cap_scale = 1;  // Switch to a different scale for smaller capacitances
			*capUnit = NO_SCALE;
		}

		// Discharge the capacitor
		GPIO_writePin(CAPACITANCE_METER_CHARGING_RES_1_PORT_ID, CAPACITANCE_METER_CHARGING_RES_1_PIN_ID, LOGIC_LOW);
		GPIO_setupPinDirection(CAPACITANCE_METER_DISCHARGING_RES_PORT_ID, CAPACITANCE_METER_DISCHARGING_RES_PIN_ID, PIN_OUTPUT);
		GPIO_writePin(CAPACITANCE_METER_DISCHARGING_RES_PORT_ID, CAPACITANCE_METER_DISCHARGING_RES_PIN_ID, LOGIC_LOW);
		while (ADC_readChannel(CAPACITANCE_METER_POSITIVE_CAP_PIN_ID) > 0);
		GPIO_setupPinDirection(CAPACITANCE_METER_DISCHARGING_RES_PORT_ID, CAPACITANCE_METER_DISCHARGING_RES_PIN_ID, PIN_INPUT);
	} else {
		// Setup pin directions for charging and discharging resistors and capacitor terminals
		GPIO_setupPinDirection(CAPACITANCE_METER_CHARGING_RES_1_PORT_ID, CAPACITANCE_METER_CHARGING_RES_1_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(CAPACITANCE_METER_DISCHARGING_RES_PORT_ID, CAPACITANCE_METER_DISCHARGING_RES_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(CAPACITANCE_METER_CHARGING_RES_2_PORT_ID, CAPACITANCE_METER_CHARGING_RES_2_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(CAPACITANCE_METER_POSITIVE_CAP_PORT_ID, CAPACITANCE_METER_POSITIVE_CAP_PIN_ID, PIN_INPUT);
		GPIO_setupPinDirection(CAPACITANCE_METER_NEGATIVE_CAP_PORT_ID, CAPACITANCE_METER_NEGATIVE_CAP_PIN_ID, PIN_OUTPUT);

		// Measure the ADC value to determine capacitance
		GPIO_writePin(CAPACITANCE_METER_NEGATIVE_CAP_PORT_ID, CAPACITANCE_METER_NEGATIVE_CAP_PIN_ID, LOGIC_HIGH);
		adcValue = ADC_readChannel(CAPACITANCE_METER_POSITIVE_CAP_PIN_ID);
		GPIO_writePin(CAPACITANCE_METER_POSITIVE_CAP_PORT_ID, CAPACITANCE_METER_POSITIVE_CAP_PIN_ID, LOGIC_LOW);

		if (adcValue < 1018) {
			// Calculate the capacitance based on the ADC value
			GPIO_setupPinDirection(CAPACITANCE_METER_POSITIVE_CAP_PORT_ID, CAPACITANCE_METER_POSITIVE_CAP_PIN_ID, PIN_OUTPUT);
			GPIO_writePin(CAPACITANCE_METER_POSITIVE_CAP_PORT_ID, CAPACITANCE_METER_POSITIVE_CAP_PIN_ID, LOGIC_HIGH);
			capacitance = (float)adcValue * 6 / (float)(CAPACITANCE_METER_ADC_MAX_VALUE - adcValue);
			GPIO_writePin(CAPACITANCE_METER_POSITIVE_CAP_PORT_ID, CAPACITANCE_METER_POSITIVE_CAP_PIN_ID, LOGIC_LOW);
			GPIO_writePin(CAPACITANCE_METER_NEGATIVE_CAP_PORT_ID, CAPACITANCE_METER_NEGATIVE_CAP_PIN_ID, LOGIC_LOW);
			_delay_ms(50);
			*capUnit = PICO_FARAD;
		} else {
			// Switch to a different method for larger capacitances
			GPIO_setupPinDirection(CAPACITANCE_METER_CHARGING_RES_1_PORT_ID, CAPACITANCE_METER_CHARGING_RES_1_PIN_ID, PIN_INPUT);
			GPIO_setupPinDirection(CAPACITANCE_METER_DISCHARGING_RES_PORT_ID, CAPACITANCE_METER_DISCHARGING_RES_PIN_ID, PIN_INPUT);
			GPIO_setupPinDirection(CAPACITANCE_METER_CHARGING_RES_2_PORT_ID, CAPACITANCE_METER_CHARGING_RES_2_PIN_ID, PIN_OUTPUT);
			GPIO_setupPinDirection(CAPACITANCE_METER_POSITIVE_CAP_PORT_ID, CAPACITANCE_METER_POSITIVE_CAP_PIN_ID, PIN_INPUT);
			GPIO_setupPinDirection(CAPACITANCE_METER_NEGATIVE_CAP_PORT_ID, CAPACITANCE_METER_NEGATIVE_CAP_PIN_ID, PIN_OUTPUT);

			// Start charging the capacitor
			GPIO_writePin(CAPACITANCE_METER_NEGATIVE_CAP_PORT_ID, CAPACITANCE_METER_NEGATIVE_CAP_PIN_ID, LOGIC_LOW);
			GPIO_writePin(CAPACITANCE_METER_CHARGING_RES_2_PORT_ID, CAPACITANCE_METER_CHARGING_RES_2_PIN_ID, LOGIC_HIGH);
			Timer1_Config.prescaler = NO_PRESCALER;
			Timer1_init(&Timer1_Config);

			// Wait until the capacitor voltage reaches a certain level
			while (ADC_readChannel(CAPACITANCE_METER_POSITIVE_CAP_PIN_ID) < 648);

			// Calculate the capacitance based on the charging time
			time = Timer1_getCount() / 16;
			capacitance = (float)time / 1000;
			Timer1_deInit();

			// Discharge the capacitor
			GPIO_writePin(CAPACITANCE_METER_CHARGING_RES_2_PORT_ID, CAPACITANCE_METER_CHARGING_RES_2_PIN_ID, LOGIC_LOW);
			_delay_ms(time / 20000);

			if (capacitance > 1000.0) {
				cap_scale = 0;  // Change the scale to 1uF - max
			} else {
				*capUnit = NANO_FARAD;
			}
		}
	}

	return capacitance;
}
