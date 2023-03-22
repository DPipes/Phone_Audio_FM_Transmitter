#include <stdio.h>
#include "i2c_main.h"

#define TRANS_ADDR      0x63
#define TRANS_RST_PIN   8

/**
 * @brief Write to transmitter IC
 */
void trans_write(uint8_t reg_addr, uint8_t *data, size_t len);

/**
 * @brief Read from transmitter IC
 */
void trans_read(uint8_t reg_addr, uint8_t *data, size_t len);

/**
 * @brief Initialize the transmitter IC
 */
void trans_init(void);

/**
 * @brief Change transmit frequency
 * 
 * @param freq New frequency in 10's of kHz. 101.1 MHz = 10110d = 0x277E
 */
void trans_freq(uint16_t freq);