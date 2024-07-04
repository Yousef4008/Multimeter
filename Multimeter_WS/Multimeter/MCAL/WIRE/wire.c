/*
 * wire.c
 *
 *  Created on: Jul 1, 2024
 *      Author: Youssif Hossam
 */


#include "wire.h"

static uint8_t rxBuffer[BUFFER_LENGTH];
static uint8_t rxBufferIndex = 0;
static uint8_t rxBufferLength = 0;

static uint8_t txAddress = 0;
static uint8_t txBuffer[BUFFER_LENGTH];
static uint8_t txBufferIndex = 0;

void Wire_init(uint32_t scl_clock) {
    TWSR = 0x00;
    TWBR = ((F_CPU / scl_clock) - 16) / 2;
    TWCR = (1 << TWEN);
}

void Wire_begin() {
    Wire_init(100000L); // Default 100kHz SCL clock
}

void Wire_beginWithAddress(uint8_t address) {
    TWAR = (address << 1);
    Wire_begin();
}

void Wire_beginTransmission(uint8_t address) {
    txAddress = address;
    txBufferIndex = 0;
}

uint8_t Wire_endTransmission() {
    return Wire_endTransmissionWithStop(1);
}

uint8_t Wire_endTransmissionWithStop(uint8_t sendStop) {
    uint8_t i;

    // Send start condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    // Send slave address
    TWDR = (txAddress << 1) | TW_WRITE;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    // Check for ACK/NACK
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK) return 1;

    // Write data
    for (i = 0; i < txBufferIndex; i++) {
        TWDR = txBuffer[i];
        TWCR = (1 << TWINT) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)));
        if ((TWSR & 0xF8) != TW_MT_DATA_ACK) return 2;
    }

    // Send stop condition if required
    if (sendStop) {
        TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
        while (TWCR & (1 << TWSTO));
    }

    return 0;
}

uint8_t Wire_requestFrom(uint8_t address, uint8_t quantity) {
    return Wire_requestFromWithStop(address, quantity, 1);
}

uint8_t Wire_requestFromWithStop(uint8_t address, uint8_t quantity, uint8_t sendStop) {
    uint8_t count = 0;

    // Send start condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    // Send slave address
    TWDR = (address << 1) | TW_READ;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    // Check for ACK/NACK
    if ((TWSR & 0xF8) != TW_MR_SLA_ACK) return 0;

    // Read data
    while (count < quantity) {
        if (count == quantity - 1)
            TWCR = (1 << TWINT) | (1 << TWEN); // NACK on last byte
        else
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // ACK on other bytes

        while (!(TWCR & (1 << TWINT)));

        rxBuffer[count++] = TWDR;
    }

    rxBufferIndex = 0;
    rxBufferLength = count;

    // Send stop condition if required
    if (sendStop) {
        TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
        while (TWCR & (1 << TWSTO));
    }

    return count;
}

void Wire_write(uint8_t data) {
    if (txBufferIndex >= BUFFER_LENGTH) return; // Buffer overflow
    txBuffer[txBufferIndex++] = data;
}

uint8_t Wire_read() {
    if (rxBufferIndex >= rxBufferLength) return 0; // No more data
    return rxBuffer[rxBufferIndex++];
}

int Wire_available() {
    return rxBufferLength - rxBufferIndex;
}
