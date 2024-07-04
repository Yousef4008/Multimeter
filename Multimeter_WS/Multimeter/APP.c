/*	____________________________________________________________________________
  |																			    |
  | Application: Multi-functional Meter										    |
  |																			    |
  | File Name: APP.c														    |
  |																			    |
  | Description: Main application file for a multi-functional meter			    |
  |																			    |
  | Author: Youssif Hossam													    |
  |_____________________________________________________________________________|
 */

#include "HAL/LCD/lcd.h"
#include "HAL/OHMMETER/ohmmeter.h"
#include "HAL/VOLTMETER/voltmeter.h"
#include "HAL/CAPACITANCE_METER/capacitance_meter.h"
#include "HAL/INDUCTANCE_METER/inductance_meter.h"
#include "HAL/CURRENT_METER/current_meter.h"
#include "HAL/ADS1015/ads1015.h"
#include "HAL/MODE_SELECTOR/mode_selector.h"

#include "MCAL/ADC/adc.h"
#include "MCAL/TIMER1/timer1.h"
#include "LIB/std_types.h"

#include <util/delay.h>
#include <avr/io.h>

/*______________________________________________________________________________
 |                                Main Function                                 |
 |______________________________________________________________________________|
 */

int main(void){

	/* Local Variables */
	ADC_ConfigType  ADC_MyConfig = {Ext_AVCC , PRESCALER_128}; /* ADC Configuration */
	ADS1015 ads; /* ADS1015 Configuration */

	float resistance, voltage, capacitance, inductance, current; /* Measurement variables */

	uint8 ohmSymbol[] = {0x0E, 0x11, 0x11, 0x11, 0x1B, 0x0A, 0x1B}; /* Custom symbol for Ohms */

	OHMMETER_unitType resistanceUnit; /* Resistance unit */
	CAPACITANCE_unitType capacitanceUnit = NO_SCALE; /* Capacitance unit */

	Mode_type currentMode, previousMode = NO_MODE; /* Mode selection */

	/* Enable global interrupts */
	SREG |= 1<<7;

	/* Initialization Functions */
	ADC_init(&ADC_MyConfig); /* Initialize ADC */

	ADS1015_init(&ads, ADS1015_ADDRESS); /* Initialize ADS1015 */
	ADS1015_begin(&ads); /* Begin ADS1015 */
	ADS1015_setGain(&ads, GAIN_TWOTHIRDS); /* Set gain for ADS1015 */

	LCD_init(); /* Initialize LCD */
	LCD_StoreCustomPtrn(ohmSymbol, 0); /* Store custom Ohm symbol */

	/* Welcome Message */

	LCD_displayString("  Auto-Ranging  ");
	LCD_moveCursor(1,0);
	LCD_displayString("   Multimeter   ");
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_displayString("  Developed By  ");
	LCD_moveCursor(1,0);
	LCD_displayString(" Youssif Hossam ");
	_delay_ms(1000);
	LCD_clearScreen();
	_delay_ms(500);


	/* Main Loop */
	while(1){

		/* Run mode selector to get current mode */
		MODE_SELECTOR_run(&currentMode, &previousMode);

		/* Clear previous measurement display */
		LCD_moveCursor(1,5);
		LCD_displayString("           ");
		LCD_moveCursor(1,5);

		/* Process current mode */
		switch(currentMode){

		case OHMMETER_MODE:
			/* Measure resistance */
			resistance = OHMMETER_getReading(&ads, &resistanceUnit);
			LCD_floatToString(resistance); /* Display resistance value */
			switch(resistanceUnit){
			case OHMS:
				LCD_DisplayCustomPtrn(0); /* Display Ohm symbol */
				break;
			case K_OHMS:
				LCD_displayCharacter('K');
				LCD_DisplayCustomPtrn(0);
				break;
			case M_OHMS:
				LCD_displayCharacter('M');
				LCD_DisplayCustomPtrn(0);
				break;
			}
			break;

			case VOLTMETER_MODE:
				/* Measure voltage */
				voltage = VOLTMETER_getReading(&ads);
				LCD_floatToString(voltage); /* Display voltage value */
				LCD_displayCharacter('V');
				break;

			case CAPMETER_MODE:
				/* Measure capacitance */
				capacitance = CAPACITANCE_METER_getReading(&capacitanceUnit);
				if(capacitanceUnit != NO_SCALE){
					LCD_floatToString(capacitance); /* Display capacitance value */
					switch(capacitanceUnit){
					case PICO_FARAD:
						LCD_displayString("pF");
						break;
					case NANO_FARAD:
						LCD_displayString("nF");
						break;
					case MICRO_FARAD:
						LCD_displayString("uF");
						break;
					case MILLI_FARAD:
						LCD_displayString("mF");
						break;
					case FARAD:
						LCD_displayString("F");
						break;
					case NO_SCALE:
						/* Do Nothing .. */
						break;
					}
				}
				break;

			case INDUCTOMETER_MODE:
				/* Measure inductance */
				inductance = INDUCTANCE_METER_getReading();
				if(inductance < 1000){
					LCD_floatToString(inductance); /* Display inductance value */
					LCD_displayString("uH");
				}
				else{
					LCD_floatToString(inductance/1000); /* Display inductance value */
					LCD_displayString("mH");
				}


				break;

			case CURRENTMETER_MODE:
				/* Measure current */
				current = CURRENT_METER_getReading(&ads);
				if(current < 1){
					LCD_floatToString(current*1000); /* Display current in mA */
					LCD_displayString("mA");
				}
				else{
					LCD_floatToString(current); /* Display current in A */
					LCD_displayString("A");
				}
				break;

			case NO_MODE:
				/* Do Nothing .. */
				break;
		}
		_delay_ms(500); /* Delay between measurements */
	}

	return 0; /* Return from main function */
}