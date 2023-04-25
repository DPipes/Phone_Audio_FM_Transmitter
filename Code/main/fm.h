#include <stdio.h>
#include <string.h>
#include "trans.h"
#include "rec.h"
#include "driver/ledc.h"
#include "display.h"
#include "fm_ic.h"

#define RCLK_FREQ	32768
#define RCLK_PIN	17

#define START_FREQ	8810
#define STOP_FREQ	10790
#define FREQ_STEP	20
#define NUM_CHANS	100

#define SUM_LEN		5	// Length of rolling sum

#define RSSI_THRESH 20

#define SPACING		5

/* Channel struct to track rolling rssi values */
struct Chan {
	uint16_t freq;
	uint8_t rssi[SUM_LEN];
	uint16_t rssi_sum;
};

/**
 * @brief Initialize RCLK PWM
 */
void rclk_init(void);

/**
 * @brief Initialize FM IC's and frequency array
 */
void fm_init(void);

/**
 * @brief Step the receiving frequency, store rssi, and update channel rolling sum
 */
void step_freq(void);

/**
 * @brief Find the optimal frequency and set to broadcast
 */
void change_freq(void);

/**
 * @brief Find the minimums in the FM band signal strength
 *
 * @param *chans Pointer to array of channels
 * @param **vals Pointer to store array of indices for minimum positions organized in order of increasing RSSI
 *
 * @return uint8_t Number of minimums
 */
uint8_t find_mins(struct Chan* chans, uint8_t* mins);