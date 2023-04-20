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
#include "input.h"
#include "fm.h"

void app_main(void)
{
    /* Initialize */
    i2c_init();
    rclk_init();
    blt_init();
    trans_init(); //be sure to set digital input format
    rec_init();
    input_init();
    disp_init();
    
    

    /* Variables for current and previous GPIO state */
    int freq = 1;
    int play = 1;
    int prev = 1;
    int next = 1;
    int freq_ = 1;
    int play_ = 1;
    int prev_ = 1;
    int next_ = 1;

    char text[20];
    int len;

    uint16_t new_freq = 0;
    uint8_t new_rssi = 0;

    //tesing the transmitter and reciever
    uint8_t trans_tune_response[8] = {};
    uint16_t trans_freq = 0;

    uint8_t int_status = 0;

    uint8_t rssi = 0;
    uint8_t snr = 0;
    uint8_t mpi = 0;

    uint16_t rec_freq = 0;
    uint8_t rec_tune_response[8] = {};
    uint16_t set_freq = 10330;

    trans_set_freq_full(set_freq);
    rec_set_freq_full(set_freq);
    len = freq_to_string(set_freq, text);
    disp_text(text, len, 0);
    

    /* Main loop */
    while (true) {

        // TODO Check if this can be done with any GPIO functions rather than having to track current and previous pin state
        /* Get current GPIO state */
        // freq_ = gpio_get_level(CHANGE_FREQ_PIN);
        // play_ = gpio_get_level(PLAY_PAUSE_PIN);
        // prev_ = gpio_get_level(PREV_PIN);
        // next_ = gpio_get_level(NEXT_PIN);

        /* On button press perform functions */
        if (!freq_ && freq) {

            // trans_set_freq_full(new_freq);
            // len = freq_to_string(new_freq, text);
            // disp_text(text, len, 0);

            set_freq = set_freq + 10;
            if (set_freq > 10800) set_freq = 8800;
            len = freq_to_string(set_freq, text);
            disp_text(text, len, 0);
        }
        // if (!play_ && play) blt_play_pause();
        // if (!prev_ && prev) {
        //     // blt_prev();
        //     set_freq = set_freq - 10;
        //     if (set_freq < 8800) set_freq = 10800;
        //     len = freq_to_string(set_freq, text);
        //     disp_text(text, len, 0);
        // }
        // // if (!next_ && next) blt_next();

        // /* Update previous GPIO state */
        // freq = freq_;
        // play = play_;
        // prev = prev_;
        // next = next_;
           
        set_freq = set_freq + 20;
        if(set_freq > 10800) set_freq = 8790;
        rec_set_freq_full(set_freq);

        rec_tune_status(rec_tune_response);
        rssi = rec_tune_response[4];

        if ((rssi < new_rssi) || (set_freq == new_freq)) {
            new_freq = set_freq;
            new_rssi = rssi;
        }

        //reciever and transmitter stuff

        /*int_status = trans_get_int_status();
        printf("int status = %x\n", int_status);
        
        trans_tune_status(trans_tune_response);
        trans_freq = (trans_tune_response[2] << 8) + (trans_tune_response[3]);

        printf("transmitting frequency: %d\n", trans_freq);*/
        
        rec_set_freq_full(set_freq);

        int_status = rec_get_int_status();
        printf("int status = %x\n", int_status);

        rec_tune_status(rec_tune_response);
        rec_freq = (rec_tune_response[2] << 8) + (rec_tune_response[3]);
        rssi = rec_tune_response[4];
        snr = rec_tune_response[5];
        mpi = rec_tune_response[6];

        printf("receiving frequency: %d\n", rec_freq);
        printf("rssi: %d\n", rssi);
        printf("snr: %d\n", snr);
        printf("mpi: %d\n", mpi);

        sprintf(text, "%d", rssi);
        disp_text(text, strlen(text), 1);

        check_buttons();
    }
    
    /* De-Initialize */
    i2c_driver_delete(I2C_MASTER_NUM);
    printf("I2C de-initialized successfully");
}
