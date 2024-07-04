/*	____________________________________________________________________________
  |																			    |
  | Driver: Current Meter													    |
  |																			    |
  | File Name: current_meter.h												    |
  |																			    |
  | Description: Header file for current meter driver						    |
  |																			    |
  | Author: Youssif Hossam													    |
  |_____________________________________________________________________________|
 */

#ifndef HAL_CURRENT_METER_CURRENT_METER_H_
#define HAL_CURRENT_METER_CURRENT_METER_H_

#include "../../LIB/std_types.h"
#include "../ADS1015/ads1015.h"

/*______________________________________________________________________________
 |                               Definitions                                    |
 |______________________________________________________________________________|
 */

#define CURRENT_METER_VOUT_ADS_CHANNEL_NUM				3
#define CURRENT_METER_ACS712ELCTR_20A_T_SENSITIVITY		0.1
#define CURRENT_METER_CURRENT_METER_VREF                5.0 // Reference voltage for the ADC
#define CURRENT_METER_ZERO_CURRENT_VOLTAGE              (CURRENT_METER_CURRENT_METER_VREF / 2) // Voltage at zero current
#define CURRENT_METER_VOLTAGE_NUM_SAMPLES				10
#define CURRENT_METER_AC_THRESHOLD                      0.05 // Threshold for standard deviation to detect AC

typedef enum {
    CURRENT_DC,
    CURRENT_AC
} CURRENT_Type;

/*______________________________________________________________________________
 |                               Functions Prototypes                           |
 |______________________________________________________________________________|
 */

/*______________________________________________________________________________
 | Function: CURRENT_METER_getReading                                           |
 | Description: Measures the current and returns its value                      |
 | Parameters:                                                                  |
 |    - ads: Pointer to ADS1015 instance                                        |
 | Returns:                                                                     |
 |    - float: Measured current value                                           |
 |______________________________________________________________________________|
 */
float CURRENT_METER_getReading(ADS1015 *ads);


#endif /* HAL_CURRENT_METER_CURRENT_METER_H_ */