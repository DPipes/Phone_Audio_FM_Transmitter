#include <stdio.h>
#include "driver/gpio.h"
#include "i2c_main.h"
#include "input.h"

void input_init(void) {

    /* Reset GPIO pins */
    gpio_reset_pin(CHANGE_FREQ_PIN);
    gpio_reset_pin(PLAY_PAUSE_PIN);
    gpio_reset_pin(PREV_PIN);
    gpio_reset_pin(NEXT_PIN);

    /* Set the GPIO pins as inputs */
    gpio_set_direction(CHANGE_FREQ_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(PLAY_PAUSE_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(PREV_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(NEXT_PIN, GPIO_MODE_INPUT);

    /* Enable GPIO pull-up resistors */
    gpio_pullup_en(CHANGE_FREQ_PIN);
    gpio_pullup_en(PLAY_PAUSE_PIN);
    gpio_pullup_en(PREV_PIN);
    gpio_pullup_en(NEXT_PIN);
}