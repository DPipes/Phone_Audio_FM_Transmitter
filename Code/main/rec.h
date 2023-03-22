#include <stdio.h>
#include "i2c_main.h"

#define REC_ADDR        0x11
#define REC_RST_PIN     7

/**
 * @brief Write to receiever IC
 */
void rec_write(uint8_t reg_addr, uint8_t *data, size_t len);

/**
 * @brief Read from receiever IC
 */
void rec_read(uint8_t reg_addr, uint8_t *data, size_t len);

/**
 * @brief Initialize the receiver IC
 */
void rec_init(void);

/**
 * @brief Change receive frequency
 * 
 * @param freq New frequency in 10's of kHz. 101.1 MHz = 10110d = 0x277E
 */
void rec_freq(uint16_t freq);

/**
 * @brief Read received signal parameters
 * 
 * @param rssi Pointer to holder for received signal strength
 * @param snr Pointer to holder for received signal to noise ratio
 * @param mpi Pointer to holder for received multipath interference
 */
void rec_param(uint8_t *rssi, uint8_t *snr, uint8_t *mpi);