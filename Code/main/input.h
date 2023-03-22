#include <stdio.h>
#include "driver/gpio.h"
#include "i2c_main.h"

#define CHANGE_FREQ_PIN 1
#define PLAY_PAUSE_PIN  2
#define PREV_PIN        4
#define NEXT_PIN        5

/**
 * @brief Initialize the user input pins
 */
void input_init(void);