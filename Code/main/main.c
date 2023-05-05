#include <stdio.h>
#include <string.h>
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
#include "fm.h"
#include "fm_ic.h"

void app_main(void)
{
    /* Initialize */
    i2c_init();
    rclk_init();
    blt_init();
    disp_init();
    fm_init();

    /* Main loop */
    while (true) {
        /* Increment receiver frequency */
        step_freq();

        /* Check if frequency should be changed */
        if (check_buttons() == CHANGE_FREQ_PIN) change_freq();
    }
}
