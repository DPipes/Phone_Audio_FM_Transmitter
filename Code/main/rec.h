#include <stdio.h>
#include "i2c_main.h"
#include "fm_ic.h"

#define REC_ADDR			0x11
#define REC_RST_PIN			27

#define RSSI				4	//Position of rssi in rec_tune_status response
#define SNR					5	//Position of snr in rec_tune_status response
#define MPI					6	//Position of mpi in rec_tune_status response

/* Commands and Arguments */
#define POWER_UP			0x01
#define POWER_RECEIVE		0x00
#define POWER_ANALOG_AUDIO	0x05

#define SET_PROPERTY		0x12

#define GET_INT_STATUS		0x14

#define	FM_TUNE_FREQ		0x20

#define FM_TUNE_STATUS		0x22
#define SEEK_CANCEL			0x02
#define TUNE_INT_CLR		0x01

#define FM_RSQ_STATUS		0x23
#define RSQ_INT_CLR			0x01

/* Properties */
#define DIGITAL_OUTPUT_FORMAT		0x0102
#define DIGITAL_OUTPUT_SAMPLE_RATE	0x0104
#define REFCLK_FREQ					0x0201
#define RCLK_PRESCALE				0x0202
#define FM_DEEMPHASIS				0x1100
#define FM_ANTENNA_INPUT			0x1107
#define FM_MAX_TUNE_ERROR			0x1108
#define RX_VOLUME					0x4000
#define RX_HARD_MUTE				0x4001

/* Receiver specific delay lengths in milliseconds */
#define	REC_STC_DELAY		60

/**
 * @brief Write to receiever IC
 *
 * @param command  Command to write to receiver
 * @param *args    Pointer to arguments to write
 * @param num_args Number of arguments to write
 */
void rec_write(uint8_t command, uint8_t* args, size_t num_args);

/**
 * @brief Read from receiever IC
 *
 * @param *data Pointer to hold response
 * @param len   Length of response in bytes
 */
void rec_read(uint8_t* data, size_t len);

/**
 * @brief Initialize the receiver IC
 */
void rec_init(void);

/**
 * @brief Writes power up command to receiver.
 */
void rec_power_up(void);

/**
 * @brief Change receive frequency
 *
 * @param freq New frequency in 10's of kHz
 *			   101.1 MHz = 10110d = 0x277E
 */
void rec_tune_freq(uint16_t freq);

/**
 * @brief Reads and returns status byte from receiver
 *		  Receiver must be in powerup mode to use
 */
uint8_t rec_get_int_status(void);

/**
 * @brief Stores the frequency and tune status to the given pointer
 *
 * @param *response Pointer to store response.
 *		   Response is 8 bytes and pointer needs enough space
 */
void rec_tune_status(uint8_t* response);

/**
 * @brief Stores the reception measurements to the given pointer
 *
 * @param *response Pointer to store response.
 *		   Response is 8 bytes and pointer needs enough space
 */
void rec_rsq_status(uint8_t* response);

/**
 * @brief Set receiver property
 *
 * @param *prop Property to set
 * @param *val  Value to set
 */
void rec_set_property(uint16_t prop, uint16_t val);