/*
 * mode_selector.c
 *
 *  Created on: Jul 2, 2024
 *      Author: Youssif Hossam
 */


#include "mode_selector.h"

void MODE_SELECTOR_run(Mode_type * currentMode, Mode_type * previousMode){
	uint16 adc_value;
	adc_value = ADC_readChannel(0);
	if(adc_value > 900){
		*currentMode = OHMMETER_MODE;

	}
	else if(adc_value > 700) {
		*currentMode = VOLTMETER_MODE;

	}
	else if (adc_value > 500){
		*currentMode = CAPMETER_MODE;

	}
	else if (adc_value > 300){
		*currentMode = INDUCTOMETER_MODE;

	}
	else{
		*currentMode = CURRENTMETER_MODE;

	}

	if(*currentMode != *previousMode){
		*previousMode = *currentMode;
		LCD_clearScreen();
		switch(*currentMode){
		case OHMMETER_MODE:
			LCD_displayString("   Resistance");
			break;
		case VOLTMETER_MODE:
			LCD_displayString("    Voltage");
			break;
		case CAPMETER_MODE:
			LCD_displayString("   Capacitance");
			break;
		case INDUCTOMETER_MODE:
			LCD_displayString("   Inductance");
			break;
		case CURRENTMETER_MODE:
			LCD_displayString("     Current");
			break;
		case NO_MODE:
			/* Do Nothing .. */
			break;
		}
	}
}