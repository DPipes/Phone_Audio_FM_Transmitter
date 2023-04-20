#include <stdio.h>
#include "driver/gpio.h"
#include "i2c_main.h"

#define CHANGE_FREQ_PIN 19
#define PLAY_PAUSE_PIN  18
#define PREV_PIN        21
#define NEXT_PIN        16

/**
 * @brief Initialize the user input pins
 */
void input_init(void);