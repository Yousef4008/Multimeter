/*	____________________________________________________________________________
  |																			    |
  | Driver: Inductance Meter												    |
  |																			    |
  | File Name: inductance_meter.c											    |
  |																			    |
  | Description: Source file for inductance meter driver					    |
  |																			    |
  | Author: Youssif Hossam													    |
  |_____________________________________________________________________________|
 */

#include "inductance_meter.h"
#include <util/delay.h>
#include <math.h>

/*______________________________________________________________________________
 |                           Functions Definitions                              |
 |______________________________________________________________________________|
 */

/*
 * Description :
 * -> Function responsible for getting the inductance reading.
 * -> This function measures the inductance by charging the inductor, measuring the
 *    oscillation period, and calculating the inductance based on the period.
 */
float INDUCTANCE_METER_getReading() {
	float inductance;
	Timer1_ConfigType Timer1_Config = {0, 0, NO_PRESCALER, NORMAL_MODE};

	GPIO_setupPinDirection(INDUCTANCE_METER_COMPARATOR_PORT_ID, INDUCTANCE_METER_COMPARATOR_PIN_ID, PIN_INPUT);
	GPIO_setupPinDirection(INDUCTANCE_METER_CHARGING_PORT_ID, INDUCTANCE_METER_CHARGING_PIN_ID, PIN_OUTPUT);

	// Charge the inductor
	GPIO_writePin(INDUCTANCE_METER_CHARGING_PORT_ID, INDUCTANCE_METER_CHARGING_PIN_ID, LOGIC_HIGH);
	_delay_ms(5);
	GPIO_writePin(INDUCTANCE_METER_CHARGING_PORT_ID, INDUCTANCE_METER_CHARGING_PIN_ID, LOGIC_LOW);

	if (GPIO_readPin(INDUCTANCE_METER_COMPARATOR_PORT_ID, PIN3_ID) == LOGIC_HIGH) {
		// Wait for the comparator to change state
		while (GPIO_readPin(INDUCTANCE_METER_COMPARATOR_PORT_ID, INDUCTANCE_METER_COMPARATOR_PIN_ID) == LOGIC_HIGH);
		Timer1_init(&Timer1_Config);

		// Measure the oscillation period
		while (GPIO_readPin(INDUCTANCE_METER_COMPARATOR_PORT_ID, INDUCTANCE_METER_COMPARATOR_PIN_ID) == LOGIC_LOW);
		while (GPIO_readPin(INDUCTANCE_METER_COMPARATOR_PORT_ID, INDUCTANCE_METER_COMPARATOR_PIN_ID) == LOGIC_HIGH);
		while (GPIO_readPin(INDUCTANCE_METER_COMPARATOR_PORT_ID, INDUCTANCE_METER_COMPARATOR_PIN_ID) == LOGIC_LOW);
		while (GPIO_readPin(INDUCTANCE_METER_COMPARATOR_PORT_ID, INDUCTANCE_METER_COMPARATOR_PIN_ID) == LOGIC_HIGH);

		// Get the timer count
		inductance = Timer1_getCount();
		Timer1_deInit();
	} else {
		// Wait for the comparator to change state
		while (GPIO_readPin(INDUCTANCE_METER_COMPARATOR_PORT_ID, INDUCTANCE_METER_COMPARATOR_PIN_ID) == LOGIC_LOW);
		Timer1_init(&Timer1_Config);

		// Measure the oscillation period
		while (GPIO_readPin(INDUCTANCE_METER_COMPARATOR_PORT_ID, INDUCTANCE_METER_COMPARATOR_PIN_ID) == LOGIC_HIGH);
		while (GPIO_readPin(INDUCTANCE_METER_COMPARATOR_PORT_ID, INDUCTANCE_METER_COMPARATOR_PIN_ID) == LOGIC_LOW);
		while (GPIO_readPin(INDUCTANCE_METER_COMPARATOR_PORT_ID, INDUCTANCE_METER_COMPARATOR_PIN_ID) == LOGIC_HIGH);
		while (GPIO_readPin(INDUCTANCE_METER_COMPARATOR_PORT_ID, INDUCTANCE_METER_COMPARATOR_PIN_ID) == LOGIC_LOW);

		// Get the timer count
		inductance = Timer1_getCount();
		Timer1_deInit();
	}

	// Calculate the inductance based on the measured period
	inductance /= (F_CPU_MEGA_HZ * 2);
	inductance = (inductance * inductance) / (4 * (M_PI * M_PI) * INDUCTANCE_METER_CAPACITOR_MICROFARAD_VALUE);

	return inductance;
}