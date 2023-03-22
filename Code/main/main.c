#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "i2c_main.h"
#include "display.h"
#include "blt.h"
#include "rec.h"
#include "trans.h"
#include "input.h"
#include "fm.h"

void app_main(void)
{

    /* Initialize */
    i2c_init();
    printf("I2C initialized successfully");
    trans_init();
    printf("Transmitter initialized successfully");
    rec_init();
    printf("Receiver initialized successfully");
    blt_init();
    printf("Bluetooth initialized successfully");
    input_init();
    printf("User Input initialized successfully");
    disp_init();
    printf("Display initialized successfully");

    int freq = 1;
    int play = 1;
    int prev = 1;
    int next = 1;
    int freq_ = 1;
    int play_ = 1;
    int prev_ = 1;
    int next_ = 1;

    /* Main loop */
    while (true) {
        freq_ = gpio_get_level(CHANGE_FREQ_PIN);
        play_ = gpio_get_level(PLAY_PAUSE_PIN);
        prev_ = gpio_get_level(PREV_PIN);
        next_ = gpio_get_level(NEXT_PIN);

        if (freq_ && !freq) change_freq();
        if (play_ && !play) blt_play_pause();
        if (prev_ && !prev) blt_prev();
        if (next_ && !next) blt_next();

        freq = freq_;
        play = play_;
        prev = prev_;
        next = next_;
    }
    
    /* De-Initialize */
    i2c_driver_delete(I2C_MASTER_NUM);
    printf("I2C de-initialized successfully");
}
