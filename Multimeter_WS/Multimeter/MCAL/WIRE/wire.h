/*
 * wire.h
 *
 *  Created on: Jul 1, 2024
 *      Author: Youssif Hossam
 */

#ifndef MCAL_WIRE_WIRE_H_
#define MCAL_WIRE_WIRE_H_



#include <avr/io.h>
#include <util/twi.h>

#define BUFFER_LENGTH 32

void Wire_begin();
void Wire_beginWithAddress(uint8_t address);
void Wire_beginTransmission(uint8_t address);
uint8_t Wire_endTransmission();
uint8_t Wire_endTransmissionWithStop(uint8_t sendStop);
uint8_t Wire_requestFrom(uint8_t address, uint8_t quantity);
uint8_t Wire_requestFromWithStop(uint8_t address, uint8_t quantity, uint8_t sendStop);
void Wire_write(uint8_t data);
uint8_t Wire_read();
int Wire_available();



#endif /* MCAL_WIRE_WIRE_H_ */
