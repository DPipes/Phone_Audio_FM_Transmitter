#include <stdio.h>
#include "trans.h"
#include "rec.h"

#define RCLK_FREQ 32768 
#define RCLK_PIN 17

struct Chan {
	uint16_t freq;
	uint8_t rssi;
	uint16_t freq2;
	uint8_t rssi2;
};

/**
 * @brief Initialize RCLK PWM
 */
void rclk_init(void);

/**
 * @brief Find the optimal frequency and set to broadcast
 */
void change_freq(void);