#include <stdio.h>
#include "i2c_main.h"

#define REC_ADDR        0x11
#define REC_RST_PIN     27

/**
 * @brief send a command to receiver with arguments 
 * and recieve the response
 */
void rec_command_full(uint8_t command, uint8_t *args, size_t num_args, 
uint16_t delay, uint8_t *response, size_t resp_len);

/**
 * @brief Write to receiever IC
 */
void rec_command_write(uint8_t command, uint8_t *args, size_t num_args);

/**
 * @brief Read from receiever IC
 */
void rec_read(uint8_t *data, size_t len);

/**
 * @brief Initialize the receiver IC
 */
void rec_init(void);

/**
 * @brief boot normally, interupts off, RCLK off, 
 * recieve mode, *analog outputs?*
 */
void rec_power_up_std(uint8_t *response);

/**
 * @brief Change receive frequency
 * 
 * @param freq New frequency in 10's of kHz. 101.1 MHz = 10110d = 0x277E
 */
void rec_set_freq_full(uint16_t freq);

/**
 * @brief Change receive frequency
 * 
 * @param freq New frequency in 10's of kHz. 101.1 MHz = 10110d = 0x277E
 */
void rec_set_freq_write(uint16_t freq);

/**
 * @brief gets the interrupt satatus, includes delay, 1ms
 * can only be sent during powerup mode
 * 
 * @param *response the response bit
 */
//void rec_get_int_status(uint8_t *response);

/**
 * @brief gets the interrupt satatus, includes delay, 1ms,
 * returns the status byte,
 * can only be sent during powerup mode
 */
uint8_t rec_get_int_status(void);

/**
 * @brief tells what frequency and power level we 
 * are currently transmitting
 * 
 * @param *response the response bit
 */
void rec_tune_status(uint8_t *response);

void rec_rsq_status(uint8_t *response);

/**
 * @brief Read received signal parameters
 * 
 * @param rssi Pointer to holder for received signal strength
 * @param snr Pointer to holder for received signal to noise ratio
 * @param mpi Pointer to holder for received multipath interference
 */
void rec_param(uint8_t *rssi, uint8_t *snr, uint8_t *mpi);

/**
 * @brief sets a property, gets status byte
 * 
 * @param *prop the property to set
 * @param *val the value to set the property to
 * @param *response the response bit
 */
void rec_set_property(uint16_t prop, uint16_t val, uint8_t *response);

/**
 * @brief sets a property, sends command only
 * 
 * @param *prop the property to set
 * @param *val the value to set the property to
 */
void rec_set_property_write(uint16_t prop, uint16_t val);

/**
 * @brief sets the frequency of the reference clock at rclk input
 * 
 * @param *freq the frequency in hz
 */
void rec_set_refclk_freq(uint16_t freq);

// /**
//  * @brief sets the prescale value of reference clock
//  * reference clock frquency is the freuqncy at the rclk input
//  * divided by the prescale fvalue
//  * 
//  * @param *freq the frequency in hz
//  * @param *rclk 0 if ussing the rclk pin, 1 if using the dclk pin
//  */
// void rec_set_refclk_prescale(bool rclk, uint16_t freq);