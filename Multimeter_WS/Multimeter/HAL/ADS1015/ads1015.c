/*
 * adc1015.c
 *
 *  Created on: Jul 1, 2024
 *      Author: Youssif Hossam
 */

#include "ads1015.h"
#include "../../MCAL/WIRE/wire.h"
#include <util/delay.h>

static uint8_t i2cread(void) {

	return Wire_read();

}

static void i2cwrite(uint8_t x) {

	Wire_write((uint8_t)x);

}

static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
	Wire_beginTransmission(i2cAddress);
	i2cwrite((uint8_t)reg);
	i2cwrite((uint8_t)(value>>8));
	i2cwrite((uint8_t)(value & 0xFF));
	Wire_endTransmission();
}

static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
	Wire_beginTransmission(i2cAddress);
	i2cwrite(ADS1015_REG_POINTER_CONVERT);
	Wire_endTransmission();
	Wire_requestFrom(i2cAddress, (uint8_t)2);
	return ((i2cread() << 8) | i2cread());
}

void ADS1015_init(ADS1015* ads, uint8_t i2cAddress)
{
	ads->i2cAddress = i2cAddress;
	ads->conversionDelay = ADS1015_CONVERSIONDELAY;
	ads->bitShift = 4;
	ads->gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
}

void ADS1015_begin(ADS1015* ads)
{
	Wire_begin();
}

void ADS1015_setGain(ADS1015* ads, adsGain gain)
{
	ads->gain = gain;
}

adsGain ADS1015_getGain(ADS1015* ads)
{
	return ads->gain;
}

uint16_t ADS1015_readADC_SingleEnded(ADS1015* ads, uint8_t channel){
	if (channel > 3)
	{
		return 0;
	}

	// Start with default values
	uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
			ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
			ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
			ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
			ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
			ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

	// Set PGA/voltage range
	config |= ads->gain;

	// Set single-ended input channel
	switch (channel)
	{
	case (0):
	    		  config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
	break;
	case (1):
	    		  config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
	break;
	case (2):
	    		  config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
	break;
	case (3):
	    		  config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
	break;
	}

	// Set 'start single-conversion' bit
	config |= ADS1015_REG_CONFIG_OS_SINGLE;

	// Write config register to the ADC
	writeRegister(ads->i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

	// Wait for the conversion to complete
	_delay_ms(ads->conversionDelay);

	// Read the conversion results
	// Shift 12-bit results right 4 bits for the ADS1015
	return readRegister(ads->i2cAddress, ADS1015_REG_POINTER_CONVERT) >> ads->bitShift;
}

/**************************************************************************/
/*!
    @brief  Reads the conversion results, measuring the voltage
            difference between the P (AIN0) and N (AIN1) input.  Generates
            a signed value since the difference can be either
            positive or negative.
*/
/**************************************************************************/
int16_t ADS1015_readADC_Differential_0_1(ADS1015* ads)
{
  // Start with default values
  uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= ads->gain;

  // Set channels
  config |= ADS1015_REG_CONFIG_MUX_DIFF_0_1;          // AIN0 = P, AIN1 = N

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(ads->i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  _delay_ms(ads->conversionDelay);

  // Read the conversion results
  uint16_t res = readRegister(ads->i2cAddress, ADS1015_REG_POINTER_CONVERT) >> ads->bitShift;
  if (ads->bitShift == 0)
  {
    return (int16_t)res;
  }
  else
  {
    // Shift 12-bit results right 4 bits for the ADS1015,
    // making sure we keep the sign bit intact
    if (res > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (int16_t)res;
  }
}

/**************************************************************************/
/*!
    @brief  Reads the conversion results, measuring the voltage
            difference between the P (AIN2) and N (AIN3) input.  Generates
            a signed value since the difference can be either
            positive or negative.
*/
/**************************************************************************/
int16_t ADS1015_readADC_Differential_2_3(ADS1015* ads) {
  // Start with default values
  uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= ads->gain;

  // Set channels
  config |= ADS1015_REG_CONFIG_MUX_DIFF_2_3;          // AIN2 = P, AIN3 = N

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(ads->i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  _delay_ms(ads->conversionDelay);

  // Read the conversion results
  uint16_t res = readRegister(ads->i2cAddress, ADS1015_REG_POINTER_CONVERT) >> ads->bitShift;
  if (ads->bitShift == 0)
  {
    return (int16_t)res;
  }
  else
  {
    // Shift 12-bit results right 4 bits for the ADS1015,
    // making sure we keep the sign bit intact
    if (res > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (int16_t)res;
  }
}

/**************************************************************************/
/*!
    @brief  Sets up the comparator to operate in basic mode, causing the
            ALERT/RDY pin to assert (go from high to low) when the ADC
            value exceeds the specified threshold.

            This will also set the ADC in continuous conversion mode.
*/
/**************************************************************************/
void ADS1015_startComparator_SingleEnded(ADS1015* ads, uint8_t channel, int16_t threshold)
{
  // Start with default values
  uint16_t config = ADS1015_REG_CONFIG_CQUE_1CONV   | // Comparator enabled and asserts on 1 match
                    ADS1015_REG_CONFIG_CLAT_LATCH   | // Latching mode
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_CONTIN  | // Continuous conversion mode
                    ADS1015_REG_CONFIG_MODE_CONTIN;   // Continuous conversion mode

  // Set PGA/voltage range
  config |= ads->gain;

  // Set single-ended input channel
  switch (channel)
  {
    case (0):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
      break;
  }

  // Set the high threshold register
  // Shift 12-bit results left 4 bits for the ADS1015
  writeRegister(ads->i2cAddress, ADS1015_REG_POINTER_HITHRESH, threshold << ads->bitShift);

  // Write config register to the ADC
  writeRegister(ads->i2cAddress, ADS1015_REG_POINTER_CONFIG, config);
}

/**************************************************************************/
/*!
    @brief  In order to clear the comparator, we need to read the
            conversion results.  This function reads the last conversion
            results without changing the config value.
*/
/**************************************************************************/
int16_t ADS1015_getLastConversionResults(ADS1015* ads)
{
  // Wait for the conversion to complete
  _delay_ms(ads->conversionDelay);

  // Read the conversion results
  uint16_t res = readRegister(ads->i2cAddress, ADS1015_REG_POINTER_CONVERT) >> ads->bitShift;
  if (ads->bitShift == 0)
  {
    return (int16_t)res;
  }
  else
  {
    // Shift 12-bit results right 4 bits for the ADS1015,
    // making sure we keep the sign bit intact
    if (res > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (int16_t)res;
  }
}