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

    /* Main loop */
    while (true) {
        
    }
    
    /* De-Initialize */
    i2c_driver_delete(I2C_MASTER_NUM);
    printf("I2C de-initialized successfully");
}
