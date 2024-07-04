/*
 * mode_selector.h
 *
 *  Created on: Jul 2, 2024
 *      Author: Youssif Hossam
 */

#ifndef HAL_MODE_SELECTOR_MODE_SELECTOR_H_
#define HAL_MODE_SELECTOR_MODE_SELECTOR_H_


#include "../../MCAL/ADC/adc.h"
#include "../../HAL/LCD/lcd.h"
#include "../../LIB/std_types.h"

typedef enum {
	OHMMETER_MODE, VOLTMETER_MODE, CAPMETER_MODE, INDUCTOMETER_MODE, CURRENTMETER_MODE, NO_MODE
}Mode_type;

void MODE_SELECTOR_run(Mode_type * currentMode, Mode_type * previousMode);
#endif /* HAL_MODE_SELECTOR_MODE_SELECTOR_H_ */
