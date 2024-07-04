/*	____________________________________________________________________________
  |																			    |
  | Driver: Current Meter													    |
  |																			    |
  | File Name: current_meter.c												    |
  |																			    |
  | Description: Source file for current meter driver						    |
  |																			    |
  | Author: Youssif Hossam													    |
  |_____________________________________________________________________________|
 */

#include "current_meter.h"
#include <math.h>

/*______________________________________________________________________________
 |                               Functions Definitions                          |
 |______________________________________________________________________________|
 */

/*______________________________________________________________________________
 | Function: CURRENT_METER_detectType                                           |
 | Description: Detects if the current is AC or DC                              |
 | Parameters:                                                                  |
 |    - ads: Pointer to ADS1015 instance                                        |
 | Returns:                                                                     |
 |    - CURRENT_Type: Detected current type (AC or DC)                          |
 |______________________________________________________________________________|
 */
static CURRENT_Type CURRENT_METER_detectType(ADS1015 *ads) {
    uint16 adcValue;
    float voltage, current;
    float sum = 0.0, sumSq = 0.0, mean, variance, stddev;
    float currents[CURRENT_METER_VOLTAGE_NUM_SAMPLES];

    for (uint8 i = 0; i < CURRENT_METER_VOLTAGE_NUM_SAMPLES; i++) {
        adcValue = ADS1015_readADC_SingleEnded(ads, CURRENT_METER_VOUT_ADS_CHANNEL_NUM); // Read ADC value
        voltage = (float)adcValue / 333.33;
        current = (voltage - CURRENT_METER_ZERO_CURRENT_VOLTAGE) / CURRENT_METER_ACS712ELCTR_20A_T_SENSITIVITY; // Convert voltage to current
        currents[i] = current; // Store current value

        sum += current; // Sum of current values
        sumSq += current * current; // Sum of squares of current values
    }

    mean = sum / CURRENT_METER_VOLTAGE_NUM_SAMPLES; // Calculate mean
    variance = (sumSq / CURRENT_METER_VOLTAGE_NUM_SAMPLES) - (mean * mean); // Calculate variance
    stddev = sqrt(variance); // Calculate standard deviation

    if (stddev > CURRENT_METER_AC_THRESHOLD) {
        return CURRENT_AC; // If standard deviation is above threshold, it's AC
    } else {
        return CURRENT_DC; // If standard deviation is below threshold, it's DC
    }
}

/*______________________________________________________________________________
 | Function: CURRENT_METER_getReading                                           |
 | Description: Measures the current and returns its value                      |
 | Parameters:                                                                  |
 |    - ads: Pointer to ADS1015 instance                                        |
 | Returns:                                                                     |
 |    - float: Measured current value                                           |
 |______________________________________________________________________________|
 */
float CURRENT_METER_getReading(ADS1015 *ads) {
    CURRENT_Type currentType = CURRENT_METER_detectType(ads); // Detect current type
    uint16 adcValue;
    float voltage, current, sum = 0.0;
    uint8 numSamples = (currentType == CURRENT_AC) ? CURRENT_METER_VOLTAGE_NUM_SAMPLES : 10;

    for (uint8 i = 0; i < numSamples; i++) {
        adcValue = ADS1015_readADC_SingleEnded(ads, CURRENT_METER_VOUT_ADS_CHANNEL_NUM); // Read ADC value
        voltage = (float)adcValue / 333.33;
        current = (voltage - CURRENT_METER_ZERO_CURRENT_VOLTAGE) / CURRENT_METER_ACS712ELCTR_20A_T_SENSITIVITY; // Convert voltage to current

        if (currentType == CURRENT_AC) {
            sum += current * current; // Square the current and add to sum for AC
        } else {
            sum += current; // Sum the current values for DC
        }
    }

    if (currentType == CURRENT_AC) {
        current = sqrt(sum / numSamples); // Calculate RMS current for AC
    } else {
        current = sum / numSamples; // Calculate average current for DC
    }

    return current;
}