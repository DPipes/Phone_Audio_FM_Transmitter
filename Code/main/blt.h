#include <stdio.h>
#include "i2c_main.h"

/**
 * @brief Initialize the Bluetooth system
 */
void blt_init(void);

/**
 * @brief Send command to user device to pause audio playback
 */
void blt_play_pause(void);

/**
 * @brief Send command to user device to go to previous song
 */
void blt_prev(void);

/**
 * @brief Send command to user device to go to next song
 */
void blt_next(void);