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
 * @brief Change transmit frequency, write and read response
 * 
 * @param freq New frequency in 10's of kHz. 101.1 MHz = 10110d = 0x277E
 */
void trans_set_freq_full(uint16_t freq);

/**
 * @brief Change transmit frequency, ony writes
 * 
 * @param freq New frequency in 10's of kHz. 101.1 MHz = 10110d = 0x277E
 */
void trans_set_freq_write(uint16_t freq);

/**
 * @brief Change transmit frequency, ony writes
 * 
 * @param power sets power in dBuV, range 88-115
 */
void trans_set_power_write(uint8_t power);

/**
 * @brief gets the interrupt satatus, includes delay, 1ms
 * can only be sent during powerup mode
 * 
 * @param *response the response bit
 */
void trans_get_int_status(uint8_t *response);

/**
 * @brief gets the interrupt satatus, includes delay, 1ms,
 * returns the status byte,
 * can only be sent during powerup mode
 */
uint8_t trans_get_int_status(void);

/**
 * @brief tells what frequency and power level we 
 * are currently transmitting
 * 
 * @param *response the response bit
 */
void trans_tune_status(uint8_t *response);

/**
 * @brief sets a property, gets status byte
 * 
 * @param *prop the property to set
 * @param *val the value to set the property to
 * @param *response the response bit
 */
void trans_set_property(uint16_t prop, uint16_t val, uint8_t *response);

/**
 * @brief sets a property, sends command only
 * 
 * @param *prop the property to set
 * @param *val the value to set the property to
 */
void trans_set_property_write(uint16_t prop, uint16_t val);
