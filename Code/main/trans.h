#include <stdio.h>
#include "i2c_main.h"
#include "fm_ic.h"

#define TRANS_ADDR			0x63
#define TRANS_RST_PIN		26

#define AUDIO_SAMPLE_RATE	44100

/* Commands and Arguments */
#define POWER_UP			0x01
#define POWER_TRANSMIT		0x02
#define POWER_DIGITAL_AUDIO	0x0F

#define SET_PROPERTY		0x12

#define GET_INT_STATUS		0x14

#define	TX_TUNE_FREQ		0x30

#define TX_TUNE_POWER		0x31

#define TX_TUNE_STATUS		0x33
#define TUNE_INT_CLR		0x01

#define TX_RDS_BUFF			0x35

#define TX_RDS_PS			0x36

/* Properties */
#define DIGITAL_INPUT_FORMAT		0x0101
#define DIGITAL_INPUT_SAMPLE_RATE	0x0103
#define REFCLK_FREQ					0x0201
#define RCLK_PRESCALE				0x0202
#define TX_COMPONENT_ENABLE			0x2100
#define TX_AUDIO_DEVIATION			0x2101
#define TX_PILOT_DEVIATION			0x2102
#define TX_RDS_DEVIATION			0x2103
#define TX_LINE_INPUT_LEVEL_MUTE	0x2105
#define TX_PREEMPHASIS				0x2106
#define TX_PILOT_FREQUENCY			0x2107
#define TX_RDS_INTERRUPT_SOURCE		0x2C00
#define TX_RDS_PI					0x2C01
#define TX_RDS_PS_MIX				0x2C02
#define TX_RDS_PS_MISC				0x2C03
#define TX_RDS_PS_REPEAT_COUNT		0x2C04
#define TX_RDS_PS_MESSAGE_COUNT		0x2C05
#define TX_RDS_PS_AF				0x2C06
#define TX_RDS_FIFO_SIZE			0x2C07

/* Transmitter specific delay lengths in milliseconds */
#define	TRANS_STC_DELAY		100

/**
 * @brief Write to transmitter IC
 *
 * @param command  Command to write to transmitter
 * @param *args    Pointer to arguments to write
 * @param num_args Number of arguments to write
 */
void trans_write(uint8_t command, uint8_t* args, size_t num_args);

/**
 * @brief Read from transmitter IC
 *
 * @param *data Pointer to hold response
 * @param len   Length of response in bytes
 */
void trans_read(uint8_t* data, size_t len);

/**
 * @brief Initialize the transmitter IC
 */
void trans_init(void);

/**
 * @brief Writes power up command to transmitter.
 */
void trans_power_up(void);

/**
 * @brief Change transmit frequency
 *
 * @param freq New frequency in 10's of kHz.
 *		       101.1 MHz = 10110d = 0x277E
 */
void trans_tune_freq(uint16_t freq);

/**
 * @brief Sets desired transmission power
 *
 * @param power Desired power level in dBuV.
 *			    Acceptable values from 88-115
 */
void trans_tune_power(uint8_t power);

/**
 * @brief Reads and returns status byte from transmitter
 *		  Transmitter must be in powerup mode to use
 */
uint8_t trans_get_int_status(void);

/**
 * @brief Stores the frequency and power currently transmitting
 *		  to the given pointer
 *
 * @param *response Pointer to store response.
 *		   Response is 8 bytes and pointer needs enough space
 */
void trans_tune_status(uint8_t* response);

/**
 * @brief Set transmitter property
 *
 * @param prop Property to set
 * @param val  Value to set
 */
void trans_set_property(uint16_t prop, uint16_t val);

void trans_rds_ps_write(uint8_t psid, char* text);

void trans_rds_write(char* text, uint8_t len);