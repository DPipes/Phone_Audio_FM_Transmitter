#include <stdio.h>
#include "i2c_main.h"

#define TRANS_ADDR      0x63
#define TRANS_RST_PIN   8

/**
 * @brief send a command to transmitter with arguments 
 * and recieve the response
 */
void trans_command_full(uint8_t command, uint8_t *args, size_t num_args, uint16_t delay, uint8_t *response, size_t resp_len);

/**
 * @brief Write to transmitter IC
 */
void trans_command_write(uint8_t reg_addr, uint8_t *data, size_t len);

/**
 * @brief Read from transmitter IC
 */
void trans_read(uint8_t *data, size_t len);

/**
 * @brief Initialize the transmitter IC
 */
void trans_init(void);

/**
 * @brief boot normally, interupts off, RCLK off, transmit mode
 */
void trans_power_up_std(uint8_t *response);

/**
 * @brief Change transmit frequency
 * 
 * @param freq New frequency in 10's of kHz. 101.1 MHz = 10110d = 0x277E
 */
void trans_freq(uint16_t freq);